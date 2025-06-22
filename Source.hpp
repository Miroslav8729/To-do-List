#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip> // Äëÿ ôîðìàòèðîâàíèÿ âûâîäà
#include <limits> // Äëÿ std::numeric_limits
#include <sstream> // Äëÿ îáðàáîòêè ñòðîê

using namespace std;

struct User {
    string username;
    string password;
};

struct ToDoItem {
    int id;
    string title;
    string dueDate;
    string description;
    bool isDone;
};

// Îáúÿâëåíèå ôóíêöèé
bool registerUser(vector<User>& users);
bool loginUser(const vector<User>& users, string& currentUsername, User*& currentUser);
void loadUsersFromFile(vector<User>& users);
void saveUsersToFile(const vector<User>& users);
void loadTasksFromFile(vector<ToDoItem>& todoList, int& nextId, const string& username);
void saveTasksToFile(const vector<ToDoItem>& todoList, const string& username);
inline void clearScreen();
inline int showMenu();

inline bool registerUser(vector<User>& users) {
    string username, password;
    cout << "Ââåäèòå èìÿ ïîëüçîâàòåëÿ: ";
    getline(cin, username);

    // Ïðîâåðêà óíèêàëüíîñòè
    for (const auto& u : users) {
        if (u.username == username) {
            cout << "Ýòî èìÿ ïîëüçîâàòåëÿ óæå çàíÿòî." << endl;
            system("pause");
            return false;
        }
    }

    cout << "Ââåäèòå ïàðîëü: ";
    getline(cin, password);

    User newUser{ username, password };
    users.push_back(newUser);
    saveUsersToFile(users);
    cout << "Ðåãèñòðàöèÿ ïðîøëà óñïåøíî!" << endl;
    system("pause");
    return true;
}

inline bool loginUser(const vector<User>& users, string& currentUsername, User*& currentUser) {
    string username, password;
    cout << "Ââåäèòå èìÿ ïîëüçîâàòåëÿ: ";
    getline(cin, username);
    cout << "Ââåäèòå ïàðîëü: ";
    getline(cin, password);

    for (const auto& u : users) {
        if (u.username == username && u.password == password) {
            currentUsername = username;
            // Ïûòàåìñÿ íàéòè ïîëüçîâàòåëÿ äëÿ äîñòóïà ê äàííûì
            // Íî ïîñêîëüêó ó íàñ åñòü âåêòîð ïîëüçîâàòåëåé, ìîæíî íàéòè ïî èìåíè
            // è ïåðåäàòü óêàçàòåëü
            // Äëÿ ýòîãî ëó÷øå ïîëó÷èòü åãî òóò
            break;
        }
    }
    // Ïîêà èùåì ïîçæå. Ìîæíî ñäåëàòü òàê:
    for (auto& u : const_cast<vector<User>&>(users)) { // íå î÷åíü êîððåêòíî, íî äîïóñòèìî äëÿ ïðèìåðà
        if (u.username == username && u.password == password) {
            currentUser = &u;
            cout << "Âõîä âûïîëíåí óñïåøíî! Äîáðî ïîæàëîâàòü, " << currentUser->username << "." << endl;
            return true;
        }
    }
    cout << "Íåâåðíîå èìÿ ïîëüçîâàòåëÿ èëè ïàðîëü." << endl;
    system("pause");
    return false;
}

inline void loadUsersFromFile(vector<User>& users) {
    ifstream file("users.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            size_t delimPos = line.find(';');
            if (delimPos != string::npos) {
                User user;
                user.username = line.substr(0, delimPos);
                user.password = line.substr(delimPos + 1);
                users.push_back(user);
            }
        }
        file.close();
    }
}

inline void saveUsersToFile(const vector<User>& users) {
    ofstream file("users.txt");
    if (file.is_open()) {
        for (const auto& u : users) {
            file << u.username << ";" << u.password << "\n";
        }
        file.close();
    }
}

inline void loadTasksFromFile(vector<ToDoItem>& todoList, int& nextId, const string& username) {
    string filename = username + "_tasks.txt";
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            ToDoItem item;
            string token;

            getline(ss, token, ';');
            item.id = stoi(token);
            getline(ss, item.title, ';');
            getline(ss, item.dueDate, ';');
            getline(ss, item.description, ';');
            getline(ss, token, ';');
            item.isDone = (token == "1" || token == "true");
            todoList.push_back(item);
            if (item.id >= nextId) nextId = item.id + 1;
        }
        file.close();
    }
}

inline void saveTasksToFile(const vector<ToDoItem>& todoList, const string& username) {
    string filename = username + "_tasks.txt";
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& item : todoList) {
            file << item.id << ";" << item.title << ";" << item.dueDate << ";" << item.description << ";" << (item.isDone ? "1" : "0") << "\n";
        }
        file.close();
        cout << "Çàäà÷è óñïåøíî ñîõðàíåíû." << endl;
        cout << "Íàæìèòå Enter äëÿ ïðîäîëæåíèÿ...";
        cin.get();
    }
    else {
        cerr << "Îøèáêà ïðè ñîõðàíåíèè çàäà÷." << endl;
    }
}

inline void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

inline int showMenu() {
    int choice;
    clearScreen();
    cout << "--- ToDo List ---" << endl;
    cout << "1. Äîáàâèòü çàäà÷ó" << endl;
    cout << "2. Ïîêàçàòü ñïèñîê çàäà÷" << endl;
    cout << "3. Âûïîëíèòü çàäà÷ó" << endl;
    cout << "4. Ðåäàêòèðîâàòü çàäà÷ó" << endl;
    cout << "5. Óäàëèòü çàäà÷ó" << endl;
    cout << "6. Ñîõðàíèòü èçìåíåíèÿ" << endl;
    cout << "7. Çàãðóçèòü çàäà÷è èç ôàéëà" << endl;
    cout << "8. Âûõîä" << "\n\n";
    cout << "ÂÍÈÌÀÍÈÅ, ÏÅÐÅÄ ÂÛÕÎÄÎÌ ÍÅ ÇÀÁÓÄÜÒÅ ÑÎÕÐÀÍÈÒÜ ÈÇÌÅÍÅÍÈß!!!" << "\n\n";
    cout << "Ââåäèòå âàø âûáîð: ";
    cin >> choice;
    while (cin.fail() || choice < 1 || choice > 8) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Íåâåðíûé ââîä. Ïîæàëóéñòà, ââåäèòå ÷èñëî îò 1 äî 8: ";
        cin >> choice;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}
inline void addTask(vector<ToDoItem>& todoList, int& nextId) {
    ToDoItem newItem;
    newItem.id = nextId++; // Ïðèñâàèâàåì óíèêàëüíûé ID
    cout << "Ââåäèòå íàçâàíèå çàäà÷è: ";
    getline(cin, newItem.title);
    cout << "Ââåäèòå äàòó âûïîëíåíèÿ (YYYY-MM-DD): ";
    getline(cin, newItem.dueDate);
    cout << "Ââåäèòå îïèñàíèå çàäà÷è: ";
    getline(cin, newItem.description);
    newItem.isDone = false; // Ïî óìîë÷àíèþ çàäà÷à íå âûïîëíåíà
    todoList.push_back(newItem); // Äîáàâëÿåì íîâóþ çàäà÷ó â ñïèñîê
    cout << "Çàäà÷à äîáàâëåíà!" << endl;
    cout << "Íàæìèòå Enter äëÿ ïðîäîëæåíèÿ...";
    cin.get();
}
inline void showTasks(const vector<ToDoItem>& todoList) {
    clearScreen();
    cout << "--- Ñïèñîê çàäà÷ ---" << endl;
    if (todoList.empty()) {
        cout << "Ñïèñîê çàäà÷ ïóñò." << endl;
    }
    else {
        cout << left << setw(5) << "ID"
            << setw(20) << "Íàçâàíèå"
            << setw(12) << "Äàòà"
            << setw(14) << "Âûïîëíåíî"
            << setw(12) << "Îïèñàíèå"
            << endl;
        cout << "-----------------------------------------------------------------------------------------------------------------------------------" << endl;
        for (const auto& item : todoList) {
            cout << left << setw(5) << item.id
                << setw(20) << item.title
                << setw(12) << item.dueDate
                << setw(14) << (item.isDone ? "Äà" : "Íåò")
                << setw(12) << item.description
                << endl;
        }
    }
    cout << "Íàæìèòå Enter äëÿ ïðîäîëæåíèÿ...";
    cin.get();
}
inline void markTaskAsDone(vector<ToDoItem>& todoList) {
    int id;
    cout << "Ââåäèòå ID çàäà÷è äëÿ âûïîëíåíèÿ: ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (auto& item : todoList) {
        if (item.id == id) {
            item.isDone = !item.isDone; // Èíâåðòèðóåì ñòàòóñ
            cout << "Ñòàòóñ çàäà÷è èçìåíåí." << endl;
            cout << "Íàæìèòå Enter äëÿ ïðîäîëæåíèÿ...";
            cin.get();
            return;
        }
    }
    cout << "Çàäà÷à ñ óêàçàííûì ID íå íàéäåíà." << endl;
    cout << "Íàæìèòå Enter äëÿ ïðîäîëæåíèÿ...";
    cin.get();
}
inline void editTask(vector<ToDoItem>& todoList) {
    int id;
    cout << "Ââåäèòå ID çàäà÷è äëÿ ðåäàêòèðîâàíèÿ: ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string title;
    string dueDate;
    string description;
    for (auto& item : todoList) {
        if (item.id == id) {
            cout << "Ââåäèòå íîâîå íàçâàíèå çàäà÷è (îñòàâüòå ïóñòûì, ÷òîáû íå èçìåíÿòü): ";
            getline(cin, title);
            if (title.empty()) {
                cout << "Íàçâàíèå îñòàâëåíî áåç èçìåíåíèé." << endl;
            }
            else {
                item.title = title;
            }
            cout << "Ââåäèòå íîâóþ äàòó âûïîëíåíèÿ (YYYY-MM-DD, îñòàâüòå ïóñòûì, ÷òîáû íå èçìåíÿòü): ";
            getline(cin, dueDate);
            if (dueDate.empty()) {
                cout << "Äàòà âûïîëíåíèÿ îñòàâëåíà áåç èçìåíåíèé." << endl;
            }
            else {
                item.dueDate = dueDate;
            }
            cout << "Ââåäèòå íîâîå îïèñàíèå çàäà÷è (îñòàâüòå ïóñòûì, ÷òîáû íå èçìåíÿòü): ";
            getline(cin, description);
            if (description.empty()) {
                cout << "Îïèñàíèå îñòàâëåíî áåç èçìåíåíèé." << endl;
            }
            else {
                item.description = description;
            }
            cout << "Çàäà÷à îòðåäàêòèðîâàíà!" << endl;
            cout << "Íàæìèòå Enter äëÿ ïðîäîëæåíèÿ...";
            cin.get();
            return;
        }
    }
    cout << "Çàäà÷à ñ óêàçàííûì ID íå íàéäåíà." << endl;
    cout << "Íàæìèòå Enter äëÿ ïðîäîëæåíèÿ...";
    cin.get();
}
inline void deleteTask(vector<ToDoItem>& todoList) {
    int id;
    cout << "Ââåäèòå ID çàäà÷è äëÿ óäàëåíèÿ: ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (size_t i = 0; i < todoList.size(); ++i) {
        if (todoList[i].id == id) {
            todoList.erase(todoList.begin() + i);
            cout << "Çàäà÷à óäàëåíà." << endl;
            cout << "Íàæìèòå Enter äëÿ ïðîäîëæåíèÿ...";
            cin.get();
            return;
        }
    }
    cout << "Çàäà÷à ñ óêàçàííûì ID íå íàéäåíà." << endl;
    cout << "Íàæìèòå Enter äëÿ ïðîäîëæåíèÿ...";
    cin.get();
}
inline void saveTasksToFile(const vector<ToDoItem>& todoList) {
    ofstream file("todo_list.txt");
    if (file.is_open()) {
        for (const auto& item : todoList) {
            file << item.id << ";" << item.title << ";" << item.dueDate << ";" << item.description << ";" << item.isDone << endl;
        }
        file.close();
        cout << "Çàäà÷è óñïåøíî ñîõðàíåíû â ôàéë." << endl;
        cout << "Íàæìèòå Enter äëÿ ïðîäîëæåíèÿ...";
        cin.get();
    }
    else {
        cerr << "Îøèáêà: Íå óäàëîñü îòêðûòü ôàéë äëÿ ñîõðàíåíèÿ." << endl;
    }
}
inline void loadTasksFromFile(vector<ToDoItem>& todoList, int& nextId) {
    ifstream file("todo_list.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            ToDoItem item;
            string token;

            getline(ss, token, ';');
            item.id = stoi(token);
            getline(ss, item.title, ';');
            getline(ss, item.dueDate, ';');
            getline(ss, item.description, ';');
            getline(ss, token, ';');
            item.isDone = (token == "1" || token == "true"); // Ïðåîáðàçîâàíèå ñòðîêè â bool

            todoList.push_back(item);
            nextId = max(nextId, item.id + 1); // Îáíîâëÿåì nextId
        }
        file.close();
        cout << "Çàäà÷è óñïåøíî çàãðóæåíû èç ôàéëà." << endl;
        cout << "Íàæìèòå Enter äëÿ ïðîäîëæåíèÿ...";
        cin.get();
    }
    else {
        cerr << "Îøèáêà: Íå óäàëîñü îòêðûòü ôàéë äëÿ çàãðóçêè." << endl;
    }
}
