import asyncio
from typing import Awaitable, Callable

import serial_asyncio


class SerialManager:
    """Manages Serial connection to Arduino"""

    def __init__(self, port: str, baud: int):
        self.port = port
        self.baud = baud
        self.command_queue = asyncio.Queue()
        self.running = False
        self.reader = None
        self.writer = None
        self.read_task = None
        self.write_task = None

    async def start(self, message_handler: Callable[[str], Awaitable[None]]):
        self.message_handler = message_handler
        self.reader, self.writer = await serial_asyncio.open_serial_connection(
            url=self.port, baudrate=self.baud
        )

        self.read_task = asyncio.create_task(self._reader_task())
        self.write_task = asyncio.create_task(self._writer_task())
        self.running = True

    async def stop(self):
        """Stop serial communication and clean up resources"""
        self.running = False

        if self.read_task:
            self.read_task.cancel()
        if self.write_task:
            self.write_task.cancel()

        if self.writer:
            try:
                self.writer.close()
                await self.writer.wait_closed()
            except Exception as e:
                print(f"Error closing writer: {e}")

        self.reader = None
        self.writer = None
        self.read_task = None
        self.write_task = None

    async def send(self, msg: str):
        """Called by GUI"""
        await self.command_queue.put(msg)

    async def _reader_task(self):
        """Reads from Arduino continuously"""
        if self.reader:
            while self.running:
                line = await self.reader.readline()
                msg = line.decode().strip()

                if self.message_handler:
                    await self.message_handler(msg)

    async def _writer_task(self):
        """Sends commands from GUI to Arduino"""
        if self.writer:
            while self.running:
                msg = await self.command_queue.get()
                self.writer.write((msg + "\n").encode())
                await self.writer.drain()
