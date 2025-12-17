import asyncio
import json
import logging
from ast import Await

from typing_extensions import Dict

from connections.websocket import WebSocketConnection
from model.model import State

logger = logging.getLogger(__name__)


class WebSocketHandler:
    """Handles all communication to websocket"""

    _MIN_PERCENTAGE = 0
    _MAX_PERCENTAGE = 100
    _ACCEPTABLE_STATES = ["AUTOMATIC", "REMOTE_MANUAL"]

    def __init__(self, connection_manager: WebSocketConnection, state: State):
        self._connection_manager = connection_manager
        self._state = state

    def handle_message_from_websocket(self, msg: str):
        asyncio.create_task(self._process_message_from_websocket(msg))

    def send_state_update_to_websocket(self):
        asyncio.create_task(
            self._connection_manager.broadcast(self._state.model_dump())
        )

    def _handle_switch_mode(self, data: dict):
        state = data["state"]
        if state not in self._ACCEPTABLE_STATES:
            logger.error(f"State change not acceptable. State: [{state}]")
            return

        self._state.set_mode(state)

    def _handle_update_op(self, data: dict):
        percentage = data["state"]
        if not self._MIN_PERCENTAGE <= percentage <= self._MAX_PERCENTAGE:
            logger.error(
                f"Percentage change not acceptable. Percentage: [{percentage}]"
            )
            return

        self._state.set_opening_percentage(percentage)

    async def _process_message_from_websocket(self, msg: str):
        data = json.loads(msg)
        match data["type"]:
            case "switch-mode":
                self._handle_switch_mode(data["data"])

            case "update-op":
                self._handle_update_op(data["data"])

            case _:
                logger.error(f"Unknown message type from WebSocket. Message: [{msg}]")
