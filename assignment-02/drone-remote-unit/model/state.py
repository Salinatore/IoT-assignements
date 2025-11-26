from enum import Enum
from typing import Any, Callable

from pydantic import BaseModel, model_serializer


class DroneState(str, Enum):
    REST = "REST"
    TAKING_OFF = "TAKING_OFF"
    OPERATING = "OPERATING"
    LANDING = "LANDING"


class HangarState(str, Enum):
    NORMAL = "NORMAL"
    ALARM = "ALARM"


class State(BaseModel):
    _drone_state: DroneState = DroneState.REST
    _hangar_state: HangarState = HangarState.NORMAL
    _current_distance: int = 0

    def setMessageHandler(self, on_status_change: Callable[None, None]) -> None:
        self._on_status_change = on_status_change

    @model_serializer
    def ser_model(self) -> dict[str, Any]:
        data = {
            "drone_state": self._drone_state,
            "hangar_state": self._hangar_state,
        }

        if self._drone_state in (DroneState.TAKING_OFF, DroneState.LANDING):
            data["current_distance"] = self._current_distance
        else:
            data["current_distance"] = "ND"

        return data

    def set_drone_state(self, new_drone_state: DroneState):
        self._drone_state = new_drone_state
        self._handle_status_change()

    def set_hangar_state(self, new_hangar_state: HangarState):
        self._hangar_state = new_hangar_state
        self._handle_status_change()

    def _handle_status_change(self):
        if self._on_status_change:
            self._on_status_change()
