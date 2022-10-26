
import asyncio
import logging

from bleak import BleakClient

logger = logging.getLogger(__name__)

ADDRESS = ("58:BF:25:9B:35:52")


async def main(address):
    async with BleakClient(address) as client:
        

        for service in client.services:
            for char in service.characteristics:
                if "read" in char.properties:
                    try:
                        value = bytes(await client.read_gatt_char(char.uuid))
                        
                        for bytes in value:
                            byte_num = int.from_bytes(bytes, "big", signed="True")
                            print(byte_num)
                            
                    except Exception as e:
                        continue

                else:
                    value = None
asyncio.run(main(ADDRESS))
