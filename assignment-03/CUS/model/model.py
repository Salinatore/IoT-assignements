import logging
from datetime import datetime
from enum import StrEnum
from typing import Any, Callable

from pydantic import BaseModel, model_serializer

logger = logging.getLogger(__name__)


class Mode(StrEnum):
    AUTOMATIC = "AUTOMATIC"
    REMOTE_MANUAL = "REMOTE_MANUAL"
    LOCAL_MANUAL = "LOCAL_MANUAL"
    UNCONNECTED = "UNCONNECTED"


# unconneted
T2: float = 1

# water level
T1: float = 10
L1: float = 10
L2: float = 20


class State(BaseModel):
    _mode: Mode = Mode.AUTOMATIC
    _water_level: float = 0
    _opening_percentage: int = 0
    _time_since_last_wl_update: None | float = None

    def get_mode(self) -> Mode:
        return self._mode

    def get_opening_percentage(self) -> int:
        return self._opening_percentage

    def set_listeners(self, on_status_change: Callable[[], None]) -> None:
        """Sets the callback function to be called on status changes"""
        self._on_status_change = on_status_change

    def check_unconnected(self, current_time: float) -> None:
        if not self._time_since_last_wl_update:
            self._time_since_last_wl_update = current_time

        if (
            self._mode != Mode.UNCONNECTED
            and current_time - self._time_since_last_wl_update > T2
        ):
            self.set_mode(Mode.UNCONNECTED)
            self._notify_listeners()
            logger.info("State is now in Unconnected mode")

    def set_mode(self, mode: Mode):
        if (mode == Mode.LOCAL_MANUAL and self._mode == Mode.REMOTE_MANUAL) or (
            mode == Mode.REMOTE_MANUAL and self._mode == Mode.LOCAL_MANUAL
        ):
            logger.error(
                f"Unexpected mode change. Requested mode: [{mode}] Current mode [{self._mode}]"
            )
            return

        self._mode = mode

        self._notify_listeners()

    def set_level(self, level: float, current_time: float):
        if level < 0:
            logger.error(f"Unexpected water level. Water level: [{level}]")
            return

        self._water_level = level

        self._time_since_last_wl_update = current_time
        if self._mode == Mode.AUTOMATIC:
            self._update_opening_presentage(current_time)

        if self._mode == Mode.UNCONNECTED:
            self.set_mode(Mode.AUTOMATIC)
            logger.info("State is now in Automatic mode")

        self._notify_listeners()

    def set_opening_percentage(self, opening_percentage: int):
        if not self._mode == Mode.LOCAL_MANUAL or self._mode == Mode.REMOTE_MANUAL:
            logger.error(
                f"Cannot change the opening percentage due to the current mode. Current mode: [{self._mode}]"
            )
            return

        self._opening_percentage = opening_percentage

        self._notify_listeners()

    def _update_opening_presentage(self, current_time: float) -> None:
        if self._water_level > L2:
            self._opening_percentage = 100
            return

        if self._water_level > L1:
            if not self._time_since_L1:
                self._time_since_L1 = current_time
            else:
                if current_time - self._time_since_L1 > T1:
                    self._opening_percentage = 50
        else:
            self._opening_percentage = 0
            self._time_since_L1 = None

    def _notify_listeners(self):
        if self._on_status_change:
            self._on_status_change()

    @model_serializer
    def ser_model(self) -> dict[str, Any]:
        data = {
            "timestamp": datetime.now().isoformat(),
            "water_level": self._water_level,
            "mode": str(self._mode),
            "opening_percentage": self._opening_percentage,
        }
        return data


state = State()
