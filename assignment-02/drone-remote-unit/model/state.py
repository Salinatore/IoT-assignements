import asyncio
from collections.abc import Coroutine
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
    PREALARM = "PREALARM"
    ALARM = "ALARM"


class State(BaseModel):
    _drone_state: DroneState = DroneState.REST
    _hangar_state: HangarState = HangarState.NORMAL
    _current_distance: int = 0

    def setMessageHandler(
        self, on_status_change: Callable[[], Coroutine[Any, Any, None]]
    ) -> None:
        self._on_status_change = on_status_change

    def is_possible_to_land(self) -> bool:
        return (
            self._drone_state == DroneState.OPERATING
            and self._hangar_state == HangarState.NORMAL
        )

    def is_possible_to_take_off(self) -> bool:
        return (
            self._drone_state == DroneState.REST
            and self._hangar_state == HangarState.NORMAL
        )

    def set_drone_state(self, new_drone_state: DroneState) -> None:
        self._drone_state = new_drone_state
        self._handle_status_change()

    def set_hangar_state(self, new_hangar_state: HangarState) -> None:
        self._hangar_state = new_hangar_state
        self._handle_status_change()

    def _handle_status_change(self):
        if self._on_status_change:
            asyncio.create_task(self._on_status_change())

    @model_serializer
    def ser_model(self) -> dict[str, Any]:
        if (
            self._hangar_state == HangarState.ALARM
            and self._drone_state == DroneState.OPERATING
        ):
            hangar_state = self._hangar_state
        else:
            hangar_state = "ND"

        if self._drone_state == DroneState.LANDING:
            current_distance = self._current_distance
        else:
            current_distance = "ND"

        data = {
            "drone_state": self._drone_state,
            "hangar_state": hangar_state,
            "current_distance": current_distance,
            "can_land": self.is_possible_to_land(),
            "can_take_off": self.is_possible_to_take_off(),
        }

        return data
