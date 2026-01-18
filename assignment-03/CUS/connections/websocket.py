import logging
from typing import Set

from fastapi import WebSocket, WebSocketDisconnect
from typing_extensions import Callable

logger = logging.getLogger(__name__)


class WebSocketConnection:
    """Manages WebSocket connections and broadcasting"""

    def __init__(self):
        self._active_connections: Set[WebSocket] = set()

    async def start(
        self,
        generate_first_msg: Callable[[], str],
        message_handeler: Callable[[str], None],
    ):
        """
        Set up the WebSocket connection manager.

        Does not start any tasks, those will be created by the FastAPI application.
        """
        self._message_handeler = message_handeler
        self._generate_first_msg = generate_first_msg

    async def broadcast(self, message: str) -> None:
        """Send message to all active websockets"""
        disconnected = set()
        for websocket in self._active_connections:
            try:
                await websocket.send_text(message)
            except Exception as e:
                logger.exception(f"Error sending to client: {e}")
                disconnected.add(websocket)
        self._active_connections.difference_update(disconnected)

    async def manage_new_connection(self, websocket: WebSocket) -> None:
        """Accepts websocket and start to listen for messages continuously"""
        await self._connect(websocket)

        await websocket.send_text(self._generate_first_msg())

        try:
            while True:
                data = await websocket.receive_text()
                self._message_handeler(data)

        except WebSocketDisconnect:
            self._disconnect(websocket)
        except Exception as e:
            logger.exception(f"WebSocket error: {e}")
            self._disconnect(websocket)

    async def _connect(self, websocket: WebSocket) -> None:
        await websocket.accept()
        self._active_connections.add(websocket)

    def _disconnect(self, websocket: WebSocket) -> None:
        if websocket in self._active_connections:
            self._active_connections.remove(websocket)
