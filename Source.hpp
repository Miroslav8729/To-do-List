#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip> // Для форматирования вывода
#include <limits> // Для std::numeric_limits
#include <sstream> // Для обработки строк

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

// Объявление функций
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
    cout << "Введите имя пользователя: ";
    getline(cin, username);

    // Проверка уникальности
    for (const auto& u : users) {
        if (u.username == username) {
            cout << "Это имя пользователя уже занято." << endl;
            system("pause");
            return false;
        }
    }

    cout << "Введите пароль: ";
    getline(cin, password);

    User newUser{ username, password };
    users.push_back(newUser);
    saveUsersToFile(users);
    cout << "Регистрация прошла успешно!" << endl;
    system("pause");
    return true;
}

inline bool loginUser(const vector<User>& users, string& currentUsername, User*& currentUser) {
    string username, password;
    cout << "Введите имя пользователя: ";
    getline(cin, username);
    cout << "Введите пароль: ";
    getline(cin, password);

    for (const auto& u : users) {
        if (u.username == username && u.password == password) {
            currentUsername = username;
            // Пытаемся найти пользователя для доступа к данным
            // Но поскольку у нас есть вектор пользователей, можно найти по имени
            // и передать указатель
            // Для этого лучше получить его тут
            break;
        }
    }
    // Пока ищем позже. Можно сделать так:
    for (auto& u : const_cast<vector<User>&>(users)) { // не очень корректно, но допустимо для примера
        if (u.username == username && u.password == password) {
            currentUser = &u;
            cout << "Вход выполнен успешно! Добро пожаловать, " << currentUser->username << "." << endl;
            return true;
        }
    }
    cout << "Неверное имя пользователя или пароль." << endl;
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
        cout << "Задачи успешно сохранены." << endl;
        cout << "Нажмите Enter для продолжения...";
        cin.get();
    }
    else {
        cerr << "Ошибка при сохранении задач." << endl;
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
    cout << "1. Добавить задачу" << endl;
    cout << "2. Показать список задач" << endl;
    cout << "3. Выполнить задачу" << endl;
    cout << "4. Редактировать задачу" << endl;
    cout << "5. Удалить задачу" << endl;
    cout << "6. Сохранить изменения" << endl;
    cout << "7. Загрузить задачи из файла" << endl;
    cout << "8. Выход" << "\n\n";
    cout << "ВНИМАНИЕ, ПЕРЕД ВЫХОДОМ НЕ ЗАБУДЬТЕ СОХРАНИТЬ ИЗМЕНЕНИЯ!!! 0" << "\n\n";
    cout << "Введите ваш выбор: ";
    cin >> choice;
    while (cin.fail() || choice < 1 || choice > 8) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Неверный ввод. Пожалуйста, введите число от 1 до 8: ";
        cin >> choice;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}
inline void addTask(vector<ToDoItem>& todoList, int& nextId) {
    ToDoItem newItem;
    newItem.id = nextId++; // Присваиваем уникальный ID
    cout << "Введите название задачи: ";
    getline(cin, newItem.title);
    cout << "Введите дату выполнения (YYYY-MM-DD): ";
    getline(cin, newItem.dueDate);
    cout << "Введите описание задачи: ";
    getline(cin, newItem.description);
    newItem.isDone = false; // По умолчанию задача не выполнена
    todoList.push_back(newItem); // Добавляем новую задачу в список
    cout << "Задача добавлена!" << endl;
    cout << "Нажмите Enter для продолжения...";
    cin.get();
}
inline void showTasks(const vector<ToDoItem>& todoList) {
    clearScreen();
    cout << "--- Список задач ---" << endl;
    if (todoList.empty()) {
        cout << "Список задач пуст." << endl;
    }
    else {
        cout << left << setw(5) << "ID"
            << setw(20) << "Название"
            << setw(12) << "Дата"
            << setw(14) << "Выполнено"
            << setw(12) << "Описание"
            << endl;
        cout << "-----------------------------------------------------------------------------------------------------------------------------------" << endl;
        for (const auto& item : todoList) {
            cout << left << setw(5) << item.id
                << setw(20) << item.title
                << setw(12) << item.dueDate
                << setw(14) << (item.isDone ? "Да" : "Нет")
                << setw(12) << item.description
                << endl;
        }
    }
    cout << "Нажмите Enter для продолжения...";
    cin.get();
}
inline void markTaskAsDone(vector<ToDoItem>& todoList) {
    int id;
    cout << "Введите ID задачи для выполнения: ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (auto& item : todoList) {
        if (item.id == id) {
            item.isDone = !item.isDone; // Инвертируем статус
            cout << "Статус задачи изменен." << endl;
            cout << "Нажмите Enter для продолжения...";
            cin.get();
            return;
        }
    }
    cout << "Задача с указанным ID не найдена." << endl;
    cout << "Нажмите Enter для продолжения...";
    cin.get();
}
inline void editTask(vector<ToDoItem>& todoList) {
    int id;
    cout << "Введите ID задачи для редактирования: ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string title;
    string dueDate;
    string description;
    for (auto& item : todoList) {
        if (item.id == id) {
            cout << "Введите новое название задачи (оставьте пустым, чтобы не изменять): ";
            getline(cin, title);
            if (title.empty()) {
                cout << "Название оставлено без изменений." << endl;
            }
            else {
                item.title = title;
            }
            cout << "Введите новую дату выполнения (YYYY-MM-DD, оставьте пустым, чтобы не изменять): ";
            getline(cin, dueDate);
            if (dueDate.empty()) {
                cout << "Дата выполнения оставлена без изменений." << endl;
            }
            else {
                item.dueDate = dueDate;
            }
            cout << "Введите новое описание задачи (оставьте пустым, чтобы не изменять): ";
            getline(cin, description);
            if (description.empty()) {
                cout << "Описание оставлено без изменений." << endl;
            }
            else {
                item.description = description;
            }
            cout << "Задача отредактирована!" << endl;
            cout << "Нажмите Enter для продолжения...";
            cin.get();
            return;
        }
    }
    cout << "Задача с указанным ID не найдена." << endl;
    cout << "Нажмите Enter для продолжения...";
    cin.get();
}
inline void deleteTask(vector<ToDoItem>& todoList) {
    int id;
    cout << "Введите ID задачи для удаления: ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (size_t i = 0; i < todoList.size(); ++i) {
        if (todoList[i].id == id) {
            todoList.erase(todoList.begin() + i);
            cout << "Задача удалена." << endl;
            cout << "Нажмите Enter для продолжения...";
            cin.get();
            return;
        }
    }
    cout << "Задача с указанным ID не найдена." << endl;
    cout << "Нажмите Enter для продолжения...";
    cin.get();
}
inline void saveTasksToFile(const vector<ToDoItem>& todoList) {
    ofstream file("todo_list.txt");
    if (file.is_open()) {
        for (const auto& item : todoList) {
            file << item.id << ";" << item.title << ";" << item.dueDate << ";" << item.description << ";" << item.isDone << endl;
        }
        file.close();
        cout << "Задачи успешно сохранены в файл." << endl;
        cout << "Нажмите Enter для продолжения...";
        cin.get();
    }
    else {
        cerr << "Ошибка: Не удалось открыть файл для сохранения." << endl;
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
            item.isDone = (token == "1" || token == "true"); // Преобразование строки в bool

            todoList.push_back(item);
            nextId = max(nextId, item.id + 1); // Обновляем nextId
        }
        file.close();
        cout << "Задачи успешно загружены из файла." << endl;
        cout << "Нажмите Enter для продолжения...";
        cin.get();
    }
    else {
        cerr << "Ошибка: Не удалось открыть файл для загрузки." << endl;
    }
}