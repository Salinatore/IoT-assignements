let ws = null;
const statusDiv = document.getElementById("currentStatus");
const controlBtn = document.getElementById("control-btn");
let currentMode = "NOT AVAILABLE";

function connect() {
  ws = new WebSocket("ws://localhost:8000/ws");

  ws.onopen = () => {
    console.log("Connected to WebSocket");
  };

  ws.onmessage = (event) => {
    const data = JSON.parse(event.data);

    updateMode(data.mode);
    addDataPoint(data.water_level, data.timestamp);
  };

  ws.onclose = () => {
    console.log("Disconnected to WebSocket");
    statusDiv.textContent = "NOT AVAILABLE";
    setTimeout(connect, 30);
  };

  ws.onerror = (error) => {
    console.error("WebSocket error:", error);
  };
}

function updateMode(mode) {
  if (mode === currentMode) return;
  currentMode = mode;

  console.log(mode);

  statusDiv.textContent = currentMode;
  if (currentMode === "REMOTE_MANUAL") {
    controlBtn.disabled = false;
    controlBtn.textContent = "Give back Control";
  } else {
    controlBtn.textContent = "Request Remote Control";
    controlBtn.disabled = currentMode !== "AUTOMATIC";
  }
}

function handleControlBtn() {
  const modeMap = {
    REMOTE_MANUAL: "AUTOMATIC",
    AUTOMATIC: "REMOTE_MANUAL",
  };

  const nextState = modeMap[currentMode];
  if (nextState) {
    ws.send(
      JSON.stringify({
        type: "switch-mode",
        data: { state: nextState },
      }),
    );
  }
}

function disconnectWebSocket() {
  if (ws && ws.readyState === WebSocket.OPEN) {
    ws.close();
    console.log("WebSocket disconnected");
  }
}

window.addEventListener("beforeunload", () => {
  disconnectWebSocket();
});
