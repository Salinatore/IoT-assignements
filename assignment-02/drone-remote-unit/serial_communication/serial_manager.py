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

    async def start(self, message_handler: Callable[[str], Awaitable[None]]):
        """Start serial connection. Creates reader and writer tasks"""
        self.message_handler = message_handler
        self.reader, self.writer = await serial_asyncio.open_serial_connection(
            url=self.port, baudrate=self.baud
        )

        asyncio.create_task(self._reader_task())
        asyncio.create_task(self._writer_task())

    async def send(self, msg: str):
        """Enqueue message to be sent through serial"""
        await self.command_queue.put(msg)

    async def _reader_task(self):
        """Reads from serial continuously"""
        if self.reader:
            while True:
                line = await self.reader.readline()
                msg = line.decode().strip()

                if self.message_handler:
                    await self.message_handler(msg)

    async def _writer_task(self):
        """Sends commands through serial continuously if the queue is not empty"""
        if self.writer:
            while True:
                msg = await self.command_queue.get()
                self.writer.write((msg + "\n").encode())
                await self.writer.drain()
