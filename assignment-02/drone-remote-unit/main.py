from contextlib import asynccontextmanager

import uvicorn
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

from config import BAUD, PORT
from handlers.serial_message_handler import SerialMessageHandler
from handlers.websocket_message_handler import WebSocketMessageHandler
from model.state import State
from serial.serial_manager import SerialManager
from websocket.manager import WebSocketManager

state = State()

serial_manager = SerialManager(
    port=PORT,
    baud=BAUD,
)

ws_msg_handler = WebSocketMessageHandler(state, serial_manager)

ws_manager = WebSocketManager(ws_msg_handler)

serial_msg_handler = SerialMessageHandler(state, ws_manager)


@asynccontextmanager
async def lifespan(app: FastAPI):
    """Manage application lifecycle"""
    try:
        print("Starting serial connection...")
        await serial_manager.start(serial_msg_handler.handle_message)
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
async def root():
    return {"message": "Drone Control API", "status": "running"}


@app.websocket("/ws")
async def websocket_endpoint(websocket):
    """WebSocket endpoint for real-time updates"""
    await ws_manager.handle_connection(websocket, state)


if __name__ == "__main__":
    uvicorn.run("main:app", host="0.0.0.0", port=8000, reload=True)
