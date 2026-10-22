# CampusMate

> A lightweight, terminal-based student assistant for UPSA students — manage courses, schedules, and grades without the bloat.

![C++](https://img.shields.io/badge/C%2B%2B-11-blue)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)
![License](https://img.shields.io/badge/license-MIT-green)

---

## Overview

**CampusMate** is a command-line application built as a final project for our Programming class. It helps students at the University of Professional Studies, Accra (UPSA) keep track of:

- **Courses** — add, edit, delete, and mark IA/tests
- **Schedule** — monthly calendar view with events and IA reminders
- **Grades** — record grades per semester and auto-calculate CGPA
- **Goals** — set academic targets (First Class, Second Upper, etc.)
- **Profile** — persistent, password-protected user accounts

Data is stored locally in plain text files under `campusmate_data/` — no internet connection required.

---

## Features

| Feature | Description |
|---------|-------------|
| Secure Login | Password input is masked on both Windows and Unix |
| Multi-user | Each account has its own courses, grades, and events |
| Course Manager | Full CRUD + IA/test flag with color-coded UI |
| Calendar View | Highlights days with events (cyan) or IAs (red) |
| CGPA Calculator | Weighted by course credits (3 each) |
| Customizable | Study hours/day and academic goal stored per user |
| Account Delete | Full data wipe on request |

---

## Tech Stack

- **Language:** C++11
- **Libraries:** Standard Library only (no external dependencies)
- **Persistence:** Pipe-delimited plain-text files
- **Terminal UI:** ANSI escape codes for colour & cursor control
- **Cross-platform:** Windows (`<windows.h>`, `<conio.h>`) + Unix (`<termios.h>`)

---

## Getting Started

### Prerequisites

- A C++11-capable compiler: `g++`, `clang++`, or MSVC
- Windows users: **MinGW-w64** or **MSYS2** (for ANSI colours + `g++`)

### Clone the repo

```bash
git clone https://github.com/<your-username>/campusmate-cpp.git
cd campusmate-cpp
```

### Build & run

**Option A — using `make` (recommended):**

```bash
make
make run
```

**Option B — manual compile:**

```bash
# Linux / macOS
g++ -std=c++11 -o campusmate src/main.cpp
./campusmate

# Windows (MinGW)
g++ -std=c++11 -o campusmate.exe src/main.cpp
campusmate.exe
```

### Demo account

| Field | Value |
|-------|-------|
| Email | `student@upsamail.edu.gh` |
| Password | `password123` |

> Registration requires an email ending in `@upsamail.edu.gh`.

---

## Screenshots

| Login | Dashboard | Schedule |
|-------|-----------|----------|
| ![login](screenshots/login.png) | ![dashboard](screenshots/dashboard.png) | ![schedule](screenshots/schedule.png) |

*(Add screenshots to `screenshots/` — see below for how to capture them.)*

### Capturing screenshots

1. Run the app
2. On **Windows:** `Win + Shift + S` → save as `.png`
3. On **Linux:** `gnome-screenshot` or `scrot`
4. On **macOS:** `Cmd + Shift + 4`
5. Name them `login.png`, `dashboard.png`, etc. and drop them into `screenshots/`

---

## Project Structure

```
campusmate-cpp/
├── src/
│   └── main.cpp              # Single-file implementation
├── docs/
│   ├── presentation.pdf      # Final presentation slides
│   └── design-notes.md       # Architecture & design decisions
├── screenshots/              # UI screenshots for README
├── tests/
│   └── test-cases.md         # Manual test scenarios & results
├── Makefile
├── LICENSE
└── README.md
```

---

## Data Storage

All user data is written to `campusmate_data/` next to the executable:

| File | Format | Example |
|------|--------|---------|
| `users.txt` | `email\|password\|name\|type\|degree\|programme\|goal` | — |
| `courses.txt` | `id\|name\|code\|day\|time\|lecturer\|venue\|isIA\|userEmail` | — |
| `grades.txt` | `id\|semester\|course\|grade\|userEmail` | — |
| `events.txt` | `id\|title\|day\|time\|isIA\|userEmail` | — |
| `settings.txt` | `studyHours\ngoal\nmonth\nyear` | — |

> **Security note:** This is an academic prototype. Passwords are stored in plaintext. A production version would hash them (e.g., bcrypt / Argon2).

---

## Team

| Name | Role |
|------|------|
| *Your Name* | Project lead / C++ developer |
| *Friend 1* | Documentation / testing |
| *Friend 2* | UI design / presentation |

---

## Roadmap

- [ ] Hash passwords before storage
- [ ] Export grades as CSV / PDF
- [ ] Weekly timetable view (not just monthly)
- [ ] Reminder notifications for upcoming IAs
- [ ] GUI port (Qt or Electron)

---

## License

MIT — see [LICENSE](LICENSE).

---

## Acknowledgements

- UPSA Department of Information Technology — for the project brief
- Our classmates who tested early builds
