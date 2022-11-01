import asyncio
import struct
from bleak import BleakClient
import time

myBLEchar = ""
myclient = ''
ADDRESS = ("58:BF:25:9B:35:52")

def print3dec(floatname, floatvalue):
    #use struct to convert byte to float in c notation
    [floatvalue] = struct.unpack('f', floatvalue)
    #print to 3 decimals the value of the float
    print(f'{floatname} :{floatvalue:.2f}')

async def readBLEvalues(address):
    global myBLEchar
    async with BleakClient(address) as client:
        
        for service in client.services:
            for char in service.characteristics:
                if (("read" in char.properties) and ("write" in char.properties)):
                    myBLEchar = char
                    #global myclient = BleakClient(address)
                    for i in range(5):
                        try: 
                            comb_byte_array = await client.read_gatt_char(char.uuid)
                            #Accelerometer X, Y, Z
                            float1 = comb_byte_array[0:4]
                            float2 = comb_byte_array[4:8]
                            float3 = comb_byte_array[8:12]
                            print3dec("Ax", float1)
                            print3dec("Ay", float2)
                            print3dec("Az", float3)
                            #Gyroscope X, Y, Z
                            float4 = comb_byte_array[12:16]
                            float5 = comb_byte_array[16:20]
                            float6 = comb_byte_array[20:24]
                            print3dec("Gx", float4)
                            print3dec("Gy", float5)
                            print3dec("Gz", float6)   
                        except:
                            continue
                        time.sleep(1)

asyncio.run(readBLEvalues(ADDRESS))

async def ReadBLE():  
    async with BleakClient(ADDRESS) as client:
        try:
            comb_byte_array = await client.read_gatt_char(myBLEchar.uuid)
            #Accelerometer X, Y, Z
            float1 = comb_byte_array[0:4]
            float2 = comb_byte_array[4:8]
            float3 = comb_byte_array[8:12]
            print3dec("Ax", float1)
            print3dec("Ay", float2)
            print3dec("Az", float3)
            #Gyroscope X, Y, Z
            float4 = comb_byte_array[12:16]
            float5 = comb_byte_array[16:20]
            float6 = comb_byte_array[20:24]
            print3dec("Gx", float4)
            print3dec("Gy", float5)
            print3dec("Gz", float6)   
        except:
            print("failed read")
for i in range(10):
    asyncio.run(ReadBLE())