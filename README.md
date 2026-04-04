# Smart Parking Management System

A multi-technology smart parking project with:

- A Qt desktop GUI for parking operations
- Python utility tools
- A web frontend prototype

## Features

- Parking entry and exit workflows
- Real-time lot status tracking
- Transaction history and reporting
- Documented UI/UX and case handling
- Web prototype for quick interface experimentation

## Tech Stack

- C++17 + Qt Widgets (desktop application)
- Python (support utilities)
- HTML, CSS, JavaScript (web frontend)

## Setup

### Desktop App (Qt)

1. Install Qt 5.12+ (or newer) with qmake support.
2. Open `desktop-app/ParkingGUI.pro` in Qt Creator.
3. Build and run the project.

### Python Tools

1. Create and activate a virtual environment.
2. Install dependencies if needed.
3. Run scripts from `python-tools/`.

### Web Frontend

1. Open `web/index.html` in a browser.
2. Or serve the folder with a local static server.

## Screenshots

Place screenshots in `assets/images/`.

Suggested files:

- `assets/images/dashboard.png`
- `assets/images/entry-form.png`
- `assets/images/exit-billing.png`

## Repository Structure

```text
smart-parking-management-system/
├── README.md
├── LICENSE
├── .gitignore
├── docs/
├── desktop-app/
├── web/
├── python-tools/
├── assets/
│   └── images/
└── tests/
```

## Notes

- Root is intentionally minimal for professional presentation.
- Generated binaries and build outputs are ignored via `.gitignore`.
