from contextlib import asynccontextmanager

import uvicorn
from fastapi import FastAPI, WebSocket
from fastapi.middleware.cors import CORSMiddleware

from config import BAUD, PORT
from handlers.message_serial_handler import MessageToSerialHandler
from handlers.message_ws_handler import MessageToWebSocketHandler
from model.state import State
from serial_communication.serial_manager import SerialManager
from websocket.manager import WebSocketManager

state = State()

serial_manager = SerialManager(
    port=PORT,
    baud=BAUD,
)

message_to_serial = MessageToSerialHandler(state, serial_manager)

web_socket_handler = WebSocketManager(message_to_serial)

message_to_web_socket = MessageToWebSocketHandler(state, web_socket_handler)
state.setMessageHandler(message_to_web_socket.notify_status_changes)


@asynccontextmanager
async def lifespan(app: FastAPI):
    """Manage application lifecycle"""
    try:
        print("Starting serial connection...")
        await serial_manager.start(message_to_web_socket.handle_message)
        print("Serial connection started")
        yield
    except Exception as e:
        print(f"Error during startup: {e}")
        raise
    finally:
        print("Stopping serial connection...")
        await serial_manager.stop()
        print("Serial connection stopped")


app = FastAPI(lifespan=lifespan)


app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.get("/")
async def root() -> dict:
    return {"message": "Drone Control API", "status": "running"}


@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    """WebSocket endpoint for real-time updates"""
    await web_socket_handler.handle_connection(websocket, state)


if __name__ == "__main__":
    uvicorn.run(
        "main:app",
        host="127.0.0.1",
        port=8000,
        reload=True,
    )
