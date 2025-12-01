let ws = null;
const logsContainer = document.getElementById("logsContainer");
const msgsContainer = document.getElementById("msgsContainer");
const statusDiv = document.getElementById("status");
const statusDrone = document.getElementById("dronestatus");
const statusHangar = document.getElementById("hangarstatus");
const current_distance = document.getElementById("distance");
const takaOffBtn = document.getElementById("takeoffBtn");
const landingBtn = document.getElementById("landingBtn");

function connect() {
  ws = new WebSocket("ws://localhost:8000/ws");

  ws.onopen = () => {
    console.log("Connected to WebSocket");
    statusDiv.textContent = "✓ Connected";
    statusDiv.className = "status connected";
  };

  ws.onmessage = (event) => {
    const data = JSON.parse(event.data);
    console.log(data);

    switch (data.type) {
      case "log":
        addLogEntry(data.data);
        break;
      case "state":
        refreshStatus(data.data);
        break;
      case "msg":
        addMsgEntry(data.data);
        break;
      default:
        console.warn(`Unknown message type: ${data.type}`);
    }
  };

  ws.onclose = () => {
    console.log("Disconnected from WebSocket");
    statusDiv.textContent = "✗ Disconnected";
    statusDiv.className = "status disconnected";
    setTimeout(connect, 3000);
  };

  ws.onerror = (error) => {
    console.error("WebSocket error:", error);
  };
}

function addLogEntry(log) {
  const emptyState = logsContainer.querySelector(".empty-state");
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

function addMsgEntry(msg) {
  const emptyState = msgsContainer.querySelector(".empty-state");
  if (emptyState) {
    emptyState.remove();
  }

  const msgEntry = document.createElement("div");
  msgEntry.className = "msg-entry";

  const timestamp = new Date(msg.timestamp).toLocaleTimeString();

  msgEntry.innerHTML = `
          <div class="msg-timestamp">${timestamp}</div>
          <div class="msg-message">${escapeHtml(msg.message)}</div>
        `;

  msgsContainer.insertBefore(msgEntry, msgsContainer.firstChild);

  while (msgsContainer.children.length > 50) {
    msgsContainer.removeChild(msgsContainer.lastChild);
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

landingBtn.addEventListener("click", async () => {
  try {
    await ws.send(JSON.stringify({ type: "command", data: "await-landing" }));
  } catch (error) {
    console.error("Error sending command:", error);
    alert("Failed to send command");
  }
});

takaOffBtn.addEventListener("click", async () => {
  try {
    await ws.send(JSON.stringify({ type: "command", data: "take-off" }));
  } catch (error) {
    console.error("Error sending command:", error);
    alert("Failed to send command");
  }
});

function refreshStatus(data) {
  statusDrone.textContent = data.drone_state;
  if (data.hangar_state !== "ND") {
    statusHangar.textContent = data.hangar_state;
    statusHangar.style.display = "block";
  } else {
    statusHangar.style.display = "none";
    statusHangar.textContent = "";
  }
  if (data.current_distance !== "ND") {
    current_distance.style.display = "block";
    current_distance.textContent = data.current_distance;
  } else {
    current_distance.style.display = "none";
  }
  takaOffBtn.disabled = !data.can_take_off;
  landingBtn.disabled = !data.can_land;
}

connect();
