// Smart Parking Lot Management System - Browser implementation

const PRICING = {
  standard: 20,
  disabled: 10,
  vip: 50,
};

const CAPACITY = {
  standard: 20,
  disabled: 5,
  vip: 5,
};

const state = {
  nextTxnId: 1,
  spots: [],
  transactions: [],
  totalRevenue: 0,
};

function initSystem() {
  initializeSpots();
  bindNavigation();
  bindInputs();
  seedInitialVehicles();
  updateAllViews();
  updateClock();
  setInterval(updateClock, 1000);
}

function initializeSpots() {
  state.spots = [];
  let id = 1;
  ["standard", "disabled", "vip"].forEach((type) => {
    for (let i = 0; i < CAPACITY[type]; i += 1) {
      state.spots.push({
        id,
        type,
        occupied: false,
        vehicleNo: null,
        category: null,
        allottedHours: null,
      });
      id += 1;
    }
  });
}

function seedInitialVehicles() {
  const sample = [
    { no: "MH-02-AB-1234", type: "standard", category: "4W", hours: 2 },
    { no: "KA-03-CD-9876", type: "disabled", category: "4W", hours: 3 },
    { no: "DL-01-XY-5678", type: "standard", category: "2W", hours: 1 },
    { no: "TN-04-EF-2345", type: "vip", category: "4W", hours: 2 },
  ];

  sample.forEach((vehicle) => {
    const spot = findAvailableSpot(vehicle.type);
    if (!spot) {
      return;
    }
    occupySpot(spot, vehicle.no, vehicle.category, vehicle.hours);
    addTransaction({
      type: "entry",
      spotId: spot.id,
      vehicleNo: vehicle.no,
      category: vehicle.category,
      duration: vehicle.hours,
      amount: 0,
    });
  });
}

function bindNavigation() {
  const buttons = document.querySelectorAll(".nav-btn");
  buttons.forEach((btn) => {
    btn.addEventListener("click", () => {
      const screen = btn.dataset.screen;
      showScreen(screen);
    });
  });

  const filter = document.getElementById("filter");
  if (filter) {
    filter.addEventListener("change", filterTransactions);
  }
}

function bindInputs() {
  const hours = document.getElementById("hours");
  const spotType = document.getElementById("spot-type");

  if (hours) {
    hours.addEventListener("input", updateFeePreview);
  }
  if (spotType) {
    spotType.addEventListener("change", updateFeePreview);
  }
}

function showScreen(screenId) {
  document.querySelectorAll(".screen").forEach((screen) => {
    screen.classList.remove("active");
  });

  const target = document.getElementById(screenId);
  if (target) {
    target.classList.add("active");
  }

  document.querySelectorAll(".nav-btn").forEach((btn) => {
    btn.classList.toggle("active", btn.dataset.screen === screenId);
  });

  if (screenId === "status") {
    renderSpotsTable();
  }
  if (screenId === "transactions") {
    renderTransactions();
  }
}

function updateClock() {
  const el = document.getElementById("clock");
  if (!el) {
    return;
  }
  const now = new Date();
  const time = now.toLocaleTimeString("en-IN", {
    hour: "2-digit",
    minute: "2-digit",
    second: "2-digit",
    hour12: true,
  });
  const day = now.toLocaleDateString("en-IN", {
    weekday: "long",
    day: "2-digit",
    month: "short",
  });
  el.innerHTML = `${time}<br />${day}`;
}

function updateFeePreview() {
  const type = getValue("spot-type");
  const hours = Number(getValue("hours") || 0);
  const fee = hours > 0 && PRICING[type] ? hours * PRICING[type] : 0;
  const feeEl = document.getElementById("fee-display");
  if (feeEl) {
    feeEl.textContent = formatINR(fee);
  }
}

function parkVehicle() {
  clearStatus("park-status");

  const vehicleNo = (getValue("vehicle-no") || "").trim();
  const type = getValue("spot-type");
  const category = getValue("category");
  const hours = Number(getValue("hours") || 0);

  if (!vehicleNo) {
    setStatus("park-status", "Enter vehicle name or number.", "error");
    return;
  }

  if (hours < 1 || hours > 24) {
    setStatus(
      "park-status",
      "Allotted hours must be between 1 and 24.",
      "error",
    );
    return;
  }

  if (findVehicleSpot(vehicleNo)) {
    setStatus("park-status", "This vehicle is already parked.", "error");
    return;
  }

  const spot = findAvailableSpot(type);
  if (!spot) {
    setStatus(
      "park-status",
      `No ${titleCase(type)} spot available right now.`,
      "error",
    );
    return;
  }

  occupySpot(spot, vehicleNo, category, hours);
  addTransaction({
    type: "entry",
    spotId: spot.id,
    vehicleNo,
    category,
    duration: hours,
    amount: 0,
  });

  setStatus(
    "park-status",
    `Vehicle parked at spot ${spot.id} (${titleCase(type)}).`,
    "success",
  );

  clearParkForm();
  updateAllViews();
}

function calculateFee() {
  clearStatus("exit-status");

  const spotId = Number(getValue("spot-id") || 0);
  const h = Number(getValue("actual-hours") || 0);
  const m = Number(getValue("extra-minutes") || 0);

  if (!spotId || spotId < 1 || spotId > state.spots.length) {
    setStatus("exit-status", "Enter a valid spot ID.", "error");
    return;
  }
  if (h < 0 || h > 24 || m < 0 || m > 59) {
    setStatus("exit-status", "Enter valid duration values.", "error");
    return;
  }

  const spot = state.spots[spotId - 1];
  if (!spot.occupied) {
    setStatus("exit-status", `Spot ${spotId} is not occupied.`, "error");
    return;
  }

  const totalHours = h + m / 60;
  const amount = Math.ceil(totalHours * PRICING[spot.type]);

  const details = [
    `Vehicle: ${spot.vehicleNo}`,
    `Spot ID: ${spot.id}`,
    `Spot Type: ${titleCase(spot.type)} (${formatINR(PRICING[spot.type])}/hour)`,
    `Duration: ${h}h ${m}m (${totalHours.toFixed(2)}h)`,
    `Total Payable: ${formatINR(amount)}`,
  ].join("\n");

  setText("billing-details", details);
  setText("exit-fee", formatINR(amount));
}

function processExit() {
  clearStatus("exit-status");

  const spotId = Number(getValue("spot-id") || 0);
  const paid = Number(getValue("paid-amount") || 0);

  if (!spotId || spotId < 1 || spotId > state.spots.length) {
    setStatus("exit-status", "Enter a valid spot ID.", "error");
    return;
  }

  const spot = state.spots[spotId - 1];
  if (!spot.occupied) {
    setStatus("exit-status", `Spot ${spotId} is not occupied.`, "error");
    return;
  }

  const h = Number(getValue("actual-hours") || 0);
  const m = Number(getValue("extra-minutes") || 0);
  if (h < 0 || h > 24 || m < 0 || m > 59) {
    setStatus("exit-status", "Enter valid duration values.", "error");
    return;
  }

  const totalHours = h + m / 60;
  const amount = Math.ceil(totalHours * PRICING[spot.type]);

  if (paid < amount) {
    setStatus(
      "exit-status",
      `Insufficient payment. Need ${formatINR(amount)}.`,
      "error",
    );
    return;
  }

  const vehicleNo = spot.vehicleNo;
  const category = spot.category;

  freeSpot(spot);
  addTransaction({
    type: "exit",
    spotId,
    vehicleNo,
    category,
    duration: Number(totalHours.toFixed(2)),
    amount,
  });
  state.totalRevenue += amount;

  const change = paid - amount;
  setStatus(
    "exit-status",
    `Exit complete. Amount ${formatINR(amount)} | Change ${formatINR(change)}.`,
    "success",
  );

  clearExitForm();
  updateAllViews();
}

function clearParkForm() {
  setValue("vehicle-no", "");
  setValue("spot-type", "standard");
  setValue("category", "2W");
  setValue("hours", 2);
  setText("fee-display", formatINR(0));
}

function clearExitForm() {
  setValue("spot-id", "");
  setValue("actual-hours", 2);
  setValue("extra-minutes", 0);
  setValue("paid-amount", "");
  setText("billing-details", "Click 'Calculate Fee' to view details");
  setText("exit-fee", formatINR(0));
}

function updateAllViews() {
  updateDashboard();
  updateStatusCards();
  renderSpotsTable();
  renderTransactions();
  updateFeePreview();
}

function updateDashboard() {
  const occupied = state.spots.filter((s) => s.occupied).length;
  const available = state.spots.length - occupied;
  const occupancy = Math.round((occupied / state.spots.length) * 100);

  setText("total-spots", state.spots.length);
  setText("occupied-spots", occupied);
  setText("available-spots", available);
  setText("occupancy-percent", `${occupancy}%`);

  const bar = document.querySelector(".progress-bar");
  if (bar) {
    bar.style.width = `${occupancy}%`;
  }

  const standardOccupied = state.spots.filter(
    (s) => s.type === "standard" && s.occupied,
  ).length;
  const disabledOccupied = state.spots.filter(
    (s) => s.type === "disabled" && s.occupied,
  ).length;
  const vipOccupied = state.spots.filter(
    (s) => s.type === "vip" && s.occupied,
  ).length;

  setText("standard-count", `${standardOccupied}/${CAPACITY.standard}`);
  setText("disabled-count", `${disabledOccupied}/${CAPACITY.disabled}`);
  setText("vip-count", `${vipOccupied}/${CAPACITY.vip}`);

  const totalEntries = state.transactions.filter(
    (t) => t.type === "entry",
  ).length;
  const totalExits = state.transactions.filter((t) => t.type === "exit").length;

  setText("total-vehicles", totalEntries);
  setText("today-vehicles", totalExits);
  setText("total-revenue", formatINR(state.totalRevenue));
}

function updateStatusCards() {
  const occupied = state.spots.filter((s) => s.occupied).length;
  const available = state.spots.length - occupied;
  const occupancy = Math.round((occupied / state.spots.length) * 100);

  setText("status-total", state.spots.length);
  setText("status-occupied", occupied);
  setText("status-available", available);
  setText("status-rate", `${occupancy}%`);
}

function renderSpotsTable() {
  const body = document.getElementById("spots-table-body");
  if (!body) {
    return;
  }

  body.innerHTML = "";
  state.spots.forEach((spot) => {
    const tr = document.createElement("tr");
    tr.innerHTML = `
      <td>${spot.id}</td>
      <td>${spotTypeIcon(spot.type)} ${titleCase(spot.type)}</td>
      <td>${spot.occupied ? '<span class="badge occupied">🔴 Occupied</span>' : '<span class="badge available">🟢 Available</span>'}</td>
      <td>${spot.vehicleNo || "-"}</td>
      <td>${spot.category || "-"}</td>
      <td>${spot.allottedHours ?? "-"}</td>
    `;
    body.appendChild(tr);
  });
}

function renderTransactions() {
  const body = document.getElementById("transactions-table-body");
  if (!body) {
    return;
  }

  const filter = getValue("filter") || "all";
  const rows =
    filter === "all" ?
      state.transactions
    : state.transactions.filter((t) => t.type === filter);

  body.innerHTML = "";
  rows
    .slice()
    .reverse()
    .forEach((txn) => {
      const tr = document.createElement("tr");
      tr.innerHTML = `
        <td>${txn.timestamp}</td>
        <td>${txn.type === "entry" ? '<span class="badge entry">🟢 Entry</span>' : '<span class="badge exit">🔴 Exit</span>'}</td>
        <td>${txn.spotId}</td>
        <td>${txn.vehicleNo}</td>
        <td>${txn.category || "-"}</td>
        <td>${txn.duration ?? "-"}</td>
        <td class="text-right ${txn.amount > 0 ? "text-success" : ""}">${txn.amount > 0 ? txn.amount.toFixed(2) : "-"}</td>
      `;
      body.appendChild(tr);
    });

  updateTransactionSummary(rows);
}

function updateTransactionSummary(rows) {
  const total = rows.length;
  const revenue = rows.reduce((sum, row) => sum + (row.amount || 0), 0);
  const avg = total ? revenue / total : 0;
  const summary = document.querySelector(".summary-info");
  if (!summary) {
    return;
  }
  summary.innerHTML =
    `Total Transactions: <strong>${total}</strong> | ` +
    `Total Revenue: <strong>${formatINR(revenue)}</strong> | ` +
    `Average Transaction: <strong>${formatINR(avg)}</strong> | ` +
    `Peak Hours: <strong>10:00 AM - 12:00 PM</strong>`;
}

function filterTransactions() {
  renderTransactions();
}

function exportTransactions() {
  if (state.transactions.length === 0) {
    alert("No transactions to export.");
    return;
  }

  const headers = [
    "Timestamp",
    "Type",
    "SpotID",
    "VehicleNumber",
    "VehicleCategory",
    "DurationHours",
    "Amount",
  ];

  const csvRows = [headers.join(",")];
  state.transactions.forEach((txn) => {
    csvRows.push(
      [
        txn.timestamp,
        txn.type,
        txn.spotId,
        txn.vehicleNo,
        txn.category || "",
        txn.duration ?? "",
        txn.amount ?? 0,
      ]
        .map((v) => `"${String(v).replaceAll('"', '""')}"`)
        .join(","),
    );
  });

  const blob = new Blob([csvRows.join("\n")], {
    type: "text/csv;charset=utf-8;",
  });
  const url = URL.createObjectURL(blob);
  const link = document.createElement("a");
  link.href = url;
  link.download = "parking-transactions.csv";
  link.click();
  URL.revokeObjectURL(url);
}

function addTransaction({
  type,
  spotId,
  vehicleNo,
  category,
  duration,
  amount,
}) {
  const txn = {
    id: state.nextTxnId,
    timestamp: new Date().toLocaleString("en-IN"),
    type,
    spotId,
    vehicleNo,
    category,
    duration,
    amount,
  };
  state.nextTxnId += 1;
  state.transactions.push(txn);
}

function findAvailableSpot(type) {
  return state.spots.find((s) => s.type === type && !s.occupied);
}

function findVehicleSpot(vehicleNo) {
  const needle = String(vehicleNo).toLowerCase();
  return state.spots.find(
    (s) => s.occupied && String(s.vehicleNo).toLowerCase() === needle,
  );
}

function occupySpot(spot, vehicleNo, category, hours) {
  spot.occupied = true;
  spot.vehicleNo = vehicleNo;
  spot.category = category;
  spot.allottedHours = hours;
}

function freeSpot(spot) {
  spot.occupied = false;
  spot.vehicleNo = null;
  spot.category = null;
  spot.allottedHours = null;
}

function spotTypeIcon(type) {
  if (type === "disabled") return "♿";
  if (type === "vip") return "⭐";
  return "🚗";
}

function titleCase(v) {
  return v.charAt(0).toUpperCase() + v.slice(1);
}

function formatINR(n) {
  return `₹${Number(n).toFixed(2)}`;
}

function getValue(id) {
  const el = document.getElementById(id);
  return el ? el.value : "";
}

function setValue(id, value) {
  const el = document.getElementById(id);
  if (el) {
    el.value = value;
  }
}

function setText(id, value) {
  const el = document.getElementById(id);
  if (el) {
    el.textContent = String(value);
  }
}

function setStatus(id, msg, type) {
  const el = document.getElementById(id);
  if (!el) {
    return;
  }
  el.textContent = msg;
  el.classList.remove("success", "error");
  el.classList.add(type);
}

function clearStatus(id) {
  const el = document.getElementById(id);
  if (!el) {
    return;
  }
  el.textContent = "";
  el.classList.remove("success", "error");
}

window.parkVehicle = parkVehicle;
window.clearParkForm = clearParkForm;
window.calculateFee = calculateFee;
window.processExit = processExit;
window.clearExitForm = clearExitForm;
window.filterTransactions = filterTransactions;
window.exportTransactions = exportTransactions;
window.showScreen = showScreen;

document.addEventListener("DOMContentLoaded", initSystem);
