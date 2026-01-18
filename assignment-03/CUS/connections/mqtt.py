import asyncio
import logging
from typing import Callable

from aiomqtt import Client

logger = logging.getLogger(__name__)


class MqttConnection:
    """Manages Mqtt connection"""

    def __init__(self, broker: str, topic: str):
        self._broker = broker
        self._topic = topic

    async def start(self, message_handler: Callable[[str], None]) -> None:
        """Start mqtt listener task"""
        self._handle_message = message_handler
        asyncio.create_task(self._listener_loop())

    async def _listener_loop(self) -> None:
        """Listen on mqtt port for messages continuously"""
        async with Client(hostname=self._broker) as client:
            await client.subscribe(self._topic)
            async for message in client.messages:
                if isinstance(message.payload, bytes):
                    self._handle_message(message.payload.decode("utf-8"))
                else:
                    logger.error(f"Received non-string payload: {message.payload}")
