from typing import Set

from fastapi import WebSocket, WebSocketDisconnect


class WebSocketManager:
    """Manages WebSocket connections and broadcasting"""

    def __init__(self):
        self._active_connections: Set[WebSocket] = set()

    async def broadcast(self, message: dict):
        disconnected = set()
        for websocket in self._active_connections:
            try:
                await websocket.send_json(message)
            except Exception as e:
                print(f"Error sending to client: {e}")
                disconnected.add(websocket)
        self._active_connections.difference_update(disconnected)

    async def handle_connection(self, websocket: WebSocket):
        """Handle WebSocket connection lifecycle"""
        await self._connect(websocket)

        try:
            while True:
                data = await websocket.receive_text()
                # add input handeling

        except WebSocketDisconnect:
            self._disconnect(websocket)
        except Exception as e:
            print(f"WebSocket error: {e}")
            self._disconnect(websocket)

    async def _connect(self, websocket: WebSocket):
        await websocket.accept()
        self._active_connections.add(websocket)
        print(f"Client connected. Total clients: {len(self._active_connections)}")

    def _disconnect(self, websocket: WebSocket):
        if websocket in self._active_connections:
            self._active_connections.remove(websocket)
