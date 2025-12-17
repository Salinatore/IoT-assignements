import asyncio
import logging
from typing import Callable

from asyncio_mqtt import Client

logger = logging.getLogger(__name__)


class MqttManager:
    """Manages Mqtt connection"""

    def __init__(self, broker: str, topic: str):
        self._broker = broker
        self._topic = topic

    async def start(self, message_handler: Callable[[str], None]):
        """Start mqtt listener task"""
        self._handle_message = message_handler
        asyncio.create_task(self._listener_loop())

    async def _listener_loop(self):
        """Listen on mqtt port for messages continuously"""
        async with Client(hostname=self._broker) as client:
            await client.subscribe(self._topic)
            async with client.messages() as messages:
                async for message in messages:
                    self._handle_message(str(message.payload))
