from serial_communication.serial_manager import SerialManager


class SerialHandler:
    """Handles all communication to serial"""

    def __init__(self, connection_manager: SerialManager):
        self._connection_manager = connection_manager
