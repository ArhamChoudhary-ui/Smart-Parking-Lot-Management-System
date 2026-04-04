// Smart Parking Lot Management System - Browser implementation

import "./style.css";

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
  bindAdminModal();
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

function bindAdminModal() {
  const modal = document.getElementById("admin-modal");
  if (!modal) return;

  // Close modal when clicking outside
  window.addEventListener("click", (event) => {
    if (event.target === modal) {
      closeAdminModal();
    }
  });

  // Close modal when pressing Escape
  document.addEventListener("keydown", (event) => {
    if (event.key === "Escape" && modal.classList.contains("show")) {
      closeAdminModal();
    }
  });
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
  if (screenId === "admin") {
    if (adminLoggedIn) {
      showAdminPanel();
    } else {
      showAdminNotLogged();
    }
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
  renderParkingVisual();
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

function renderParkingVisual() {
  const container = document.getElementById("parking-lot-visual");
  if (!container) {
    return;
  }

  container.innerHTML = "";

  state.spots.forEach((spot) => {
    const slot = document.createElement("div");
    slot.className = `parking-slot ${spot.occupied ? "occupied" : "available"}`;
    slot.innerHTML = `
      <span class="slot-id">${spot.id}</span>
      <span class="slot-type">${spotTypeIcon(spot.type)}</span>
      <span class="slot-status">${spot.occupied ? "Occupied" : "Available"}</span>
    `;
    slot.title =
      spot.occupied ?
        `Spot ${spot.id}: ${spot.vehicleNo} (${titleCase(spot.type)})`
      : `Spot ${spot.id}: Available (${titleCase(spot.type)})`;
    container.appendChild(slot);
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
  if (rows.length === 0) {
    const tr = document.createElement("tr");
    tr.innerHTML = `<td colspan="7" class="text-center">No transactions yet.</td>`;
    body.appendChild(tr);
    updateTransactionSummary(rows);
    return;
  }

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
  const peakHours = total ? "10:00 AM - 12:00 PM" : "N/A";
  const summary = document.querySelector(".summary-info");
  if (!summary) {
    return;
  }
  summary.innerHTML =
    `Total Transactions: <strong>${total}</strong> | ` +
    `Total Revenue: <strong>${formatINR(revenue)}</strong> | ` +
    `Average Transaction: <strong>${formatINR(avg)}</strong> | ` +
    `Peak Hours: <strong>${peakHours}</strong>`;
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

// ==================== ADMIN PORTAL FUNCTIONS ====================

const ADMIN_PASSWORD = "admin123"; // Simple password - change in production
let adminLoggedIn = false;

function openAdminModal() {
  const modal = document.getElementById("admin-modal");
  if (modal) {
    modal.classList.add("show");
    document.getElementById("admin-password").value = "";
    document.getElementById("login-error").textContent = "";
  }
}

function closeAdminModal() {
  const modal = document.getElementById("admin-modal");
  if (modal) {
    modal.classList.remove("show");
    document.getElementById("admin-password").value = "";
    document.getElementById("login-error").textContent = "";
  }
}

function adminLogin() {
  const password = document.getElementById("admin-password").value;
  const errorEl = document.getElementById("login-error");

  if (!password) {
    setStatus("login-error", "Please enter a password", "error");
    return;
  }

  if (password === ADMIN_PASSWORD) {
    adminLoggedIn = true;
    closeAdminModal();
    showAdminPanel();
    setStatus("login-error", "Login successful!", "success");
    setTimeout(() => {
      document.getElementById("login-error").textContent = "";
    }, 2000);
    showScreen("admin");
  } else {
    setStatus("login-error", "Invalid password. Try again.", "error");
  }
}

function adminLogout() {
  adminLoggedIn = false;
  showAdminNotLogged();
  setStatus("admin-not-logged", "You have been logged out.", "success");
  setTimeout(() => {
    showScreen("dashboard");
  }, 1500);
}

function showAdminPanel() {
  const notLogged = document.getElementById("admin-not-logged");
  const logged = document.getElementById("admin-logged");

  if (notLogged) notLogged.style.display = "none";
  if (logged) logged.style.display = "block";

  updateAdminStats();
}

function showAdminNotLogged() {
  const notLogged = document.getElementById("admin-not-logged");
  const logged = document.getElementById("admin-logged");

  if (notLogged) notLogged.style.display = "block";
  if (logged) logged.style.display = "none";
}

function updateAdminStats() {
  const totalTxn = state.transactions.length;
  const totalRev = state.totalRevenue;
  const vehiclesCount = state.spots.filter((s) => s.occupied).length;

  setText("admin-total-txn", totalTxn);
  setText("admin-total-revenue", formatINR(totalRev));
  setText("admin-vehicles-today", vehiclesCount);
  setText("admin-peak-hour", "10:00 AM - 12:00 PM");
}

function viewAllVehicles() {
  const vehicles = state.spots.filter((s) => s.occupied);

  if (vehicles.length === 0) {
    alert("No vehicles currently parked.");
    return;
  }

  let vehicleList = "=== Currently Parked Vehicles ===\n\n";
  vehicles.forEach((v) => {
    vehicleList += `Spot ${v.id} (${titleCase(v.type)}): ${v.vehicleNo} (${v.category}, ${v.allottedHours}h)\n`;
  });

  alert(vehicleList);
}

function searchVehicle() {
  const searchInput = document
    .getElementById("search-vehicle")
    .value.trim()
    .toLowerCase();
  const resultEl = document.getElementById("search-result");

  if (!searchInput) {
    resultEl.textContent = "Enter a vehicle number to search.";
    return;
  }

  const found = state.spots.find(
    (s) => s.occupied && String(s.vehicleNo).toLowerCase() === searchInput,
  );

  if (found) {
    resultEl.innerHTML = `
      <div style="padding:10px; background:#d4edda; border:2px solid #28a745; border-radius:5px;">
        <strong>✓ Vehicle Found!</strong><br>
        Spot ID: ${found.id} (${titleCase(found.type)})<br>
        Vehicle: ${found.vehicleNo}<br>
        Category: ${found.category}<br>
        Allotted: ${found.allottedHours} hours
      </div>
    `;
  } else {
    resultEl.innerHTML = `
      <div style="padding:10px; background:#f8d7da; border:2px solid #dc3545; border-radius:5px;">
        <strong>✗ Vehicle Not Found</strong><br>
        No vehicle with number "${searchInput}" is currently parked.
      </div>
    `;
  }
}

function removeVehicle() {
  if (!adminLoggedIn) {
    alert("Admin access required.");
    return;
  }

  const spotId = Number(document.getElementById("remove-spot-id").value);
  const statusEl = document.getElementById("remove-status");

  if (!spotId || spotId < 1 || spotId > state.spots.length) {
    setStatus("remove-status", "Enter a valid spot ID (1-30).", "error");
    return;
  }

  const spot = state.spots[spotId - 1];

  if (!spot.occupied) {
    setStatus("remove-status", `Spot ${spotId} is already empty.`, "error");
    return;
  }

  const vehicleNo = spot.vehicleNo;
  freeSpot(spot);
  addTransaction({
    type: "admin-remove",
    spotId,
    vehicleNo,
    category: spot.category,
    duration: 0,
    amount: 0,
  });

  updateAdminStats();
  updateAllViews();

  setStatus(
    "remove-status",
    `✓ Vehicle ${vehicleNo} removed from Spot ${spotId}.`,
    "success",
  );
  document.getElementById("remove-spot-id").value = "";

  setTimeout(() => {
    statusEl.textContent = "";
  }, 3000);
}

function confirmSystemReset() {
  if (!adminLoggedIn) {
    alert("Admin access required.");
    return;
  }

  const confirmed = confirm(
    "⚠️ WARNING: This will clear ALL vehicles and transactions!\n\nAre you sure you want to reset the entire system?\n\nClick OK to confirm, Cancel to abort.",
  );

  if (confirmed) {
    const confirmed2 = confirm(
      "This is your FINAL WARNING.\n\nAll data will be permanently deleted.\n\nProceed?",
    );

    if (confirmed2) {
      resetSystem();
    }
  }
}

function resetSystem() {
  // Clear all vehicles
  state.spots.forEach((spot) => {
    freeSpot(spot);
  });

  // Clear transactions
  state.transactions = [];
  state.totalRevenue = 0;

  // Reset transaction counter
  state.nextTxnId = 1;

  updateAdminStats();
  updateAllViews();

  alert(
    "✓ System has been completely reset!\n\nAll vehicles and transactions have been cleared.",
  );
  showScreen("dashboard");
}

function generateAuditReport() {
  if (!adminLoggedIn) {
    alert("Admin access required.");
    return;
  }

  const filterType = document.getElementById("audit-filter").value;
  const reportEl = document.getElementById("audit-report");

  const filtered =
    filterType === "all" ?
      state.transactions
    : state.transactions.filter((t) => t.type === filterType);

  if (filtered.length === 0) {
    reportEl.textContent = "No transactions found for the selected filter.";
    return;
  }

  let report = "=== AUDIT REPORT ===\n";
  report += `Generated: ${new Date().toLocaleString("en-IN")}\n`;
  report += `Filter: ${filterType.toUpperCase()}\n`;
  report += `Total Records: ${filtered.length}\n`;
  report += `\n${"─".repeat(80)}\n\n`;

  filtered.forEach((txn, index) => {
    report += `[${index + 1}] ${txn.timestamp}\n`;
    report += `    Type: ${txn.type.toUpperCase()}\n`;
    report += `    Spot ID: ${txn.spotId}\n`;
    report += `    Vehicle: ${txn.vehicleNo}\n`;
    report += `    Category: ${txn.category || "N/A"}\n`;
    report += `    Amount: ${formatINR(txn.amount || 0)}\n`;
    report += `\n`;
  });

  report += `${"─".repeat(80)}\n`;
  report += `Total Revenue: ${formatINR(
    filtered.reduce((sum, t) => sum + (t.amount || 0), 0),
  )}\n`;

  reportEl.textContent = report;
}

function exportAuditCSV() {
  if (!adminLoggedIn) {
    alert("Admin access required.");
    return;
  }

  const filterType = document.getElementById("audit-filter").value;
  const filtered =
    filterType === "all" ?
      state.transactions
    : state.transactions.filter((t) => t.type === filterType);

  if (filtered.length === 0) {
    alert("No transactions to export.");
    return;
  }

  const headers = [
    "Timestamp",
    "Type",
    "SpotID",
    "VehicleNumber",
    "VehicleCategory",
    "Duration",
    "Amount",
  ];

  const csvRows = [headers.join(",")];
  filtered.forEach((txn) => {
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
  link.download = `admin-audit-${new Date().toISOString().split("T")[0]}.csv`;
  link.click();
  URL.revokeObjectURL(url);
}

window.parkVehicle = parkVehicle;
window.clearParkForm = clearParkForm;
window.calculateFee = calculateFee;
window.processExit = processExit;
window.clearExitForm = clearExitForm;
window.filterTransactions = filterTransactions;
window.exportTransactions = exportTransactions;
window.showScreen = showScreen;
window.openAdminModal = openAdminModal;
window.closeAdminModal = closeAdminModal;
window.adminLogin = adminLogin;
window.adminLogout = adminLogout;
window.viewAllVehicles = viewAllVehicles;
window.searchVehicle = searchVehicle;
window.removeVehicle = removeVehicle;
window.confirmSystemReset = confirmSystemReset;
window.generateAuditReport = generateAuditReport;
window.exportAuditCSV = exportAuditCSV;

document.addEventListener("DOMContentLoaded", initSystem);
