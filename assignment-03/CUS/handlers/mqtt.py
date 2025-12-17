import asyncio
import logging

from connections.mqtt import MqttConnection
from model.model import State

logger = logging.getLogger(__name__)


class MqttHandler:
    """Handles all communication from MQTT. All connection are not blocking"""

    # Message protocol constant
    _TSM_CUS_WATER_LEVEL_PREFIX = "tsm->cus-wl-"
    _EXPECTED_SENDER_PREFIX = "tsm->"

    def __init__(self, connection_manager: MqttConnection, state: State):
        self._connection_manager = connection_manager
        self._state = state

    def handle_message_from_mqtt(self, msg: str) -> None:
        """Handle incoming message from MQTT broker.

        Args:
            msg: Raw message string from MQTT
        """
        asyncio.create_task(self._process_incoming_message_async(msg))

    async def _process_incoming_message_async(self, msg: str):
        """Process incoming message from MQTT broker."""
        if not msg.startswith(self._EXPECTED_SENDER_PREFIX):
            logger.error(
                f"Invalid message sender. Expected 'tsm->' prefix. Message: [{msg}]"
            )
            return

        match msg:
            case _ if msg.startswith(self._TSM_CUS_WATER_LEVEL_PREFIX):
                self._handle_water_level_message(msg)
            case _:
                logger.error(f"Unknown message type from MQTT. Message: [{msg}]")

    def _handle_water_level_message(self, msg: str):
        """Extract and process water level from message."""
        water_level_str = msg.removeprefix(self._TSM_CUS_WATER_LEVEL_PREFIX)
        try:
            water_level = float(water_level_str)
            self._state.set_level(water_level)
        except ValueError:
            logger.error(f"Invalid water level value. Message: [{msg}]")
