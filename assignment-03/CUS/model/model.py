import logging
import math
from enum import StrEnum
from typing import Callable

from pydantic import BaseModel

logger = logging.getLogger(__name__)


class Mode(StrEnum):
    AUTOMATIC = "AUTOMATIC"
    REMOTE_MANUAL = "REMOTE_MANUAL"
    LOCAL_MANUAL = "LOCAL_MANUAL"
    UNCONNECTED = "UNCONNECTED"


# unconneted
T2: float = 10

# water level
T1: float = 3
L1: float = 2
L2: float = 3

CONTAINER_HEIGHT: float = 4


class State(BaseModel):
    _mode: Mode = Mode.AUTOMATIC
    _water_level: float = 0
    _opening_percentage: int = 0
    _time_since_last_wl_update: None | float = None

    @property
    def mode(self) -> Mode:
        return self._mode

    @property
    def opening_percentage(self) -> int:
        return self._opening_percentage

    @property
    def water_level(self) -> float:
        return self._water_level

    def set_listeners(
        self,
        on_status_change: Callable[[], None],
        on_opening_percetage_change: Callable[[], None],
        on_water_level_change: Callable[[], None],
        on_mode_change: Callable[[], None],
    ) -> None:
        """Sets the callback function to be called on status changes"""
        self._on_status_change = on_status_change
        self._on_opening_percetage_change = on_opening_percetage_change
        self._on_water_level_change = on_water_level_change
        self._on_mode_change = on_mode_change

    def check_unconnected(self, current_time: float) -> None:
        if not self._time_since_last_wl_update:
            self._time_since_last_wl_update = current_time

        if (
            self._mode != Mode.UNCONNECTED
            and current_time - self._time_since_last_wl_update > T2
        ):
            self.set_mode(Mode.UNCONNECTED)

    def set_mode(self, mode: Mode) -> None:
        if (mode == Mode.LOCAL_MANUAL and self._mode == Mode.REMOTE_MANUAL) or (
            mode == Mode.REMOTE_MANUAL and self._mode == Mode.LOCAL_MANUAL
        ):
            logger.error(
                f"Unexpected mode change. Requested mode: [{mode}] Current mode [{self._mode}]"
            )
            return

        logger.debug(f"Mode change. From [{self._mode}] to [{mode}]")
        self._mode = mode

        self._on_status_change()
        self._on_mode_change()

    def set_level(self, level: float, current_time: float) -> None:
        self._time_since_last_wl_update = current_time
        if self._mode == Mode.UNCONNECTED:
            self.set_mode(Mode.AUTOMATIC)

        if level < 0:
            logger.error(f"Unexpected water level. Water level: [{level}]")
            return

        x: float = CONTAINER_HEIGHT - level
        self._water_level = math.trunc(x * 1000) / 1000
        logger.debug(f"Water level change. From [{self._water_level}] to [{level}]")

        if self._mode == Mode.AUTOMATIC:
            self._update_opening_persentage(current_time)

        self._on_status_change()
        self._on_water_level_change()

    def set_opening_percentage(self, opening_percentage: int) -> None:
        if not (self._mode == Mode.LOCAL_MANUAL or self._mode == Mode.REMOTE_MANUAL):
            logger.error(
                f"Cannot change the opening percentage due to the current mode. Current mode: [{self._mode}]"
            )
            return
        self._change_opening_persentage(opening_percentage)

    def _update_opening_persentage(self, current_time: float) -> None:
        if self._water_level > L2:
            self._change_opening_persentage(100)
            return

        if self._water_level > L1:
            if not self._time_since_L1:
                self._time_since_L1 = current_time
            else:
                if current_time - self._time_since_L1 > T1:
                    self._change_opening_persentage(50)
        else:
            self._change_opening_persentage(0)
            self._time_since_L1 = None

    def _change_opening_persentage(self, opening_percentage: int) -> None:
        logger.debug(
            f"Opening percentage set from [{self._opening_percentage}] to [{opening_percentage}]"
        )
        self._opening_percentage = opening_percentage

        self._on_status_change()
        self._on_opening_percetage_change()


state = State()
