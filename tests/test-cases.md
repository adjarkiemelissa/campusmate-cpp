# CampusMate — Manual Test Cases

> Run these in order on a fresh install (`campusmate_data/` deleted) unless marked "requires existing data."

**Legend:** ✅ Passed · ❌ Failed · ⚠️ Passed with caveat

---

## 1. Authentication

### TC-01 — Login with demo account
1. Launch app
2. Choose **1. Login**
3. Email: `student@upsamail.edu.gh`
4. Password: `password123`

**Expected:** Green "Welcome, Student Name!" message, dashboard appears after ~0.5s.
**Actual:** ⬜

---

### TC-02 — Login with wrong password
1. Launch app → **1. Login**
2. Email: `student@upsamail.edu.gh`
3. Password: `wrongpass`

**Expected:** Red "Invalid credentials." message, returns to auth menu after 1s.
**Actual:** ⬜

---

### TC-03 — Password masking
1. At the "Password: " prompt, type `hello`

**Expected:** Four `*` characters appear, no plaintext.
**Actual:** ⬜

---

### TC-04 — Register with invalid email
1. Launch → **2. Register**
2. Enter any name
3. Email: `nobody@gmail.com`

**Expected:** Red "Must end with @upsamail.edu.gh", returns to menu.
**Actual:** ⬜

---

### TC-05 — Register with short password
1. Register flow
2. Email: `newuser@upsamail.edu.gh`
3. Password: `abc`

**Expected:** Red "Min 6 chars."
**Actual:** ⬜

---

### TC-06 — Register with duplicate email
1. Register with an email already in `users.txt`

**Expected:** Red "Email already used."
**Actual:** ⬜

---

### TC-07 — Register with defaults (blank fields)
1. Register flow, press Enter on Student Type, Degree, Programme, Goal

**Expected:** Account created with values `Regular / Bachelors / BSc Information Technology / First Class`.
**Actual:** ⬜

---

## 2. Courses

### TC-08 — Add a course
1. Login → **Courses → Add Course**
2. Name: `Data Structures`, Code: `CSC 205`, Venue: `Room 12`, press Enter on Day/Time

**Expected:** Green "Course added." Course appears in list with defaults `Monday 11:00am-2:00pm`.
**Actual:** ⬜

---

### TC-09 — Add a course with all fields
1. Add Course → fill every field including Lecturer

**Expected:** Lecturer name shown in parentheses in the course list.
**Actual:** ⬜

---

### TC-10 — Add an IA
1. Courses → **Add IA/Test**
2. Name: `Midterm`, Code: `CSC 205`, Venue: `Auditorium`

**Expected:** Course listed as `CSC 205 (IA)` with a red **[IA]** tag.
**Actual:** ⬜

---

### TC-11 — Edit a course (partial update)
1. Add course `CSC 205` → Edit Course
2. Change only the **Name** to `Advanced Data Structures`; press Enter on everything else

**Expected:** Only name changes; day/time/venue preserved.
**Actual:** ⬜

---

### TC-12 — Delete a course (confirm)
1. Delete Course → select → type `y`

**Expected:** Green "Deleted." Course vanishes from list and from `courses.txt`.
**Actual:** ⬜

---

### TC-13 — Delete a course (cancel)
1. Delete Course → select → type `n`

**Expected:** Blue "Cancelled." Course remains.
**Actual:** ⬜

---

### TC-14 — Course isolation between users
1. Log out. Register a second user.
2. Check Courses for the new user.

**Expected:** New user sees "No courses yet." — sees *their own* courses only.
**Actual:** ⬜

---

## 3. Schedule

### TC-15 — Calendar renders current month
1. Login → **Schedule**

**Expected:** Grid with correct days of week, correct first-day offset, correct days-per-month, coloured day numbers where events exist.
**Actual:** ⬜

---

### TC-16 — Add event and verify calendar
1. Schedule → Add Event
2. Title: `Study Group`, Day: `Monday`

**Expected:** Every Monday in calendar appears cyan.
**Actual:** ⬜

---

### TC-17 — Add IA event and verify colour
1. Schedule → Add IA/Test
2. Day: `Wednesday`

**Expected:** Every Wednesday in calendar appears red.
**Actual:** ⬜

---

### TC-18 — View day events
1. Schedule → View Day Events → enter `Monday`

**Expected:** Lists Study Group with time. Pressing Enter returns to schedule.
**Actual:** ⬜

---

### TC-19 — View day with no events
1. View Day Events → enter a day with no events, e.g. `Saturday`

**Expected:** Blue "No events on Saturday".
**Actual:** ⬜

---

## 4. Grades

### TC-20 — Add grade and verify CGPA
1. Grades → Add Grade
2. Semester `1`, Course `CSC 101`, Grade `A`

**Expected:** CGPA = 4.00 (single A).
**Actual:** ⬜

---

### TC-21 — Multiple grades CGPA calculation
1. Add grades: `A` (Sem 1), `B` (Sem 1), `C` (Sem 2)

**Expected:** CGPA = (4.0 + 3.0 + 2.0) / 3 = **3.00**.
**Actual:** ⬜

---

### TC-22 — Invalid grade fallback
1. Add Grade, enter grade `Z`

**Expected:** Red "Invalid, using A.", grade stored as `A`.
**Actual:** ⬜

---

### TC-23 — Empty grade defaults to A
1. Add Grade, press Enter at grade prompt

**Expected:** Grade stored as `A`.
**Actual:** ⬜

---

### TC-24 — Empty grades screen
1. New user → Grades

**Expected:** "No grades yet." No CGPA line.
**Actual:** ⬜

---

## 5. Settings

### TC-25 — Change goal
1. Settings → Change Goal → pick `2. Second Class Upper`

**Expected:** Green "Goal set to Second Class Upper". Dashboard header updates.
**Actual:** ⬜

---

### TC-26 — Change study hours (boundary)
1. Settings → Change Study Hours → enter `25`

**Expected:** Red "Value must be between 0 and 24".
**Actual:** ⬜

---

### TC-27 — Change study hours (valid)
1. Enter `5`

**Expected:** Green "Updated." Schedule header shows 5.
**Actual:** ⬜

---

### TC-28 — Delete account (confirm)
1. Settings → Delete Account → type `YES`

**Expected:** Account and all their courses/grades/events removed. Returns to login.
**Actual:** ⬜

---

### TC-29 — Delete account (cancel)
1. Delete Account → type anything else

**Expected:** Blue "Cancelled." Account intact.
**Actual:** ⬜

---

## 6. Persistence

### TC-30 — Data survives restart
1. Add course, grade, and event
2. Close app (Ctrl+C or exit)
3. Re-launch and login

**Expected:** All data still present.
**Actual:** ⬜

---

### TC-31 — Files created on first run
1. Delete `campusmate_data/`
2. Launch app

**Expected:** `campusmate_data/` folder created automatically with all five `.txt` files, `users.txt` contains demo account.
**Actual:** ⬜

---

### TC-32 — Corrupted line handling
1. Manually add `garbage` line to `courses.txt`
2. Re-launch

**Expected:** App ignores the malformed line, keeps working.
**Actual:** ⬜ *(Note: depends on `split()` returning <9 tokens — verify.)*

---

## 7. Edge Cases / Robustness

### TC-33 — Empty menu selection
1. Press Enter at any `Choose:` prompt

**Expected:** Re-prompts without crashing.
**Actual:** ⬜

---

### TC-34 — Very long input
1. Add a course with 1000-character name

**Expected:** Accepted (no buffer overflow).
**Actual:** ⬜

---

### TC-35 — Login prompt with extra spaces
1. Login with `  student@upsamail.edu.gh  ` (surrounded by spaces)

**Expected:** Trims and logs in successfully.
**Actual:** ⬜

---

## Summary

| Section | Passed | Failed | Notes |
|---------|--------|--------|-------|
| Auth | /7 | | |
| Courses | /7 | | |
| Schedule | /5 | | |
| Grades | /5 | | |
| Settings | /5 | | |
| Persistence | /3 | | |
| Edge Cases | /3 | | |

**Tester:** _____________ **Date:** _____________ **Build:** _____________
