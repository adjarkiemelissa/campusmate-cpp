# CampusMate — Design Notes

> Companion document to the source. Explains *why* the code looks the way it does.

---

## 1. Goals & Constraints

| Goal | Rationale |
|------|-----------|
| **Zero dependencies** | Builds anywhere with just `g++` — no package manager needed |
| **Cross-platform** | Works in the lab (Windows) and at home (Linux/macOS) |
| **Single-file architecture** | Simple to submit and grade; no fragile CMake configs |
| **Human-readable storage** | Plain text so markers can inspect data files directly |
| **Offline-first** | No network calls — pure terminal app |

**Constraints we accepted:**
- No password hashing (prototype scope; documented as a known limitation)
- No concurrency (single user at a time; correct by construction)
- Terminal-only UI (no GUI library allowed in the brief)

---

## 2. Architecture

The program is **monolithic** — one `main.cpp` file organised into clear sections:

```
┌───────────────────────────────────────────┐
│                 main()                    │
│  ┌─────────────────────────────────────┐  │
│  │   Auth Loop  (showAuth)             │  │
│  │      ↓ (on success)                 │  │
│  │   Dashboard  (showDashboard)        │  │
│  │    ├── Courses   (showCourses)      │  │
│  │    ├── Schedule  (showSchedule)     │  │
│  │    ├── Grades    (showGrades)       │  │
│  │    └── Settings  (showSettings)     │  │
│  └─────────────────────────────────────┘  │
└───────────────────────────────────────────┘
                ↕
      ┌─────────────────────┐
      │   File I/O Layer    │
      │  loadAllData / save │
      └─────────────────────┘
                ↕
      ┌─────────────────────┐
      │  campusmate_data/   │
      │   *.txt files       │
      └─────────────────────┘
```

### Design pattern: **Screen loop**

Every menu is a `while(true)` loop that:
1. clears the screen and prints a header,
2. shows a numbered menu,
3. dispatches to a handler,
4. breaks back up one level.

This gives a natural stack of screens with an implicit "Back" button.

---

## 3. Data Structures

```cpp
struct User    { email, password, name, studentType, degreeType, programme, goal };
struct Course  { id, name, code, day, time, lecturer, venue, isIA, userEmail };
struct Grade   { id, semester, course, grade, userEmail };
struct Event   { id, title, day, time, isIA, userEmail };
```

**Key decisions:**

- **Ownership via `userEmail`.** Rather than one nested vector per user, we keep flat global vectors and filter by email. Simpler file I/O, and easy to extend to sharing later.
- **`isIA` boolean** on both `Course` and `Event` lets the UI colour-code IAs red without a separate type.
- **Manual `id` counters** (`courses.back().id + 1`). Fine for a single-user-at-a-time app; would need mutex in a concurrent version.
- **`currentUser` as a raw pointer** into the `users` vector. Safe because we never `push_back` during a logged-in session… but brittle. **Known limitation.**

---

## 4. File Format

Pipe-delimited, one record per line. Chosen because:

- `std::getline` + `split()` handles it in 3 lines of code
- Easy to eyeball in a text editor
- No escaping needed *as long as* field values don't contain `|` (documented limitation)

**Saving strategy:** *write-whole-file on every mutation*. Simple, atomic enough for our use, and avoids partial-write bugs. Cost: O(n) I/O per action — irrelevant at this scale.

**Loading strategy:** *load-all-on-startup*. Keeps every menu fast and lets us cross-reference (e.g., counting a user's events on the dashboard).

---

## 5. Cross-Platform Abstractions

Two OS-specific concerns are isolated behind helper functions:

| Concern | Windows | Unix |
|---------|---------|------|
| **Password masking** | `_getch()` loop with `\b` backspace handling | `tcsetattr` to disable `ECHO`, then `getline` |
| **Sleep** | `Sleep(ms)` | `usleep(ms * 1000)` |
| **Clear screen** | `system("cls")` | `system("clear")` |

All other logic is pure standard C++, so behaviour is identical across platforms.

**Risks:**
- `system("cls")` / `system("clear")` is not portable to shells that lack them, but both standard shells on the target platforms do.
- Windows legacy console (`cmd.exe`) needs ANSI VT processing enabled manually on older builds; modern Windows Terminal handles it fine.

---

## 6. UI Design Choices

- **ANSI escape codes** for colour (`\033[31m`, etc.) — no curses library required.
- **Purple header** (`BG_PURPLE`) to give the app identity without being distracting.
- **Semantic colouring:** green = success, red = error, cyan = info, blue = highlight.
- **Emoji-free** by request — safer on older terminals and more readable in screenshots.
- **Confirmations for destructive actions** (delete course, delete account) to prevent accidental data loss.

---

## 7. Known Limitations

| # | Limitation | Suggested fix |
|---|------------|---------------|
| 1 | Passwords stored in plaintext | Hash with bcrypt / Argon2 |
| 2 | `currentUser` raw pointer can dangle if `users` is resized | Use `optional<User>` or index |
| 3 | Pipe `\|` in input breaks file format | Escape or use a binary format |
| 4 | No CGPA validation against goal | Add a "on track / off track" banner |
| 5 | `events` not filtered by user on calendar | Currently all users' events appear — intentional for shared events, but filter available |
| 6 | Fixed credit weight (3) per course | Add a `credits` field to `Course` |
| 7 | Semester max = 12 | Sufficient for BSc + some postgrad; make configurable |
| 8 | No concurrency | Single process only |

---

## 8. Testing Strategy

Manual test cases live in [`tests/test-cases.md`](../tests/test-cases.md). Each test covers a **happy path**, an **error path**, and a **boundary** where applicable. Automated tests were deemed out of scope for a 4-week course project.

---

## 9. Future Work

1. Extract file I/O into a `Storage` class for testability
2. Add a `--demo` flag that seeds richer sample data
3. Timetable view (Mon–Fri grid) alongside the calendar
4. Encrypt data file at rest (AES via a small library)
5. Optional GUI port using Qt for the "wow" factor in presentations

---

*Last updated: July 2026*
