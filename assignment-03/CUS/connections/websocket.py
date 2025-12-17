import logging
from typing import Set

from fastapi import WebSocket, WebSocketDisconnect
from typing_extensions import Callable

logger = logging.getLogger(__name__)


class WebSocketManager:
    """Manages WebSocket connections and broadcasting"""

    def __init__(self):
        self._active_connections: Set[WebSocket] = set()

    async def start(self, message_handeler: Callable[[str], None]):
        self._message_handeler = message_handeler

    async def broadcast(self, message: dict):
        """Send message to all active websockets"""
        disconnected = set()
        for websocket in self._active_connections:
            try:
                await websocket.send_json(message)
            except Exception as e:
                logger.exception(f"Error sending to client: {e}")
                disconnected.add(websocket)
        self._active_connections.difference_update(disconnected)

    async def manage_new_connection(self, websocket: WebSocket):
        """Accepts websocket and start to listen for messages continuously"""
        await self._connect(websocket)

        try:
            while True:
                data = await websocket.receive_text()
                # add input handeling with task create

        except WebSocketDisconnect:
            self._disconnect(websocket)
        except Exception as e:
            logger.exception(f"WebSocket error: {e}")
            self._disconnect(websocket)

    async def _connect(self, websocket: WebSocket):
        await websocket.accept()
        self._active_connections.add(websocket)
        logger.info(f"Client connected. Total clients: {len(self._active_connections)}")

    def _disconnect(self, websocket: WebSocket):
        if websocket in self._active_connections:
            self._active_connections.remove(websocket)
