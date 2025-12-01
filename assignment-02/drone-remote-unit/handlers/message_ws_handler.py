from datetime import datetime

from model.state import DroneState, HangarState, State
from websocket.manager import WebSocketManager


class MessageToWebSocketHandler:
    """Handles incoming serial messages"""

    def __init__(self, state: State, ws_manager: WebSocketManager):
        self.state = state
        self.ws_manager = ws_manager

    async def notify_status_changes(self) -> None:
        """Notify clients about state changes"""
        await self.ws_manager.broadcast(
            {"type": "state", "data": self.state.model_dump()}
        )

    async def handle_message(self, msg: str) -> None:
        """Process incoming serial messages and broadcast to clients"""
        if msg.startswith("lo:"):
            await self._handle_log_message(msg)
        else:
            await self._handle_state_message(msg)

    async def _handle_log_message(self, msg: str):
        """Handle log messages (prefixed with 'lo:')"""
        log_content = msg[3:]
        entry = {"timestamp": datetime.now().isoformat(), "message": log_content}
        await self.ws_manager.broadcast({"type": "log", "data": entry})

    async def _handle_state_message(self, msg: str):
        """Handle state update messages"""
        self._state_interpreter(msg)
        entry = {"timestamp": datetime.now().isoformat(), "message": msg}
        await self.ws_manager.broadcast({"type": "msg", "data": entry})

    def _state_interpreter(self, msg: str) -> None:
        match msg:
            case "st-d-fully-out":
                self.state.set_drone_state(DroneState.OPERATING)
            case "st-d-fully-in":
                self.state.set_drone_state(DroneState.REST)
            case "st-a-prealarm":
                self.state.set_hangar_state(HangarState.PREALARM)
            case "st-a-alarm":
                self.state.set_hangar_state(HangarState.ALARM)
            case "st-a-normal":
                self.state.set_hangar_state(HangarState.NORMAL)
