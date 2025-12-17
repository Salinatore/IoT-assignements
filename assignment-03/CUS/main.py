"""Main application file for the Drone Control API using FastAPI framework."""

import asyncio
import logging
from contextlib import asynccontextmanager

import uvicorn
from fastapi import FastAPI, WebSocket

from config import BAUD, BROKER, PORT, TOPIC
from connections.mqtt import MqttConnection
from connections.serial import SerialConnection
from connections.websocket import WebSocketConnection
from handlers.mqtt import MqttHandler
from handlers.serial import SerialHandler
from handlers.websocket import WebSocketHandler
from logger_config import setup_logging
from model.model import State

setup_logging()
logger = logging.getLogger(__name__)

state = State()

mqtt_connection = MqttConnection(broker=BROKER, topic=TOPIC)
serial_connection = SerialConnection(port=PORT, baud=BAUD)
websocket_connection = WebSocketConnection()

mqtt_handler = MqttHandler(mqtt_connection, state)
serial_handler = SerialHandler(serial_connection, state)
websocket_handler = WebSocketHandler(websocket_connection)


@asynccontextmanager
async def lifespan(app: FastAPI):
    """Manage application lifecycle"""
    await asyncio.gather(
        serial_connection.start(
            message_handler=serial_handler.handle_message_from_serial
        ),
        mqtt_connection.start(message_handler=mqtt_handler.handle_message_from_mqtt),
        websocket_connection.start(
            message_handeler=websocket_handler.handle_message_from_websocket
        ),
    )
    yield


app = FastAPI(lifespan=lifespan)


@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    """WebSocket endpoint for establishing a new connection"""
    await websocket_connection.manage_new_connection(websocket)


if __name__ == "__main__":
    uvicorn.run(
        "main:app",
        host="127.0.0.1",
        port=8000,
        reload=True,
    )
