
import asyncio
import logging
from bleak import BleakClient

logger = logging.getLogger(__name__)
###Add Device address from scan
ADDRESS = ("CF:DB:B6:15:63:35")

async def main(address):
    async with BleakClient(address) as client:
        logger.info(f"Connected: {client.is_connected}")
        for service in client.services:
            logger.info(f"[Service] {service}")
            for char in service.characteristics:
                if "read" in char.properties:
                    try:
                        value = bytes(await client.read_gatt_char(char.uuid))
                        logger.info(f"\t[Characteristic] {char} ({','.join(char.properties)}), Value: {value}")
                    except Exception as e:
                        logger.error(f"\t[Characteristic] {char} ({','.join(char.properties)}), Value: {e}")
                else:
                    value = None
                    logger.info(
                        f"\t[Characteristic] {char} ({','.join(char.properties)}), Value: {value}")
                for descriptor in char.descriptors:
                    try:
                        value = bytes(await client.read_gatt_descriptor(descriptor.handle))
                        logger.info(f"\t\t[Descriptor] {descriptor}) | Value: {value}")
                    except Exception as e:
                        logger.error(f"\t\t[Descriptor] {descriptor}) | Value: {e}")
logging.basicConfig(level=logging.INFO)
asyncio.run(main(ADDRESS))
