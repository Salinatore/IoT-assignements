from connections.websocket import WebSocketManager


class MessageToWebSocketHandler:
    """Handles all communication to websocket"""

    def __init__(self, ws_manager: WebSocketManager):
        self.ws_manager = ws_manager
