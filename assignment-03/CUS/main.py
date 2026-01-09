import asyncio
import logging
from contextlib import asynccontextmanager

import uvicorn
from fastapi import FastAPI, WebSocket

from config import Config
from connections.mqtt import MqttConnection
from connections.serial import SerialConnection
from connections.websocket import WebSocketConnection
from handlers.mqtt import MqttHandler
from handlers.serial import SerialHandler
from handlers.websocket import WebSocketHandler
from logger_config import setup_logging
from model.model import state
from monitoring.water_level_monitor import WaterLevelMonitor
from routers.test import router

setup_logging()
logger: logging.Logger = logging.getLogger(__name__)
config: Config = Config()

mqtt_connection: MqttConnection = MqttConnection(broker=config.broker, topic=config.topic)
serial_connection: SerialConnection = SerialConnection(port=config.serial_port, baud=config.baud)
websocket_connection: WebSocketConnection = WebSocketConnection()

mqtt_handler: MqttHandler = MqttHandler(mqtt_connection, state)
serial_handler: SerialHandler = SerialHandler(serial_connection, state)
websocket_handler: WebSocketHandler = WebSocketHandler(websocket_connection, state)

water_level_monitor: WaterLevelMonitor = WaterLevelMonitor(state)


def notify_listeners():
    websocket_handler.send_state_update_to_websocket()
    serial_handler.send_state_update_to_serial()


state.set_listeners(notify_listeners)


@asynccontextmanager
async def lifespan(app: FastAPI):
    """Manage application lifecycle"""
    if config.in_development:
        logger.warning(
            "Testing mode enabled, please put TESTING False in config file if you are not testing"
        )

    startup_tasks = [
        serial_connection.start(
            message_handler=serial_handler.handle_message_from_serial
        ),
        mqtt_connection.start(message_handler=mqtt_handler.handle_message_from_mqtt),
        websocket_connection.start(
            message_handeler=websocket_handler.handle_message_from_websocket,
            generate_first_msg=websocket_handler.generate_state_update,
        ),
    ]

    await asyncio.gather(*startup_tasks)

    # Start monitor
    water_level_monitor.start()

    yield


app = FastAPI(lifespan=lifespan)
if config.in_development:
    app.include_router(router)


@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket) -> None:
    """
    WebSocket endpoint for real-time bidirectional communication.

    Args:
        websocket: The WebSocket connection to manage
    """
    await websocket_connection.manage_new_connection(websocket)


if __name__ == "__main__":
    uvicorn.run(
        "main:app",
        host=config.host,
        port=config.port,
        reload=config.in_development, # set auto reload only on development
    )
