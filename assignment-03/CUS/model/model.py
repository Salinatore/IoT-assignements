from enum import Enum

from pydantic import BaseModel


class Mode(str, Enum):
    AUTOMATIC = "AUTOMATIC"
    REMOTE_MANUAL = "REMOTE_MANUAL"
    LOCAL_MANUAL = "LOCAL_MANUAL"
    UNCONNECTED = "UNCONNECTED"


class State(BaseModel):
    _mode: Mode = Mode.AUTOMATIC
    _water_level: float = 0
    _opening_percentage: int = 0

    def set_mode(self, mode: Mode):
        pass

    def set_level(self, level: float):
        pass

    def set_opening_percentage(self, opening_percentage: int):
        pass
