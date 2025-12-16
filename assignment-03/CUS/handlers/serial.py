import asyncio
import logging

from connections.serial import SerialManager

logger = logging.getLogger(__name__)

CUS_TO_WCS_STATUS_AUTOMATIC = "cus->wcs-st-automatic"
CUS_TO_WCS_STATUS_REMOTE_MANUAL = "cus->wcs-st-remote-manual"
CUS_TO_WCS_STATUS_UNCONNECTED = "cus->wcs-st-unconnected"
CUS_TO_WCS_OPENING_PERCENTEGE_PREFIX = "cus->wcs-op-"
WCS_TO_CUS_STATUS_AUTOMATIC = "wcs->cus-st-automatic"
WCS_TO_CUS_STATUS_LOCAL_MANUAL = "wcs->cus-st-local-manual"
WCS_TO_CUS_OPENING_PERCENTEGE_PREFIX = "wcs->cus-op-"


class SerialHandler:
    """Handles all communication to serial"""

    def __init__(self, connection_manager: SerialManager):
        self._connection_manager = connection_manager

    def handle_message(self, msg):
        asyncio.create_task(self._process_message_async())

    async def _process_message_async(self):
        pass  # actual handeling
