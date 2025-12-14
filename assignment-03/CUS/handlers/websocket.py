import logging

from connections.websocket import WebSocketManager

logger = logging.getLogger(__name__)


class WebSocketHandler:
    """Handles all communication to websocket"""

    def __init__(self, connection_manager: WebSocketManager):
        self._connection_manager = connection_manager
