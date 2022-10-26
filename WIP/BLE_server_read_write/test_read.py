import sys
import platform
import asyncio
import logging

from bleak import BleakClient

logger = logging.getLogger(__name__)

ADDRESS = ("58:BF:25:9B:35:52")


async def main(address):
    async with BleakClient(address) as client:
        logger.info(f"Connected: {client.is_connected}")

        for service in client.services:
            logger.info(f"[Service] {service}")
            for char in service.characteristics:
                if "read" in char.properties:
                    try:
                        value = bytes(await client.read_gatt_char(char.uuid))
                        for values in value:
                            print(value)
                    except Exception as e:
                        logger.error(
                            f"\t[Characteristic] {char} ({','.join(char.properties)}), Value: {e}"
                        )

                else:
                    value = None
                    logger.info(
                        f"\t[Characteristic] {char} ({','.join(char.properties)}), Value: {value}"
                    )

                for descriptor in char.descriptors:
                    try:
                        value = bytes(
                            await client.read_gatt_descriptor(descriptor.handle)
                        )
                        logger.info(f"\t\t[Descriptor] {descriptor}) | Value: {value}")
                    except Exception as e:
                        logger.error(f"\t\t[Descriptor] {descriptor}) | Value: {e}")


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    asyncio.run(main(sys.argv[1] if len(sys.argv) == 2 else ADDRESS))