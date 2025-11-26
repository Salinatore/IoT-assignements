import asyncio
from contextlib import asynccontextmanager
from datetime import datetime
from typing import Set

from fastapi import FastAPI, HTTPException, WebSocket, WebSocketDisconnect
from fastapi.middleware.cors import CORSMiddleware

from controllers.serial_manager import SerialManager
from model.state import DroneState, State

state = State()

active_websockets: Set[WebSocket] = set()


async def broadcast_to_clients(message: dict):
    """Send message to all connected WebSocket clients"""
    disconnected = set()

    for websocket in active_websockets:
        try:
            await websocket.send_json(message)
        except Exception as e:
            print(f"Error sending to client: {e}")
            disconnected.add(websocket)

    active_websockets.difference_update(disconnected)


def handle_state_change():
    try:
        type = "state"
        current_state = state.model_dump_json()
        asyncio.create_task(broadcast_to_clients({"type": type, "data": current_state}))
    except Exception as e:
        print(f"Error broadcasting message: {e}")


state.setMessageHandler(on_status_change=handle_state_change)


def handle_serial_message(msg: str):
    """Process incoming serial messages and broadcast to clients"""
    if msg.startswith("lo:"):
        log_content = msg[3:]
        type = "log"
        entry = {"timestamp": datetime.now().isoformat(), "message": log_content}
    else:
        type = "msg"
        entry = {"timestamp": datetime.now().isoformat(), "message": msg}

    try:
        asyncio.create_task(broadcast_to_clients({"type": type, "data": entry}))
    except Exception as e:
        print(f"Error broadcasting message: {e}")


serial_manager = SerialManager(
    port="/dev/tty.usbmodem1101",
    baud=115200,
    on_message=handle_serial_message,
)


@asynccontextmanager
async def lifespan(app: FastAPI):
    """Manage application lifecycle"""
    try:
        print("Starting serial connection...")
        await serial_manager.start()
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


@app.get("/state")
async def get_state() -> State:
    """Get current drone/hangar state"""
    return state


@app.get("/changestate")
async def change_state() -> State:
    """Get current drone/hangar state"""
    state.set_drone_state(DroneState.OPERATING)
    return state


@app.get("/send")
async def send(msg: str):
    """Send command to Arduino"""
    if not msg:
        raise HTTPException(status_code=400, detail="Message cannot be empty")

    try:
        await serial_manager.send(msg)
        return {"status": "success", "message": f"Sent: {msg}"}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    """WebSocket endpoint for real-time log updates"""
    await websocket.accept()
    active_websockets.add(websocket)

    print(f"Client connected. Total clients: {len(active_websockets)}")

    try:
        while True:
            data = await websocket.receive_text()
            print(f"Received from client: {data}")

    except WebSocketDisconnect:
        print("Client disconnected")
        active_websockets.remove(websocket)
    except Exception as e:
        print(f"WebSocket error: {e}")
        if websocket in active_websockets:
            active_websockets.remove(websocket)
