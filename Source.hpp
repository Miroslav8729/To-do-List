#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip> // ��� �������������� ������
#include <limits> // ��� std::numeric_limits
#include <sstream> // ��� ��������� �����

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

// ���������� �������
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
    cout << "������� ��� ������������: ";
    getline(cin, username);

    // �������� ������������
    for (const auto& u : users) {
        if (u.username == username) {
            cout << "��� ��� ������������ ��� ������." << endl;
            system("pause");
            return false;
        }
    }

    cout << "������� ������: ";
    getline(cin, password);

    User newUser{ username, password };
    users.push_back(newUser);
    saveUsersToFile(users);
    cout << "����������� ������ �������!" << endl;
    system("pause");
    return true;
}

inline bool loginUser(const vector<User>& users, string& currentUsername, User*& currentUser) {
    string username, password;
    cout << "������� ��� ������������: ";
    getline(cin, username);
    cout << "������� ������: ";
    getline(cin, password);

    for (const auto& u : users) {
        if (u.username == username && u.password == password) {
            currentUsername = username;
            // �������� ����� ������������ ��� ������� � ������
            // �� ��������� � ��� ���� ������ �������������, ����� ����� �� �����
            // � �������� ���������
            // ��� ����� ����� �������� ��� ���
            break;
        }
    }
    // ���� ���� �����. ����� ������� ���:
    for (auto& u : const_cast<vector<User>&>(users)) { // �� ����� ���������, �� ��������� ��� �������
        if (u.username == username && u.password == password) {
            currentUser = &u;
            cout << "���� �������� �������! ����� ����������, " << currentUser->username << "." << endl;
            return true;
        }
    }
    cout << "�������� ��� ������������ ��� ������." << endl;
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
        cout << "������ ������� ���������." << endl;
        cout << "������� Enter ��� �����������...";
        cin.get();
    }
    else {
        cerr << "������ ��� ���������� �����." << endl;
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
    cout << "1. �������� ������" << endl;
    cout << "2. �������� ������ �����" << endl;
    cout << "3. ��������� ������" << endl;
    cout << "4. ������������� ������" << endl;
    cout << "5. ������� ������" << endl;
    cout << "6. ��������� ���������" << endl;
    cout << "7. ��������� ������ �� �����" << endl;
    cout << "8. �����" << "\n\n";
    cout << "��������, ����� ������� �� �������� ��������� ���������!!! 0" << "\n\n";
    cout << "������� ��� �����: ";
    cin >> choice;
    while (cin.fail() || choice < 1 || choice > 8) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "�������� ����. ����������, ������� ����� �� 1 �� 8: ";
        cin >> choice;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}
inline void addTask(vector<ToDoItem>& todoList, int& nextId) {
    ToDoItem newItem;
    newItem.id = nextId++; // ����������� ���������� ID
    cout << "������� �������� ������: ";
    getline(cin, newItem.title);
    cout << "������� ���� ���������� (YYYY-MM-DD): ";
    getline(cin, newItem.dueDate);
    cout << "������� �������� ������: ";
    getline(cin, newItem.description);
    newItem.isDone = false; // �� ��������� ������ �� ���������
    todoList.push_back(newItem); // ��������� ����� ������ � ������
    cout << "������ ���������!" << endl;
    cout << "������� Enter ��� �����������...";
    cin.get();
}
inline void showTasks(const vector<ToDoItem>& todoList) {
    clearScreen();
    cout << "--- ������ ����� ---" << endl;
    if (todoList.empty()) {
        cout << "������ ����� ����." << endl;
    }
    else {
        cout << left << setw(5) << "ID"
            << setw(20) << "��������"
            << setw(12) << "����"
            << setw(14) << "���������"
            << setw(12) << "��������"
            << endl;
        cout << "-----------------------------------------------------------------------------------------------------------------------------------" << endl;
        for (const auto& item : todoList) {
            cout << left << setw(5) << item.id
                << setw(20) << item.title
                << setw(12) << item.dueDate
                << setw(14) << (item.isDone ? "��" : "���")
                << setw(12) << item.description
                << endl;
        }
    }
    cout << "������� Enter ��� �����������...";
    cin.get();
}
inline void markTaskAsDone(vector<ToDoItem>& todoList) {
    int id;
    cout << "������� ID ������ ��� ����������: ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (auto& item : todoList) {
        if (item.id == id) {
            item.isDone = !item.isDone; // ����������� ������
            cout << "������ ������ �������." << endl;
            cout << "������� Enter ��� �����������...";
            cin.get();
            return;
        }
    }
    cout << "������ � ��������� ID �� �������." << endl;
    cout << "������� Enter ��� �����������...";
    cin.get();
}
inline void editTask(vector<ToDoItem>& todoList) {
    int id;
    cout << "������� ID ������ ��� ��������������: ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string title;
    string dueDate;
    string description;
    for (auto& item : todoList) {
        if (item.id == id) {
            cout << "������� ����� �������� ������ (�������� ������, ����� �� ��������): ";
            getline(cin, title);
            if (title.empty()) {
                cout << "�������� ��������� ��� ���������." << endl;
            }
            else {
                item.title = title;
            }
            cout << "������� ����� ���� ���������� (YYYY-MM-DD, �������� ������, ����� �� ��������): ";
            getline(cin, dueDate);
            if (dueDate.empty()) {
                cout << "���� ���������� ��������� ��� ���������." << endl;
            }
            else {
                item.dueDate = dueDate;
            }
            cout << "������� ����� �������� ������ (�������� ������, ����� �� ��������): ";
            getline(cin, description);
            if (description.empty()) {
                cout << "�������� ��������� ��� ���������." << endl;
            }
            else {
                item.description = description;
            }
            cout << "������ ���������������!" << endl;
            cout << "������� Enter ��� �����������...";
            cin.get();
            return;
        }
    }
    cout << "������ � ��������� ID �� �������." << endl;
    cout << "������� Enter ��� �����������...";
    cin.get();
}
inline void deleteTask(vector<ToDoItem>& todoList) {
    int id;
    cout << "������� ID ������ ��� ��������: ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (size_t i = 0; i < todoList.size(); ++i) {
        if (todoList[i].id == id) {
            todoList.erase(todoList.begin() + i);
            cout << "������ �������." << endl;
            cout << "������� Enter ��� �����������...";
            cin.get();
            return;
        }
    }
    cout << "������ � ��������� ID �� �������." << endl;
    cout << "������� Enter ��� �����������...";
    cin.get();
}
inline void saveTasksToFile(const vector<ToDoItem>& todoList) {
    ofstream file("todo_list.txt");
    if (file.is_open()) {
        for (const auto& item : todoList) {
            file << item.id << ";" << item.title << ";" << item.dueDate << ";" << item.description << ";" << item.isDone << endl;
        }
        file.close();
        cout << "������ ������� ��������� � ����." << endl;
        cout << "������� Enter ��� �����������...";
        cin.get();
    }
    else {
        cerr << "������: �� ������� ������� ���� ��� ����������." << endl;
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
            item.isDone = (token == "1" || token == "true"); // �������������� ������ � bool

            todoList.push_back(item);
            nextId = max(nextId, item.id + 1); // ��������� nextId
        }
        file.close();
        cout << "������ ������� ��������� �� �����." << endl;
        cout << "������� Enter ��� �����������...";
        cin.get();
    }
    else {
        cerr << "������: �� ������� ������� ���� ��� ��������." << endl;
    }
}