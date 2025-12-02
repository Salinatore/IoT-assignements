# Drone Remote Unit

A small FastAPI-based service that bridges WebSocket clients to a serial-connected drone controller.  
It exposes a simple HTTP health endpoint and a WebSocket endpoint for real-time control and status updates.

## What this repo contains
- `main.py` — Application entrypoint: wires together the `State`, serial manager, message handlers, and WebSocket manager and exposes endpoints.
- `config.py` — Configuration (serial `PORT`, `BAUD`, etc.).
- `serial_communication/` — Serial manager and related code.
- `websocket/` — WebSocket connection manager.
- `handlers/` — Handlers that translate between WebSocket messages, state, and serial IO.
- `model/` — Application `State` model.

## Quick set
You can create a virtual enviroment using `uv venv`.
After activating it you can run `uv sync` to install all the dependencies and then `uv run main.py` to run the project.
Make sure that you set the correct PORT and BOUD rate in the confi.py before running.

## Endpoints
- HTTP:
  - `GET /` — Health/status response: `{"message": "Drone Control API", "status": "running"}`
- WebSocket:
  - `ws://<host>:8000/ws` — Connect to send control messages and receive status updates.
