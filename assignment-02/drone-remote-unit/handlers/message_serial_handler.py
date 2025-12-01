from model.state import DroneState, State
from serial_communication.serial_manager import SerialManager


class MessageToSerialHandler:
    """Handles all communication to serial"""

    def __init__(self, state: State, serial_manager: SerialManager):
        self.state = state
        self.serial_manager = serial_manager

    async def handle_ws_message(self, msg: dict) -> None:
        """Handles all communication from the websocket"""
        msg_type = msg.get("type")
        data = msg.get("data")

        if msg_type == "command":
            match data:
                case "take-off":
                    await self._take_off()
                case "await-landing":
                    await self._await_landing()

    async def _await_landing(self):
        """Send command to Arduino"""
        if self.state.is_possible_to_land():
            self.state.set_drone_state(DroneState.LANDING)
            await self.serial_manager.send("landing")

    async def _take_off(self):
        """Send command to Arduino"""
        if self.state.is_possible_to_take_off():
            self.state.set_drone_state(DroneState.TAKING_OFF)
            await self.serial_manager.send("free-your-wings")
