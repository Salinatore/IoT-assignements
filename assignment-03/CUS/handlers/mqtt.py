import logging

from connections.mqtt import MqttMenager

logger = logging.getLogger(__name__)


class MqttHandler:
    """Handles all communication to serial"""

    def __init__(self, connection_manager: MqttMenager):
        self._connection_manager = connection_manager
