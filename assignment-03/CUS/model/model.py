from datetime import datetime
from enum import Enum
from typing import Any

from pydantic import BaseModel, model_serializer


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

    @model_serializer
    def ser_model(self) -> dict[str, Any]:
        data = {
            "timestamp": datetime.now().isoformat(),
            "mode": self._mode,
            "water_level": self._water_level,
        }
        return data
