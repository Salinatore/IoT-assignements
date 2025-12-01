import json
from typing import Any, Set

from fastapi import WebSocket, WebSocketDisconnect

from handlers.websocket_message_handler import WebSocketMessageHandler
from model.state import State


class WebSocketManager:
    """Manages WebSocket connections and broadcasting"""

    def __init__(self, websocket_message_handler: WebSocketMessageHandler):
        self.active_connections: Set[WebSocket] = set()
        self.websocket_message_handler = websocket_message_handler

    async def broadcast(self, message: dict):
        disconnected = set()
        for websocket in self.active_connections:
            try:
                await websocket.send_json(message)
            except Exception as e:
                print(f"Error sending to client: {e}")
                disconnected.add(websocket)
        self.active_connections.difference_update(disconnected)

    async def handle_connection(self, websocket: WebSocket, state: State):
        """Handle WebSocket connection lifecycle"""
        await self._connect(websocket)

        initial_message = self._compose_initial_message(state)
        await websocket.send_json(initial_message)

        try:
            while True:
                data = await websocket.receive_text()
                try:
                    message = json.loads(data)
                    await self.websocket_message_handler.handle_message(message)
                except json.JSONDecodeError:
                    print(f"Invalid JSON received: {data}")

        except WebSocketDisconnect:
            self._disconnect(websocket)
        except Exception as e:
            print(f"WebSocket error: {e}")
            self._disconnect(websocket)

    async def _connect(self, websocket: WebSocket):
        await websocket.accept()
        self.active_connections.add(websocket)
        print(f"Client connected. Total clients: {len(self.active_connections)}")

    def _disconnect(self, websocket: WebSocket):
        if websocket in self.active_connections:
            self.active_connections.remove(websocket)

    def _compose_initial_message(self, state: State) -> dict[str, Any]:
        current_state_dict = state.model_dump()
        return {"type": "state", "data": current_state_dict}
