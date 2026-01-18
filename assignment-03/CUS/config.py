from dataclasses import dataclass

@dataclass(frozen=True)
class Config:
    # Development
    in_development: bool = True

    # Serial
    serial_port: str = "/dev/ttyACM0"
    baud: int = 115200

    # MQTT
    broker: str = "broker.mqtt-dashboard.com"
    topic: str = "esiot-2025-fgor"

    # Uvicorn
    host: str = "127.0.0.1"
    port: int = 8000
