import asyncio
import logging
import time

from model.model import State

logger = logging.getLogger(__name__)


class WaterLevelMonitor:
    POLLING_RATE: int = 1

    def __init__(self, state: State) -> None:
        self._state: State = state

    def start(self) -> None:
        asyncio.create_task(self._monitor())

    async def _monitor(self):
        while True:
            await asyncio.sleep(self.POLLING_RATE)
            current_time = time.time()
            self._state.check_unconnected(current_time)
            self._state.check_water_level(current_time)
