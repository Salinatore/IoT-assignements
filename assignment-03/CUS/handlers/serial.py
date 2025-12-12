from serial_communication.serial_manager import SerialManager


class MessageToSerialHandler:
    """Handles all communication to serial"""

    def __init__(self, serial_manager: SerialManager):
        self.serial_manager = serial_manager
