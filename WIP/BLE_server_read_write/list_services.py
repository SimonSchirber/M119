
import asyncio
import sys

from bleak import BleakClient, BleakScanner
from bleak.exc import BleakError
import bleak


ADDRESS = ("58:BF:25:9B:35:52")


async def main(ble_address: str):
    device = await BleakScanner.find_device_by_address(ble_address, timeout=20.0)
    if not device:
        raise BleakError(f"A device with address {ble_address} could not be found.")
    async with BleakClient(device) as client:
        print("Services:")
        for service in client.services:
            print(service)


if __name__ == "__main__":
    asyncio.run(main(sys.argv[1] if len(sys.argv) == 2 else ADDRESS))