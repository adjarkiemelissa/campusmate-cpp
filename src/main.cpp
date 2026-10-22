// ============================================================
//  CAMPUS MATE - Very Basic Version (No Emojis)
//  Simple student assistant: Login, Courses, Schedule, Grades.
// ============================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

// ------------------------------------------------
//  Utility Functions
// ------------------------------------------------

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

vector<string> split(const string& s, char delim) {
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delim)) tokens.push_back(token);
    return tokens;
}

string toLower(const string& s) {
    string r = s;
    for (size_t i = 0; i < r.size(); i++) r[i] = tolower(r[i]);
    return r;
}

string getCurrentDate() {
    time_t now = time(0);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
    return string(buf);
}

string getDayOfWeek(int day, int month, int year) {
    tm tm = {};
    tm.tm_mday = day;
    tm.tm_mon = month - 1;
    tm.tm_year = year - 1900;
    mktime(&tm);
    const char* days[] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
    return days[tm.tm_wday];
}

int daysInMonth(int month, int year) {
    if (month == 2) return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
    const int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    return days[month-1];
}

int firstDayOfMonth(int month, int year) {
    tm tm = {};
    tm.tm_mday = 1;
    tm.tm_mon = month - 1;
    tm.tm_year = year - 1900;
    mktime(&tm);
    return tm.tm_wday;
}

void sleepMs(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitForEnter() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

string readPassword() {
    string password;
    char ch;
#ifdef _WIN32
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') { if (!password.empty()) { password.pop_back(); cout << "\b \b"; } }
        else { password += ch; cout << '*'; }
    }
    cout << "\n";
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    getline(cin, password);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
    return password;
}

// ------------------------------------------------
//  Simple Colors (keep minimal)
// ------------------------------------------------

const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string DIM = "\033[2m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string BLUE = "\033[34m";
const string CYAN = "\033[36m";
const string BG_PURPLE = "\033[48;5;57m";
const string WHITE = "\033[37m";

// ------------------------------------------------
//  Data Structures
// ------------------------------------------------

struct User {
    string email, password, name, studentType, degreeType, programme, goal;
};

struct Course {
    int id;
    string name, code, day, time, lecturer, venue;
    bool isIA;
    string userEmail;
};

struct Grade {
    int id, semester;
    string course, grade;
    string userEmail;
};

struct Event {
    int id;
    string title, day, time;
    bool isIA;
    string userEmail;
};

// ------------------------------------------------
//  Global State
// ------------------------------------------------

vector<User> users;
vector<Course> courses;
vector<Grade> grades;
vector<Event> events;
User* currentUser = 0;
int studyHours = 2;
string goal = "First Class";
int currentMonth = 7, currentYear = 2026;

const string DATA_DIR = "campusmate_data/";
const string USERS_FILE = "users.txt";
const string COURSES_FILE = "courses.txt";
const string GRADES_FILE = "grades.txt";
const string EVENTS_FILE = "events.txt";
const string SETTINGS_FILE = "settings.txt";

// ------------------------------------------------
//  File I/O
// ------------------------------------------------

void createDataDir() {
#ifdef _WIN32
    system(("mkdir " + DATA_DIR + " 2>nul").c_str());
#else
    system(("mkdir -p " + DATA_DIR).c_str());
#endif
}

string filePath(const string& f) { return DATA_DIR + f; }

void saveUsers() {
    ofstream f(filePath(USERS_FILE).c_str());
    if (!f.is_open()) return;
    for (size_t i = 0; i < users.size(); i++)
        f << users[i].email << "|" << users[i].password << "|" << users[i].name << "|"
          << users[i].studentType << "|" << users[i].degreeType << "|"
          << users[i].programme << "|" << users[i].goal << "\n";
    f.close();
}

void loadUsers() {
    users.clear();
    ifstream f(filePath(USERS_FILE).c_str());
    if (!f.is_open()) return;
    string line;
    while (getline(f, line)) {
        vector<string> p = split(line, '|');
        if (p.size() >= 7) {
            User u;
            u.email = p[0]; u.password = p[1]; u.name = p[2];
            u.studentType = p[3]; u.degreeType = p[4];
            u.programme = p[5]; u.goal = p[6];
            users.push_back(u);
        }
    }
    f.close();
}

void saveCourses() {
    ofstream f(filePath(COURSES_FILE).c_str());
    if (!f.is_open()) return;
    for (size_t i = 0; i < courses.size(); i++)
        f << courses[i].id << "|" << courses[i].name << "|" << courses[i].code << "|"
          << courses[i].day << "|" << courses[i].time << "|" << courses[i].lecturer << "|"
          << courses[i].venue << "|" << (courses[i].isIA ? "1" : "0") << "|"
          << courses[i].userEmail << "\n";
    f.close();
}

void loadCourses() {
    courses.clear();
    ifstream f(filePath(COURSES_FILE).c_str());
    if (!f.is_open()) return;
    string line;
    while (getline(f, line)) {
        vector<string> p = split(line, '|');
        if (p.size() >= 9) {
            Course c;
            c.id = atoi(p[0].c_str());
            c.name = p[1]; c.code = p[2]; c.day = p[3]; c.time = p[4];
            c.lecturer = p[5]; c.venue = p[6];
            c.isIA = (p[7] == "1"); c.userEmail = p[8];
            courses.push_back(c);
        }
    }
    f.close();
}

void saveGrades() {
    ofstream f(filePath(GRADES_FILE).c_str());
    if (!f.is_open()) return;
    for (size_t i = 0; i < grades.size(); i++)
        f << grades[i].id << "|" << grades[i].semester << "|"
          << grades[i].course << "|" << grades[i].grade << "|"
          << grades[i].userEmail << "\n";
    f.close();
}

void loadGrades() {
    grades.clear();
    ifstream f(filePath(GRADES_FILE).c_str());
    if (!f.is_open()) return;
    string line;
    while (getline(f, line)) {
        vector<string> p = split(line, '|');
        if (p.size() >= 5) {
            Grade g;
            g.id = atoi(p[0].c_str());
            g.semester = atoi(p[1].c_str());
            g.course = p[2]; g.grade = p[3]; g.userEmail = p[4];
            grades.push_back(g);
        }
    }
    f.close();
}

void saveEvents() {
    ofstream f(filePath(EVENTS_FILE).c_str());
    if (!f.is_open()) return;
    for (size_t i = 0; i < events.size(); i++)
        f << events[i].id << "|" << events[i].title << "|"
          << events[i].day << "|" << events[i].time << "|"
          << (events[i].isIA ? "1" : "0") << "|" << events[i].userEmail << "\n";
    f.close();
}

void loadEvents() {
    events.clear();
    ifstream f(filePath(EVENTS_FILE).c_str());
    if (!f.is_open()) return;
    string line;
    while (getline(f, line)) {
        vector<string> p = split(line, '|');
        if (p.size() >= 6) {
            Event e;
            e.id = atoi(p[0].c_str());
            e.title = p[1]; e.day = p[2]; e.time = p[3];
            e.isIA = (p[4] == "1"); e.userEmail = p[5];
            events.push_back(e);
        }
    }
    f.close();
}

void saveSettings() {
    ofstream f(filePath(SETTINGS_FILE).c_str());
    if (!f.is_open()) return;
    f << studyHours << "\n" << goal << "\n" << currentMonth << "\n" << currentYear << "\n";
    f.close();
}

void loadSettings() {
    ifstream f(filePath(SETTINGS_FILE).c_str());
    if (!f.is_open()) return;
    string line;
    if (getline(f, line)) studyHours = atoi(line.c_str());
    if (getline(f, line)) goal = line;
    if (getline(f, line)) currentMonth = atoi(line.c_str());
    if (getline(f, line)) currentYear = atoi(line.c_str());
    f.close();
}

void loadAllData() {
    loadUsers(); loadCourses(); loadGrades(); loadEvents(); loadSettings();
}

void saveAllData() {
    saveUsers(); saveCourses(); saveGrades(); saveEvents(); saveSettings();
}

// ------------------------------------------------
//  Seed Default Data
// ------------------------------------------------

void seedData() {
    if (users.empty()) {
        User demo;
        demo.email = "student@upsamail.edu.gh";
        demo.password = "password123";
        demo.name = "Student Name";
        demo.studentType = "Regular";
        demo.degreeType = "Bachelors";
        demo.programme = "BSc Information Technology";
        demo.goal = "First Class";
        users.push_back(demo);
    }
    if (events.empty()) {
        Event e1, e2;
        e1.id = 1; e1.title = "Study Group"; e1.day = "Monday"; e1.time = "14:00";
        e1.isIA = false; e1.userEmail = "";
        e2.id = 2; e2.title = "CSC 201 Test"; e2.day = "Wednesday"; e2.time = "10:00";
        e2.isIA = true; e2.userEmail = "";
        events.push_back(e1); events.push_back(e2);
    }
    saveAllData();
}

// ------------------------------------------------
//  UI Helpers (No Emojis)
// ------------------------------------------------

void printHeader(const string& title) {
    cout << "\n" << BG_PURPLE << " " << WHITE << BOLD << " CampusMate " << RESET;
    cout << " " << WHITE << BOLD << "UPSA Portal" << RESET << "\n";
    cout << string(50, '=') << "\n";
    if (!title.empty()) cout << CYAN << BOLD << "  " << title << RESET << "\n";
}

void clearAndHeader(const string& title) {
    clearScreen();
    printHeader(title);
    cout << string(50, '-') << "\n";
}

void printSuccess(const string& m) { cout << GREEN << "[OK] " << m << RESET << "\n"; }
void printError(const string& m) { cout << RED << "[ERR] " << m << RESET << "\n"; }
void printInfo(const string& m) { cout << BLUE << "[INFO] " << m << RESET << "\n"; }

string getInput(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return trim(s);
}

string getInputReq(const string& prompt) {
    while (true) {
        string s = getInput(prompt);
        if (!s.empty()) return s;
        printError("Cannot be empty.");
    }
}

int getIntInput(const string& prompt, int minV, int maxV) {
    while (true) {
        string s = getInput(prompt);
        if (s.empty()) continue;
        bool ok = true;
        for (size_t i = 0; i < s.size(); i++)
            if (!isdigit(s[i]) && s[i] != '-') { ok = false; break; }
        if (!ok) { printError("Enter a number."); continue; }
        int v = atoi(s.c_str());
        if (v < minV || v > maxV) {
            printError("Value must be between " + to_string(minV) + " and " + to_string(maxV));
            continue;
        }
        return v;
    }
}

int menuChoice(const vector<string>& options) {
    while (true) {
        for (size_t i = 0; i < options.size(); i++)
            cout << "  " << (i+1) << ". " << options[i] << "\n";
        cout << "\n";
        int c = getIntInput("Choose: ", 1, (int)options.size());
        return c-1;
    }
}

// ------------------------------------------------
//  Auth Functions
// ------------------------------------------------

bool isValidEmail(const string& e) {
    return e.find("@upsamail.edu.gh") != string::npos;
}

void showLogin() {
    clearAndHeader("Login");
    string email = getInputReq("Email: ");
    cout << "Password: ";
    string pass = readPassword();

    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].email == email && users[i].password == pass) {
            currentUser = &users[i];
            goal = users[i].goal;
            printSuccess("Welcome, " + users[i].name + "!");
            sleepMs(500);
            return;
        }
    }
    printError("Invalid credentials.");
    sleepMs(1000);
}

void showRegister() {
    clearAndHeader("Register");
    string name = getInputReq("Full Name: ");
    string email = getInputReq("UPSA Email: ");
    if (!isValidEmail(email)) {
        printError("Must end with @upsamail.edu.gh");
        sleepMs(1000); return;
    }
    for (size_t i = 0; i < users.size(); i++)
        if (users[i].email == email) { printError("Email already used."); sleepMs(1000); return; }

    cout << "Password: ";
    string pass = readPassword();
    if (pass.length() < 6) { printError("Min 6 chars."); sleepMs(1000); return; }

    string type = getInput("Student Type (Regular/Evening/Weekend) [Regular]: ");
    if (type.empty()) type = "Regular";
    string degree = getInput("Degree (Bachelors/Diploma) [Bachelors]: ");
    if (degree.empty()) degree = "Bachelors";
    string prog = getInput("Programme [BSc IT]: ");
    if (prog.empty()) prog = "BSc Information Technology";
    string g = getInput("Goal [First Class]: ");
    if (g.empty()) g = "First Class";

    User u;
    u.email = email; u.password = pass; u.name = name;
    u.studentType = type; u.degreeType = degree;
    u.programme = prog; u.goal = g;
    users.push_back(u);
    saveAllData();
    printSuccess("Account created!");
    sleepMs(1000);
}

void showAuth() {
    while (true) {
        clearAndHeader("Welcome");
        cout << "\n 1. Login\n 2. Register\n 3. Exit\n";
        int c = getIntInput("Option: ", 1, 3);
        if (c == 1) { showLogin(); if (currentUser) break; }
        else if (c == 2) showRegister();
        else exit(0);
    }
}

// ------------------------------------------------
//  Courses
// ------------------------------------------------

vector<Course> getMyCourses() {
    vector<Course> r;
    for (size_t i = 0; i < courses.size(); i++)
        if (courses[i].userEmail == currentUser->email)
            r.push_back(courses[i]);
    return r;
}

void addCourse() {
    clearAndHeader("Add Course");
    string name = getInputReq("Name: ");
    string code = getInputReq("Code: ");
    string day = getInput("Day [Monday]: "); if (day.empty()) day = "Monday";
    string time = getInput("Time [11:00am-2:00pm]: "); if (time.empty()) time = "11:00am-2:00pm";
    string lecturer = getInput("Lecturer (optional): ");
    string venue = getInputReq("Venue: ");

    Course c;
    c.id = courses.empty() ? 1 : courses.back().id + 1;
    c.name = name; c.code = code; c.day = day; c.time = time;
    c.lecturer = lecturer; c.venue = venue;
    c.isIA = false; c.userEmail = currentUser->email;
    courses.push_back(c);
    saveAllData();
    printSuccess("Course added.");
    sleepMs(1000);
}

void addIA() {
    clearAndHeader("Add IA/Test");
    string name = getInputReq("IA/Test name: ");
    string code = getInputReq("Course code: ");
    string day = getInput("Day [Monday]: "); if (day.empty()) day = "Monday";
    string time = getInput("Time [10:00am-12:00pm]: "); if (time.empty()) time = "10:00am-12:00pm";
    string venue = getInputReq("Venue: ");

    Course c;
    c.id = courses.empty() ? 1 : courses.back().id + 1;
    c.name = "IA: " + name;
    c.code = code + " (IA)";
    c.day = day; c.time = time;
    c.lecturer = "TBA"; c.venue = venue;
    c.isIA = true; c.userEmail = currentUser->email;
    courses.push_back(c);
    saveAllData();
    printInfo("IA added.");
    sleepMs(1000);
}

void editCourse() {
    vector<Course> my = getMyCourses();
    if (my.empty()) { printError("No courses."); sleepMs(1000); return; }
    clearAndHeader("Edit Course");
    for (size_t i = 0; i < my.size(); i++)
        cout << " " << (i+1) << ". " << my[i].code << " - " << my[i].name << "\n";
    int idx = getIntInput("Select: ", 1, (int)my.size()) - 1;
    int real = -1;
    for (size_t i = 0; i < courses.size(); i++)
        if (courses[i].id == my[idx].id) { real = i; break; }
    if (real == -1) { printError("Not found."); sleepMs(1000); return; }

    cout << "\n(Enter to keep current)\n";
    string name = getInput("Name [" + courses[real].name + "]: ");
    if (!name.empty()) courses[real].name = name;
    string code = getInput("Code [" + courses[real].code + "]: ");
    if (!code.empty()) courses[real].code = code;
    string day = getInput("Day [" + courses[real].day + "]: ");
    if (!day.empty()) courses[real].day = day;
    string time = getInput("Time [" + courses[real].time + "]: ");
    if (!time.empty()) courses[real].time = time;
    string venue = getInput("Venue [" + courses[real].venue + "]: ");
    if (!venue.empty()) courses[real].venue = venue;
    saveAllData();
    printSuccess("Updated.");
    sleepMs(1000);
}

void deleteCourse() {
    vector<Course> my = getMyCourses();
    if (my.empty()) { printError("No courses."); sleepMs(1000); return; }
    clearAndHeader("Delete Course");
    for (size_t i = 0; i < my.size(); i++)
        cout << " " << (i+1) << ". " << my[i].code << " - " << my[i].name << "\n";
    int idx = getIntInput("Select: ", 1, (int)my.size()) - 1;
    cout << "Delete " << my[idx].code << "? (y/n): ";
    string conf = getInput("");
    if (toLower(conf) != "y") { printInfo("Cancelled."); sleepMs(1000); return; }
    for (size_t i = 0; i < courses.size(); ) {
        if (courses[i].id == my[idx].id) courses.erase(courses.begin() + i);
        else i++;
    }
    saveAllData();
    printSuccess("Deleted.");
    sleepMs(1000);
}

void showCourses() {
    while (true) {
        clearAndHeader("Courses");
        vector<Course> my = getMyCourses();
        if (my.empty()) cout << "\nNo courses yet.\n";
        else {
            cout << "\n";
            for (size_t i = 0; i < my.size(); i++) {
                cout << " " << (i+1) << ". " << CYAN << my[i].code << RESET
                     << " - " << my[i].name;
                if (my[i].isIA) cout << " " << RED << "[IA]" << RESET;
                cout << "\n   " << DIM << my[i].day << " " << my[i].time
                     << " @ " << my[i].venue << RESET;
                if (!my[i].lecturer.empty())
                    cout << " (Lecturer: " << my[i].lecturer << ")";
                cout << "\n\n";
            }
        }
        cout << "\n";
        vector<string> opts;
        opts.push_back("Add Course");
        opts.push_back("Add IA/Test");
        opts.push_back("Edit Course");
        opts.push_back("Delete Course");
        opts.push_back("Back");
        int c = menuChoice(opts);
        if (c == 0) addCourse();
        else if (c == 1) addIA();
        else if (c == 2) editCourse();
        else if (c == 3) deleteCourse();
        else break;
    }
}

// ------------------------------------------------
//  Schedule
// ------------------------------------------------

void showCalendar() {
    cout << "\n  " << CYAN << BOLD << currentMonth << "/" << currentYear << RESET << "\n  ";
    const char* days[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    for (int i = 0; i < 7; i++) cout << setw(5) << days[i];
    cout << "\n";
    int first = firstDayOfMonth(currentMonth, currentYear);
    int total = daysInMonth(currentMonth, currentYear);
    int cnt = 0;
    for (int i = 0; i < first; i++) { cout << setw(5) << " "; cnt++; }
    for (int d = 1; d <= total; d++) {
        string dayName = getDayOfWeek(d, currentMonth, currentYear);
        bool hasEvent = false, hasIA = false;
        for (size_t i = 0; i < events.size(); i++) {
            if (events[i].day == dayName) {
                hasEvent = true;
                if (events[i].isIA) hasIA = true;
            }
        }
        string color = hasIA ? RED : (hasEvent ? CYAN : "");
        cout << color << setw(5) << d << RESET;
        cnt++;
        if (cnt % 7 == 0 && d < total) cout << "\n  ";
    }
    cout << "\n\n  Legend: " << CYAN << "*" << RESET << " Event  " << RED << "*" << RESET << " IA/Test\n";
}

void viewDayEvents() {
    string day = getInputReq("Enter day (Monday-Sunday): ");
    cout << "\n";
    bool found = false;
    for (size_t i = 0; i < events.size(); i++) {
        if (toLower(events[i].day) == toLower(day)) {
            cout << " " << (events[i].isIA ? RED : CYAN) << "* " << events[i].title
                 << " (" << events[i].time << ")"
                 << (events[i].isIA ? " [IA]" : "") << RESET << "\n";
            found = true;
        }
    }
    if (!found) printInfo("No events on " + day);
    waitForEnter();
}

void addEvent() {
    clearAndHeader("Add Event");
    string title = getInputReq("Title: ");
    string day = getInput("Day [Monday]: "); if (day.empty()) day = "Monday";
    string time = getInput("Time [12:00]: "); if (time.empty()) time = "12:00";
    Event e;
    e.id = events.empty() ? 1 : events.back().id + 1;
    e.title = title; e.day = day; e.time = time;
    e.isIA = false; e.userEmail = currentUser->email;
    events.push_back(e);
    saveAllData();
    printSuccess("Event added.");
    sleepMs(1000);
}

void addIAEvent() {
    clearAndHeader("Add IA Event");
    string title = getInputReq("IA Title: ");
    string day = getInput("Day [Monday]: "); if (day.empty()) day = "Monday";
    string time = getInput("Time [10:00]: "); if (time.empty()) time = "10:00";
    Event e;
    e.id = events.empty() ? 1 : events.back().id + 1;
    e.title = "IA: " + title;
    e.day = day; e.time = time;
    e.isIA = true; e.userEmail = currentUser->email;
    events.push_back(e);
    saveAllData();
    printInfo("IA event added.");
    sleepMs(1000);
}

void showSchedule() {
    while (true) {
        clearAndHeader("Schedule");
        cout << "\n  " << DIM << "Study hours/day: " << studyHours
             << "   Goal: " << goal << RESET << "\n";
        showCalendar();
        cout << "\n";
        vector<string> opts;
        opts.push_back("View Day Events");
        opts.push_back("Add Event");
        opts.push_back("Add IA/Test");
        opts.push_back("Back");
        int c = menuChoice(opts);
        if (c == 0) viewDayEvents();
        else if (c == 1) addEvent();
        else if (c == 2) addIAEvent();
        else break;
    }
}

// ------------------------------------------------
//  Grades
// ------------------------------------------------

vector<Grade> getMyGrades() {
    vector<Grade> r;
    for (size_t i = 0; i < grades.size(); i++)
        if (grades[i].userEmail == currentUser->email)
            r.push_back(grades[i]);
    return r;
}

double gradePoint(const string& g) {
    if (g == "A") return 4.0;
    if (g == "A-") return 3.7;
    if (g == "B+") return 3.3;
    if (g == "B") return 3.0;
    if (g == "B-") return 2.7;
    if (g == "C+") return 2.3;
    if (g == "C") return 2.0;
    if (g == "D") return 1.0;
    return 0.0;
}

void addGrade() {
    clearAndHeader("Add Grade");
    int sem = getIntInput("Semester (1-12): ", 1, 12);
    string course = getInputReq("Course code: ");
    cout << "Grade (A, A-, B+, B, B-, C+, C, D, F) [A]: ";
    string g = getInput("");
    if (g.empty()) g = "A";
    // Validate
    vector<string> valid;
    valid.push_back("A"); valid.push_back("A-"); valid.push_back("B+");
    valid.push_back("B"); valid.push_back("B-"); valid.push_back("C+");
    valid.push_back("C"); valid.push_back("D"); valid.push_back("F");
    bool ok = false;
    for (size_t i = 0; i < valid.size(); i++)
        if (valid[i] == g) { ok = true; break; }
    if (!ok) { printError("Invalid, using A."); g = "A"; }
    Grade gr;
    gr.id = grades.empty() ? 1 : grades.back().id + 1;
    gr.semester = sem; gr.course = course; gr.grade = g;
    gr.userEmail = currentUser->email;
    grades.push_back(gr);
    saveAllData();
    printSuccess("Grade added.");
    sleepMs(1000);
}

void showGrades() {
    while (true) {
        clearAndHeader("Grades");
        vector<Grade> my = getMyGrades();
        if (my.empty()) cout << "\nNo grades yet.\n";
        else {
            cout << "\n";
            double total = 0;
            int credits = 0;
            const int per = 3;
            for (size_t i = 0; i < my.size(); i++) {
                double p = gradePoint(my[i].grade);
                total += p * per;
                credits += per;
                cout << " * " << my[i].course << " - " << CYAN << my[i].grade << RESET
                     << " (Sem " << my[i].semester << ")\n";
            }
            double cgpa = (credits > 0) ? total / credits : 0.0;
            cout << "\n  " << CYAN << BOLD << "CGPA: " << fixed << setprecision(2) << cgpa << RESET << "\n";
        }
        cout << "\n";
        vector<string> opts;
        opts.push_back("Add Grade");
        opts.push_back("Back");
        int c = menuChoice(opts);
        if (c == 0) addGrade();
        else break;
    }
}

// ------------------------------------------------
//  Settings (Simple)
// ------------------------------------------------

void changeGoal() {
    cout << "\n  Goals:\n  1. First Class\n  2. Second Class Upper\n  3. Second Class Lower\n  4. Pass\n  5. Just Graduate\n";
    int c = getIntInput("Select: ", 1, 5);
    const char* goals[] = {"First Class","Second Class Upper","Second Class Lower","Pass","Just Graduate"};
    goal = goals[c-1];
    if (currentUser) {
        for (size_t i = 0; i < users.size(); i++)
            if (users[i].email == currentUser->email) { users[i].goal = goal; break; }
    }
    saveAllData();
    printSuccess("Goal set to " + goal);
    sleepMs(1000);
}

void changeStudyHours() {
    int h = getIntInput("Study hours per day (0-24): ", 0, 24);
    studyHours = h;
    saveSettings();
    printSuccess("Updated.");
    sleepMs(1000);
}

void showSettings() {
    while (true) {
        clearAndHeader("Settings");
        cout << "\n  Goal: " << goal << "\n  Study Hours/Day: " << studyHours << "\n";
        cout << "\n";
        vector<string> opts;
        opts.push_back("Change Goal");
        opts.push_back("Change Study Hours");
        opts.push_back("Delete Account");
        opts.push_back("Back");
        int c = menuChoice(opts);
        if (c == 0) changeGoal();
        else if (c == 1) changeStudyHours();
        else if (c == 2) {
            cout << "\nType 'YES' to delete account: ";
            string conf = getInput("");
            if (conf == "YES") {
                string email = currentUser->email;
                // Remove user
                for (size_t i = 0; i < users.size(); ) {
                    if (users[i].email == email) users.erase(users.begin() + i);
                    else i++;
                }
                // Remove user's courses
                for (size_t i = 0; i < courses.size(); ) {
                    if (courses[i].userEmail == email) courses.erase(courses.begin() + i);
                    else i++;
                }
                // Remove user's grades
                for (size_t i = 0; i < grades.size(); ) {
                    if (grades[i].userEmail == email) grades.erase(grades.begin() + i);
                    else i++;
                }
                // Remove user's events
                for (size_t i = 0; i < events.size(); ) {
                    if (events[i].userEmail == email) events.erase(events.begin() + i);
                    else i++;
                }
                saveAllData();
                currentUser = 0;
                printSuccess("Account deleted.");
                sleepMs(1000);
                return; // exit settings loop
            } else {
                printInfo("Cancelled.");
                sleepMs(1000);
            }
        }
        else break;
    }
}

// ------------------------------------------------
//  Dashboard (Main App)
// ------------------------------------------------

void showDashboard() {
    while (true) {
        clearAndHeader("Dashboard");
        cout << "\n  " << CYAN << "User: " << currentUser->name << RESET
             << " (" << currentUser->studentType << " - " << currentUser->degreeType << ")\n";
        cout << "  " << DIM << "Programme: " << currentUser->programme << RESET << "\n";
        cout << "  " << DIM << "Goal: " << goal << RESET << "\n\n";
        cout << "  Courses: " << getMyCourses().size() << "\n";
        cout << "  Grades: " << getMyGrades().size() << "\n";
        // Count user events
        int evCount = 0;
        for (size_t i = 0; i < events.size(); i++)
            if (events[i].userEmail == currentUser->email || events[i].userEmail.empty())
                evCount++;
        cout << "  Events: " << evCount << "\n";

        cout << "\n" << string(50, '-') << "\n\n";
        vector<string> opts;
        opts.push_back("Courses");
        opts.push_back("Schedule");
        opts.push_back("Grades");
        opts.push_back("Settings");
        opts.push_back("Logout");
        int c = menuChoice(opts);
        if (c == 0) showCourses();
        else if (c == 1) showSchedule();
        else if (c == 2) showGrades();
        else if (c == 3) showSettings();
        else {
            currentUser = 0;
            printInfo("Logged out.");
            sleepMs(500);
            break;
        }
    }
}

// ------------------------------------------------
//  Main
// ------------------------------------------------

int main() {
    srand(time(0));
    createDataDir();
    loadAllData();
    seedData();

    while (true) {
        if (currentUser == 0) showAuth();
        else showDashboard();
    }
    return 0;
}
