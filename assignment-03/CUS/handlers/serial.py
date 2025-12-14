import logging

from connections.serial import SerialManager

logger = logging.getLogger(__name__)


class SerialHandler:
    """Handles all communication to serial"""

    def __init__(self, connection_manager: SerialManager):
        self._connection_manager = connection_manager
