let ws = null;
const logsContainer = document.getElementById("logsContainer");
const statusDiv = document.getElementById("status");
const statusDrone = document.getElementById("dronestatus");
const statusHangar = document.getElementById("hangarstatus");
const current_distance = document.getElementById("distance");
const commandInput = document.getElementById("commandInput");
const sendBtn = document.getElementById("sendBtn");

function connect() {
  ws = new WebSocket("ws://localhost:8000/ws");

  ws.onopen = () => {
    console.log("Connected to WebSocket");
    statusDiv.textContent = "✓ Connected";
    statusDiv.className = "status connected";
    commandInput.disabled = false;
    sendBtn.disabled = false;
  };

  ws.onmessage = (event) => {
    const data = JSON.parse(event.data);
    console.log(data);
    if (data.type === "log") {
      addLogEntry(data.data);
    }
    if (data.type === "state") {
      refreshStatus(data.data);
    }
  };

  ws.onclose = () => {
    console.log("Disconnected from WebSocket");
    statusDiv.textContent = "✗ Disconnected";
    statusDiv.className = "status disconnected";
    commandInput.disabled = true;
    sendBtn.disabled = true;
    setTimeout(connect, 3000);
  };

  ws.onerror = (error) => {
    console.error("WebSocket error:", error);
  };
}

function addLogEntry(log) {
  const emptyState = logsContainer.querySelector("empty-state");
  if (emptyState) {
    emptyState.remove();
  }

  const logEntry = document.createElement("div");
  logEntry.className = "log-entry";

  const timestamp = new Date(log.timestamp).toLocaleTimeString();

  logEntry.innerHTML = `
          <div class="log-timestamp">${timestamp}</div>
          <div class="log-message">${escapeHtml(log.message)}</div>
        `;

  logsContainer.insertBefore(logEntry, logsContainer.firstChild);

  while (logsContainer.children.length > 50) {
    logsContainer.removeChild(logsContainer.lastChild);
  }
}

async function sendCommand() {
  const command = commandInput.value.trim();
  if (!command) return;

  try {
    const response = await fetch(
      `http://localhost:8000/send?msg=${encodeURIComponent(command)}`,
    );
    const data = await response.json();

    if (data.status === "success") {
      commandInput.value = "";
      console.log("Command sent successfully");
    }
  } catch (error) {
    console.error("Error sending command:", error);
    alert("Failed to send command");
  }
}

function clearLogs() {
  logsContainer.innerHTML =
    '<div class="empty-state">Logs cleared. Waiting for new messages...</div>';
}

function escapeHtml(text) {
  const div = document.createElement("div");
  div.textContent = text;
  return div.innerHTML;
}

commandInput.addEventListener("keypress", (e) => {
  if (e.key === "Enter") {
    sendCommand();
  }
});

function refreshStatus(data) {
  const json_data = JSON.parse(data);
  statusDrone.textContent = json_data.drone_state;
  statusHangar.textContent = json_data.hangar_state;
  current_distance.textContent = json_data.current_distance;
}

async function initializeStatus() {
  try {
    const response = await fetch(`http://localhost:8000/state`);
    const data = await response.json();
    statusDrone.textContent = data.drone_state;
    statusHangar.textContent = data.hangar_state;
    current_distance.textContent = data.current_distance;
  } catch (error) {
    console.error("Error sending command:", error);
    alert("Failed to send command");
  }
}

initializeStatus();

connect();
