let chart = null;
let dataPoints = [];
let maxDataPoints = 50;

function initChart() {
  const ctx = document.getElementById("waterChart").getContext("2d");
  chart = new Chart(ctx, {
    type: "line",
    data: {
      labels: [],
      datasets: [
        {
          label: "Water Level (m)",
          data: [],
          borderColor: "#3498db",
          backgroundColor: "rgba(52, 152, 219, 0.1)",
          borderWidth: 2,
          tension: 0.4,
          fill: true,
          pointRadius: 2,
          pointHoverRadius: 5,
        },
      ],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      animation: {
        duration: 300,
      },
      scales: {
        y: {
          min: 0,
          max: 14,
          beginAtZero: true,
          title: {
            display: true,
            text: "Water Level (m)",
          },
        },
        x: {
          title: {
            display: true,
            text: "Time",
          },
        },
      },
      plugins: {
        legend: {
          display: true,
        },
      },
    },
  });
}

function addDataPoint(level, timestamp) {
  const time = new Date(timestamp).toLocaleTimeString();

  dataPoints.push(level);

  // Keep only last N points
  if (chart.data.labels.length >= maxDataPoints) {
    chart.data.labels.shift();
    chart.data.datasets[0].data.shift();
  }

  chart.data.labels.push(time);
  chart.data.datasets[0].data.push(level);
  chart.update("none");
}

window.onload = () => {
  initChart();
  connect();
};
