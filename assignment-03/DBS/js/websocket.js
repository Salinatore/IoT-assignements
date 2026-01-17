const controlBtn = document.getElementById("control-btn");

const slider = document.getElementsByClassName("slider")[0];
const inputSlider = document.getElementById("slider");

const waterLevelTag = document.getElementById("waterLevel");
const openingPercentageTag = document.getElementById("openingPercentage");
const connectionStatusTag = document.getElementById("displayConnection");

let currentMode = "NOT AVAILABLE";
let ws = null;

function connect() {
  ws = new WebSocket("ws://localhost:8000/ws");

  ws.onopen = () => {
    console.log("Connected to WebSocket");
    connectionStatusTag.textContent = "NOT AVAILABLE";
  };

  ws.onmessage = (event) => {
    const data = JSON.parse(event.data);

    updateMode(data.mode);
    updateOP(data.opening_percentage);
    updateWaterLevel(data.water_level);

    addDataPoint(data.water_level, data.timestamp);
  };

  ws.onclose = () => {
    console.log("Disconnected to WebSocket");
    connectionStatusTag.textContent = "NOT AVAILABLE";
  };

  ws.onerror = (error) => {
    console.error("WebSocket error:", error);
  };
}

function updateMode(mode) {
  if (mode === currentMode) return;
  currentMode = mode;

  connectionStatusTag.textContent = currentMode;
  if (currentMode === "REMOTE_MANUAL") {
    slider.hidden = false;
    controlBtn.disabled = false;
    controlBtn.textContent = "Give back Control";
  } else {
    slider.hidden = true;
    controlBtn.textContent = "Request Remote Control";
    controlBtn.disabled = currentMode !== "AUTOMATIC";
  }
}

function updateOP(openingPercentage) {
  openingPercentageTag.textContent = openingPercentage + " %";
}

function updateWaterLevel(waterLevel) {
  waterLevelTag.textContent = waterLevel + " cm";
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
        data: { mode: nextState },
      }),
    );
  }
}

function sendOpeningPercentage() {
  ws.send(
      JSON.stringify({
        type: "update-op",
        data: { percentage: inputSlider.value },
      }),
    );
}
