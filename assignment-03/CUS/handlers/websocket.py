import logging

from connections.websocket import WebSocketConnection

logger = logging.getLogger(__name__)


class WebSocketHandler:
    """Handles all communication to websocket"""

    def __init__(self, connection_manager: WebSocketConnection):
        self._connection_manager = connection_manager

    def handle_message_from_websocket(self, msg: str):
        pass
