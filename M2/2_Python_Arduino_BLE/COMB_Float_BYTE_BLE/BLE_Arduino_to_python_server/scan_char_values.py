
import asyncio
import logging
import struct

from bleak import BleakClient



ADDRESS = ("58:BF:25:9B:35:52")
CHAR = "19b10000-e8f2-537e-4f6c-d104768a1214 (Handle: 11): Unknown"

def print3dec(floatvalue):
    print(f'{floatvalue:.3f}')




async def main(address):
    async with BleakClient(address) as client:

        for service in client.services:
            print(service)
            for char in service.characteristics:
                print(char)
                if (("read" in char.properties) and ("write" in char.properties)):
                    print(char.properties)
                    try:
                        
                        my_value = await client.read_gatt_char(char.uuid)
                        float1 = my_value[0:4]
                    
                        [x] = struct.unpack('f', float1)
                        print(F"simons unpacking a float {x}")
                        print(f'{x:.3f}')
                        float1 = round(x, 3)
                    except:
                        continue

asyncio.run(main(ADDRESS))

# if __name__ == "__main__":
#     logging.basicConfig(level=logging.INFO)
#     asyncio.run(main(sys.argv[1] if len(sys.argv) == 2 else ADDRESS))