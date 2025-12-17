import logging
from datetime import datetime
from enum import Enum
from typing import Any, Callable

from pydantic import BaseModel, model_serializer

logger = logging.getLogger(__name__)


class Mode(str, Enum):
    AUTOMATIC = "AUTOMATIC"
    REMOTE_MANUAL = "REMOTE_MANUAL"
    LOCAL_MANUAL = "LOCAL_MANUAL"
    UNCONNECTED = "UNCONNECTED"


class State(BaseModel):
    _mode: Mode = Mode.AUTOMATIC
    _water_level: float = 0
    _opening_percentage: int = 0

    def get_mode(self) -> Mode:
        return self._mode

    def setListeners(self, on_status_change: Callable[[], None]) -> None:
        """Sets the callback function to be called on status changes"""
        self._on_status_change = on_status_change

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

    def set_level(self, level: float):
        if level < 0:
            logger.error(f"Unexpected water level. Water level: [{level}]")
            return

        self._water_level = level
        if self._mode == Mode.AUTOMATIC:
            self._compute_new_opening_percentage()

        self._notify_listeners()

    def set_opening_percentage(self, opening_percentage: int):
        if not self._mode == Mode.LOCAL_MANUAL or self._mode == Mode.REMOTE_MANUAL:
            logger.error(
                f"Cannot change the opening percentage due to the current mode. Current mode: [{self._mode}]"
            )
            return

        self._opening_percentage = opening_percentage

        self._notify_listeners()

    def _notify_listeners(self):
        if self._on_status_change:
            self._on_status_change()

    def _compute_new_opening_percentage(self):
        pass

    @model_serializer
    def ser_model(self) -> dict[str, Any]:
        data = {
            "timestamp": datetime.now().isoformat(),
            "water_level": self._water_level,
            "mode": self._mode,
            "opening_percentage": self._opening_percentage,
        }
        return data
