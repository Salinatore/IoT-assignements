let ws = null;
const statusDiv = document.getElementById("currentStatus");
const controlBtn = document.getElementById("control-btn");
let currentMode = null;

function connect() {
  ws = new WebSocket("ws://localhost:8000/ws");

  ws.onopen = () => {
    console.log("Connected to WebSocket");
    statusDiv.textContent = "Connected";
  };

  ws.onmessage = (event) => {
    const data = JSON.parse(event.data);
    console.log(data);

    statusDiv.textContent = mode;
    addDataPoint(data.water_level, data.opening_percentage);
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

  statusDiv.textContent = mode;
  if (mode === "REMOTE_MANUAL") {
    controlBtn.disabled = false;
    controlBtn.textContent = "Give back Control";
  } else {
    controlBtn.textContent = "Request Remote Control";
    controlBtn.disabled = mode !== "AUTOMATIC";
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
  ws.disconnectWebSocket();
}

window.onbeforeunload = () => {
  disconnectWebSocket();
};
