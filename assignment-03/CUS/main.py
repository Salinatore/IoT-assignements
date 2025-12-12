"""Main application file for the Drone Control API using FastAPI framework."""

from contextlib import asynccontextmanager

import uvicorn
from fastapi import FastAPI, WebSocket
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse
from fastapi.staticfiles import StaticFiles

from config import BAUD, PORT
from connections.websocket import WebSocketManager
from handlers.serial import MessageToSerialHandler
from handlers.websocket import MessageToWebSocketHandler

serial_manager = SerialManager(
    port=PORT,
    baud=BAUD,
)

message_to_serial = MessageToSerialHandler(serial_manager)

web_socket_handler = WebSocketManager(message_to_serial)

message_to_web_socket = MessageToWebSocketHandler(web_socket_handler)


@asynccontextmanager
async def lifespan(app: FastAPI):
    """Manage application lifecycle"""
    await serial_manager.start(message_to_web_socket.handle_serial_message)
    yield


app = FastAPI(lifespan=lifespan)
app.mount("/static", StaticFiles(directory="static"), name="static")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.get("/", response_class=FileResponse)
async def get_frontend():
    return FileResponse("static/index.html")


@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    """WebSocket endpoint for establishing a new connection"""
    await web_socket_handler.handle_connection(websocket, state)


if __name__ == "__main__":
    uvicorn.run(
        "main:app",
        host="127.0.0.1",
        port=8000,
        reload=True,
    )
