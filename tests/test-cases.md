# CampusMate — Manual Test Cases & Results

> **Project Note:** As this is a prototype/demo version, our testing focused on verifying the core rules (like the UPSA email restriction), the editing flows after login, and checking for cross-device compatibility. Some advanced features were not fully programmed for this demo.

**Tester:** Melissa Adjarkie Adjanor, Cadnel-Morrel Darteyvi Mensah, Joshua Apre Yaw, Nana Yaw Obeng Forson, Samuel Duku Yeboah
**Date:** July 2026

**Legend:** ✅ Passed · ❌ Failed · ⚠️ Passed with caveat (bug found / partial feature)

---

## 1. Rules & Regulations (Authentication)

### TC-01 — UPSA Email Restriction (Rule Check)
1. Launch app and choose **Register**.
2. Attempt to register with an invalid email (e.g., `melissa@gmail.com`).
3. Attempt to register with a valid UPSA email (e.g., `melissa@upsamail.edu.gh`).

**Expected:** App rejects non-UPSA emails. App accepts UPSA emails.
**Result:** ✅ Passed. The app successfully enforced the `@upsamail.edu.gh` rule.

---

### TC-02 — Login & Dashboard Access
1. Log in with the demo account or a registered account.
2. Verify that the dashboard shows the correct user details.

**Expected:** User is logged in and sees their specific dashboard.
**Result:** ✅ Passed.

---

### TC-03 — Password Masking
1. Type a password during login.

**Expected:** Characters are hidden with asterisks (`*`).
**Result:** ✅ Passed.

---

## 2. Editing & Deployment of Functions (Post-Login)

### TC-04 — Adding Courses
1. Log in.
2. Navigate to **Courses** -> **Add Course**.
3. Enter a course name, code, and venue.

**Expected:** Course is successfully added to the list and saved.
**Result:** ✅ Passed. (Function deployed successfully).

---

### TC-05 — Editing Courses at Each Stage
1. Log in -> **Courses** -> **Edit Course**.
2. Select an existing course and change only the name.
3. Change only the time/venue.

**Expected:** The app allows partial edits without erasing the rest of the course data.
**Result:** ✅ Passed. Editing works smoothly after logging in.

---

### TC-06 — Adding Grades & Events
1. Navigate to **Grades** and add a grade.
2. Navigate to **Schedule** and add an event or IA.

**Expected:** Data is added and CGPA updates correctly.
**Result:** ⚠️ Passed with caveat. Some functions took multiple test runs to execute properly, but eventually worked.

---

## 3. Bugs & Compatibility Issues

### TC-07 — Running on Different Devices
1. Compile and run the C++ file on the original development machine.
2. Attempt to compile and run the exact same file on a different laptop/device.

**Expected:** The app should run identically on all devices.
**Result:** ❌ **Bug Found.** Sometimes the app would not open or would fail to run on other devices without specific compiler/environment adjustments.

---

### TC-08 — Incomplete Demo Features
1. Navigate through all menus (Settings, Calendar, etc.).
2. Try to interact with all buttons.

**Expected:** All features should be fully functional.
**Result:** ⚠️ **Known Limitation.** Since this is a demo version, some parts were not fully programmed and required multiple test runs to work correctly during the presentation.

---

## Summary of Findings

| Area | Status | Notes |
|------|--------|-------|
| UPSA Email Rules | ✅ Pass | Strictly enforced. |
| Editing Post-Login | ✅ Pass | Fully functional for courses and settings. |
| Core Functions | ⚠️ Partial | Worked, but some required multiple test runs. |
| Cross-Device Running | ❌ Fail | Compilation/Runtime issues on other devices. |

**Conclusion:** The core rules (like email validation) and basic CRUD operations (editing courses/grades) work as intended. However, for a final release, the code needs to be stabilized so it runs out-of-the-box on any machine without compilation errors, and incomplete features need to be finished.
