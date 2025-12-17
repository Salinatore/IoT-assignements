import asyncio
import logging
from typing import Callable

import serial_asyncio

logger = logging.getLogger(__name__)


class SerialManager:
    """Manages Serial connection"""

    def __init__(self, port: str, baud: int):
        self._port = port
        self._baud = baud
        self._command_queue = asyncio.Queue()
        self._running = False
        self._reader = None
        self._writer = None

    async def start(self, message_handler: Callable[[str], None]):
        """Start serial connection. Creates reader and writer tasks"""
        self._message_handler = message_handler
        self._reader, self._writer = await serial_asyncio.open_serial_connection(
            url=self._port, baudrate=self._baud
        )

        asyncio.create_task(self._reader_loop())
        asyncio.create_task(self._writer_loop())

    async def send(self, msg: str):
        """Enqueue message to be sent through serial"""
        await self._command_queue.put(msg)

    async def _reader_loop(self):
        """Reads from serial continuously"""
        if self._reader:
            while True:
                line = await self._reader.readline()
                msg = line.decode().strip()

                if self._message_handler:
                    self._message_handler(msg)

    async def _writer_loop(self):
        """Sends commands through serial continuously if the queue is not empty"""
        if self._writer:
            while True:
                msg = await self._command_queue.get()
                self._writer.write((msg + "\n").encode())
                await self._writer.drain()
