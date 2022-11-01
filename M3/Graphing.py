import asyncio
import struct
from bleak import BleakClient
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import logging

plt.style.use('fivethirtyeight')
logger = logging.getLogger(__name__)
ADDRESS = ("58:BF:25:9B:35:52")
f_ax, f_ay, f_az, f_gx, f_gy, f_gz = 0, 0, 0, 0, 0, 0
sample = 0
x_vals = [0]
ax_vals = [0]
ay_vals = [0]
az_vals = [0]
gx_vals = [0]
gy_vals = [0]
gz_vals = [0]



def animate():
    global sample   
    sample +=1
    x_vals.append(sample)
    ax_vals.append(f_ax)
    ay_vals.append(f_ay)
    az_vals.append(f_az)
    gx_vals.append(f_gx)
    gy_vals.append(f_gy)
    gz_vals.append(f_gz)
    plt.cla()
    plt.plot(x_vals, ax_vals, label='ax')
    plt.plot(x_vals, ay_vals, label='ay')
    plt.plot(x_vals, az_vals, label='az')
    plt.legend(loc='upper left')
    plt.tight_layout()
    plt.draw()
    plt.pause(0.2)
      
    

def print3dec(ax, ay, az, gx, gy, gz):
    #use struct to convert byte to float in c notation
    global  f_ax, f_ay, f_az, f_gx, f_gy, f_gz
    [ax] = struct.unpack('f', ax)
    [ay] = struct.unpack('f', ay)
    [az] = struct.unpack('f', az)
    [gx] = struct.unpack('f', gx)
    [gy] = struct.unpack('f', gy)
    [gz] = struct.unpack('f', gz)
    f_ax, f_ay, f_az, f_gx, f_gy, f_gz = ax, ay, az, gx, gy, gz
    #print to 2 decimals the value of the float
    print(f'Ax :{ax:.2f}, Ay :{ay:.2f}, Az :{az:.2f}, Gx :{gx:.2f}, Gy :{gy:.2f}, Gz :{gz:.2f}')

async def read_ble_bytes(myclient, myuuid):
    try: 
        comb_byte_array = await myclient.read_gatt_char(myuuid)
        #Accelerometer X, Y, Z
        float1 = comb_byte_array[0:4]
        float2 = comb_byte_array[4:8]
        float3 = comb_byte_array[8:12]
        
        #Gyroscope X, Y, Z
        float4 = comb_byte_array[12:16]
        float5 = comb_byte_array[16:20]
        float6 = comb_byte_array[20:24]  
        #print data
        print3dec(float1, float2, float3, float4, float5, float6)
   
    except Exception as e:
        logger.error(e)
    animate()
    return 1
    

async def readBLEvalues(address):
    async with BleakClient(address) as client:
        for service in client.services:
            for char in service.characteristics:
                if (("read" in char.properties) and ("write" in char.properties)):
                    plt.ion()
                    
                    for i in range(200):
                        
                        await read_ble_bytes(client, char.uuid)
                        animate()
                    plt.show(block=True)
                        
                     
                    
                       
asyncio.run(readBLEvalues(ADDRESS))



