import asyncio
import json
import logging
from datetime import datetime
from typing import Literal

from pydantic import BaseModel, Field, TypeAdapter, ValidationError

from connections.websocket import WebSocketConnection
from model.model import Mode, State

logger = logging.getLogger(__name__)


# Pydantic models for incoming messages
class _SwitchModeData(BaseModel):
    mode: Literal["AUTOMATIC", "REMOTE_MANUAL"]


class _UpdateOpData(BaseModel):
    percentage: int = Field(ge=0, le=100)


class _SwitchModeMessage(BaseModel):
    type: Literal["switch-mode"]
    data: _SwitchModeData


class _UpdateOpMessage(BaseModel):
    type: Literal["update-op"]
    data: _UpdateOpData


type WebSocketMessage = _SwitchModeMessage | _UpdateOpMessage
adapter = TypeAdapter(WebSocketMessage)


class WebSocketHandler:
    """Handles all communication to and from websockets."""

    def __init__(self, connection_manager: WebSocketConnection, state: State):
        self._connection_manager = connection_manager
        self._state = state

    def handle_message_from_websocket(self, msg: str) -> None:
        """
        Handle an incoming WebSocket message.

        Creates a background task without blocking.

        Args:
            msg: Raw JSON string message from WebSocket client
        """
        asyncio.create_task(self._process_message_from_websocket(msg))

    def send_mode_update_to_websocket(self) -> None:
        """
        Broadcast the current mode to all connected WebSocket clients.

        Creates a background task without blocking.
        """
        self._send_update_to_websocket(("mode", str(self._state.mode)))

    def send_wl_update_to_websocket(self) -> None:
        """
        Broadcast the current water level to all connected WebSocket clients.

        Creates a background task without blocking.
        """
        asyncio.create_task(
            self._connection_manager.broadcast(
                self._create_json_to_websocket(
                    "water_level",
                    ("water_level", str(self._state.water_level)),
                    ("timestamp", datetime.now().isoformat()),
                )
            )
        )

    def send_op_update_to_websocket(self) -> None:
        """
        Broadcast the current opening percentage to all connected WebSocket clients.

        Creates a background task without blocking.
        """
        self._send_update_to_websocket(
            ("opening_percentage", str(self._state.opening_percentage))
        )

    def generate_state_update(self) -> str:
        """
        Generate a JSON representation of the current state.

        Returns:
            JSON string containing the serialized current state
        """
        return self._create_json_to_websocket(
            "init",
            ("mode", self._state.mode),
            ("opening_percentage", str(self._state.opening_percentage)),
            ("water_level", str(self._state.water_level)),
            ("timestamp", datetime.now().isoformat()),
        )

    async def _process_message_from_websocket(self, msg: str) -> None:
        """Parse and route incoming WebSocket messages to appropriate handlers."""
        try:
            parsed_msg = adapter.validate_json(msg)

            match parsed_msg:
                case _SwitchModeMessage(data=data):
                    self._handle_switch_mode(data)
                case _UpdateOpMessage(data=data):
                    self._handle_update_op(data)

        except ValidationError as e:
            logger.error(f"Invalid message from WebSocket: {e}. Message: {msg}")
        except Exception as e:
            logger.error(f"Error processing WebSocket message: {e}. Message: {msg}")

    def _handle_switch_mode(self, data: _SwitchModeData) -> None:
        """Handle mode switching requests."""
        self._state.set_mode(Mode(data.mode))

    def _handle_update_op(self, data: _UpdateOpData) -> None:
        """Handle opening percentage update requests."""
        self._state.set_opening_percentage(data.percentage)

    def _send_update_to_websocket(self, *msgs: tuple[str, str]) -> None:
        """
        Broadcast updates to all connected WebSocket clients.
        Creates a background task without blocking.

        Args:
            *msgs: Variable number of (type, value) tuples to broadcast
        """
        for msg_type, value in msgs:
            asyncio.create_task(
                self._connection_manager.broadcast(
                    self._create_json_to_websocket(msg_type, (msg_type, value))
                )
            )

    def _create_json_to_websocket(self, type: str, *msgs: tuple[str, str]) -> str:
        """
        Create a JSON message for WebSocket communication.

        Args:
            type: Message type
            *msgs: Variable number of (key, value) tuples

        Returns:
            JSON string with type and data fields
        """
        return json.dumps(
            {
                "type": type,
                "data": dict(msgs) if msgs else {},
            }
        )
