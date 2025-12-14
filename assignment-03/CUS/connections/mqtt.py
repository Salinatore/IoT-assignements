import asyncio

from paho.mqtt.client import CallbackAPIVersion, Client
from typing_extensions import Awaitable, Callable


class MqttMenager:
    def __init__(self, message_handler: Callable[[str], Awaitable[None]]):
        self._client = Client(CallbackAPIVersion.VERSION2)

        self._client.on_connect = self._on_connect
        self._client.on_message = self._on_message

    async def start(
        self,
    ):
        asyncio.create_task(self._recive_task())

    async def _recive_task(self):
        self._client.loop_forever()

    @staticmethod
    def _on_connect(client, userdata, flags, reason_code, properties):
        print(f"Connected with result code {reason_code}")
        client.subscribe("esiot-2025-fgor")

    @staticmethod
    def _on_message(client, userdata, msg):
        print(msg.topic + " " + str(msg.payload))
        # real handeling here
