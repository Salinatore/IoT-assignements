import asyncio
import logging
from typing import Any, Callable, Coroutine

from asyncio_mqtt import Client

logger = logging.getLogger(__name__)


class MqttManager:
    """Manages Mqtt connection"""

    def __init__(self, broker: str, topic: str):
        self._broker = broker
        self._topic = topic

    def start(self, handle_message: Callable[[str], Coroutine[Any, Any, None]]):
        """Start mqtt listener task"""
        self._handle_message = handle_message
        asyncio.create_task(self._listener_loop())

    async def _listener_loop(self):
        """Listen on mqtt port for messages continuously"""
        async with Client(hostname=self._broker) as client:
            await client.subscribe(self._topic)
            async with client.messages() as messages:
                async for message in messages:
                    asyncio.create_task(self._handle_message(str(message.payload)))
