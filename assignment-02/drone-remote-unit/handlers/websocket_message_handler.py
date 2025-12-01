import json

from model.state import DroneState, State
from serial.serial_manager import SerialManager


class WebSocketMessageHandler:
    def __init__(self, state: State, serial_manager: SerialManager):
        self.state = state
        self.serial_manager = serial_manager

    async def handle_message(self, msg: str) -> None:
        """Process incoming serial messages and broadcast to clients"""
        message = json.loads(msg)
        msg_type = message.get("type")
        data = message.get("data")

        if msg_type == "command":
            match data:
                case "take-off":
                    await self._await_landing()
                case "await-landing":
                    await self._take_off()

    async def _await_landing(self):
        """Send command to Arduino"""
        if not self.state.is_possible_to_land():
            self.state.set_drone_state(DroneState.LANDING)
            await self.serial_manager.send("landing")

    async def _take_off(self):
        """Send command to Arduino"""
        if not self.state.is_possible_to_take_off():
            self.state.set_drone_state(DroneState.TAKING_OFF)
            await self.serial_manager.send("free-your-wings")
