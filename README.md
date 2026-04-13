# Smart Parking Lot Management System

A multi-platform smart parking project that includes:

- A full Qt desktop management application
- A browser-based web frontend (Vite)
- Python utility tooling
- End-user and technical documentation

The system is designed to manage day-to-day parking lot operations such as vehicle entry, exit billing, spot tracking, and transaction reporting.

## Highlights

- Dashboard with occupancy and revenue stats
- Vehicle entry flow with spot-type and duration selection
- Vehicle exit workflow with fee calculation and payment processing
- Real-time lot status and occupancy visibility
- Transaction history and CSV export support
- Admin capabilities in the web implementation

## Tech Stack

- C++17 + Qt Widgets (desktop app)
- JavaScript + Vite (web app)
- Python (support utilities)
- Markdown documentation for guides and specs

## Repository Structure

```text
.
├── desktop-app/          # Qt desktop application (C++)
│   ├── include/
│   ├── src/
│   ├── resources/
│   └── ParkingGUI.pro
├── web/                  # Vite frontend
│   ├── public/
│   ├── src/
│   ├── index.html
│   └── vercel.json
├── python-tools/         # Python utilities and logs
├── docs/                 # User guide, UI/UX spec, quick reference, case handling
├── assets/
│   └── images/
├── tests/
├── LICENSE
└── README.md
```

## Submission Package Format

Use the following folder layout when preparing your final submission bundle:

```text
[Project Name]_Submission/
|___01_Source_Code/
|   |___main.cpp
|   |___MyClass.h
|   |___MyClass.cpp
|
|___02_Input_Files/
|   |___input.txt       // or relevant file e.g., nav_data.csv, grid_config.txt
|   |___stop_words.txt  // if applicable
|
|___03_Test_Report/
|   |___Example_Execution.txt   // console output demonstration
|
|___04_Technical_README/
	|___README.pdf      // document explaining move semantics / DMA usage
```

For this project, adapt section names as needed while keeping the same top-level numbering style.

## Quick Start

### 1) Desktop App (Qt)

Prerequisites:

- Qt 5.12+ with qmake support
- C++17 toolchain

Run:

1. Open `desktop-app/ParkingGUI.pro` in Qt Creator.
2. Select a valid Qt kit.
3. Build and run.

For more details, see `desktop-app/README.md` and `docs/README_GUI.md`.

### 2) Web App (Vite)

Prerequisites:

- Node.js 18+ and npm

Run locally:

```bash
cd web
npm install
npm run dev
```

Build and preview:

```bash
cd web
npm run build
npm run preview
```

For deployment details, see `web/README.md`.

### 3) Python Tools

```bash
cd python-tools
python -m venv .venv
source .venv/bin/activate   # macOS/Linux
python parking_gui.py
```

See `python-tools/README.md` for folder details.

## Documentation

Detailed docs are available in `docs/`:

- `docs/USER_GUIDE.md` - End-user usage walkthrough
- `docs/QUICK_REFERENCE.md` - Fast operational reference
- `docs/CASE_HANDLING.md` - Error and edge-case handling
- `docs/UI_UX_SPECIFICATION.md` - UI/UX design specification
- `docs/README_GUI.md` - Desktop GUI feature and architecture notes

## Deployment (Web)

The web app is configured for Git-based deployment to Vercel.

Recommended Vercel project settings:

- Root Directory: `web`
- Build Command: `npm run build`
- Output Directory: `dist`

SPA routing fallback is configured in `web/vercel.json`.

## Screenshots

Add screenshots to `assets/images/` for a richer project page.

Suggested names:

- `assets/images/dashboard.png`
- `assets/images/entry-form.png`
- `assets/images/exit-billing.png`

## Contributing

1. Fork the repository.
2. Create a feature branch.
3. Make focused changes.
4. Open a pull request with a clear description and test notes.

## License

This project is licensed under the terms of the `LICENSE` file in the repository root.
