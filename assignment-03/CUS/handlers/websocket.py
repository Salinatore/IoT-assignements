import asyncio
import json
import logging

from connections.websocket import WebSocketConnection
from model.model import Mode, State

logger = logging.getLogger(__name__)


class WebSocketHandler:
    """Handles all communication to and from websockets."""

    _MIN_PERCENTAGE = 0
    _MAX_PERCENTAGE = 100
    _ACCEPTABLE_MODES = ["AUTOMATIC", "REMOTE_MANUAL"]

    def __init__(self, connection_manager: WebSocketConnection, state: State):
        self._connection_manager = connection_manager
        self._state = state

    def handle_message_from_websocket(self, msg: str):
        """
        Handle an incoming WebSocket message.

        Creates a background task without blocking.

        Args:
            msg: Raw JSON string message from WebSocket client
        """
        asyncio.create_task(self._process_message_from_websocket(msg))

    def send_state_update_to_websocket(self):
        """
        Broadcast the current state to all connected WebSocket clients.

        Creates a background task without blocking.
        """
        asyncio.create_task(
            self._connection_manager.broadcast(self._state.model_dump_json())
        )

    def generate_state_update(self) -> str:
        """
        Generate a JSON representation of the current state.

        Returns:
            JSON string containing the serialized current state
        """
        return self._state.model_dump_json()

    async def _process_message_from_websocket(self, msg: str):
        """Parse and route incoming WebSocket messages to appropriate handlers."""
        data = json.loads(msg)
        match data["type"]:
            case "switch-mode":
                self._handle_switch_mode(data["data"])
            case "update-op":
                self._handle_update_op(data["data"])
            case _:
                logger.error(f"Unknown message type from WebSocket. Message: [{msg}]")

    def _handle_switch_mode(self, data: dict):
        """Handle mode switching requests."""
        mode_srt = data["mode"]
        if mode_srt not in self._ACCEPTABLE_MODES:
            logger.error(f"Mode change not acceptable. Mode: [{mode_srt}]")
            return
        self._state.set_mode(Mode(mode_srt))

    def _handle_update_op(self, data: dict):
        """Handle opening percentage update requests."""
        percentage = data["percentage"]
        if not self._MIN_PERCENTAGE <= percentage <= self._MAX_PERCENTAGE:
            logger.error(
                f"Percentage change not acceptable. Percentage: [{percentage}]"
            )
            return
        self._state.set_opening_percentage(percentage)
