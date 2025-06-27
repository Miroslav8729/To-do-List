#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip> // Для форматирования вывода
#include <limits>   // Для std::numeric_limits
#include <sstream> // Для обработки строк
#include <algorithm> // Для std::max

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
            // Ищем указатель
            for (auto& userRef : const_cast<vector<User>&>(users)) {
                if (userRef.username == username && userRef.password == password) {
                    currentUser = &userRef;
                    cout << "Вход выполнен успешно! Добро пожаловать, " << currentUser->username << "." << endl;
                    return true;
                }
            }
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
    string filename = username + "_tasks.dat"; // бинарный файл
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        todoList.clear();
        nextId = 1;
        while (file.peek() != EOF) {
            ToDoItem item;

            // Читаем ID
            file.read(reinterpret_cast<char*>(&item.id), sizeof(item.id));
            if (file.gcount() != sizeof(item.id)) break;

            size_t size;
            // Читаем название
            file.read(reinterpret_cast<char*>(&size), sizeof(size));
            string title(size, '\0');
            file.read(&title[0], size);
            item.title = title;

            // Читаем дату
            file.read(reinterpret_cast<char*>(&size), sizeof(size));
            string date(size, '\0');
            file.read(&date[0], size);
            item.dueDate = date;

            // Читаем описание
            file.read(reinterpret_cast<char*>(&size), sizeof(size));
            string description(size, '\0');
            file.read(&description[0], size);
            item.description = description;

            // Читаем статус
            file.read(reinterpret_cast<char*>(&item.isDone), sizeof(item.isDone));

            todoList.push_back(item);
            if (item.id >= nextId) nextId = item.id + 1;
        }
        file.close();
        cout << "Задачи успешно загружены из файла." << endl;
        cout << "Нажмите Enter для продолжения...";
        cin.get();
    }
    else {
        cout << "Файл задач пользователя не найден или поврежден. Будет создан новый." << endl;
    }
}

inline void saveTasksToFile(const vector<ToDoItem>& todoList, const string& username) {
    string filename = username + "_tasks.dat"; // бинарный файл
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        for (const auto& item : todoList) {
            // Записываем id
            file.write(reinterpret_cast<const char*>(&item.id), sizeof(item.id));

            size_t size;

            // Записываем title
            size = item.title.size();
            file.write(reinterpret_cast<const char*>(&size), sizeof(size));
            file.write(&item.title[0], size);

            // Записываем dueDate
            size = item.dueDate.size();
            file.write(reinterpret_cast<const char*>(&size), sizeof(size));
            file.write(&item.dueDate[0], size);

            // Записываем description
            size = item.description.size();
            file.write(reinterpret_cast<const char*>(&size), sizeof(size));
            file.write(&item.description[0], size);

            // Записываем isDone
            file.write(reinterpret_cast<const char*>(&item.isDone), sizeof(item.isDone));
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


inline int showRenderMenu() {
    vector<string> options = {
        "Добавить задачу",
        "Показать список задач",
        "Выполнить задачу",
        "Редактировать задачу",
        "Удалить задачу",
        "Сохранить изменения",
        "Загрузить задачи из файла",
        "Выход"
    };
    const int width = 80;
    // Рамка с заголовком
    cout << "+" << string(width - 2, '-') << "+" << endl;
    cout << "| " << left << setw(width - 4) << "Меню" << " |" << endl;
    cout << "+" << string(width - 2, '-') << "+" << endl;

    for (size_t i = 0; i < options.size(); ++i) {
        cout << "| " << left << setw(width - 4)
            << (to_string(i + 1) + ". " + options[i]) << " |" << endl;
    }
    cout << "+" << string(width - 2, '-') << "+" << endl;

    int choice;
    cout << "Введите ваш выбор (1-" << options.size() << "): ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    while (cin.fail() || choice < 1 || choice > options.size()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Некорректный ввод. Попробуйте снова: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return choice;
}
vector<string> splitString(const string& str, size_t max_length) {
    vector<string> lines;
    size_t start = 0;
    while (start < str.size()) {
        size_t len = min(max_length, str.size() - start);
        lines.push_back(str.substr(start, len));
        start += len;
    }
    return lines;
}

void renderTaskTable(const vector<ToDoItem>& todoList) {
    const int widthID = 5;
    const int widthTitle = 20;
    const int widthDate = 12;
    const int widthStatus = 10;
    const int widthDesc = 50; // увеличиваем ширину для текста описания
    const int separatorSize = 6; // разделитель и пробелы
    const int totalWidth = widthID + widthTitle + widthDate + widthStatus + widthDesc + separatorSize;

    // Заголовок таблицы
    cout << "+" << string(totalWidth - 2, '-') << "+" << endl;
    cout << "| " << left << setw(widthID) << "ID"
        << "| " << left << setw(widthTitle) << "Название"
        << "| " << left << setw(widthDate) << "Дата"
        << "| " << left << setw(widthStatus) << "Статус"
        << "| " << left << setw(widthDesc) << "Описание"
        << " |" << endl;
    cout << "+" << string(totalWidth - 2, '-') << "+" << endl;

    // Обработка каждой задачи
    for (const auto& item : todoList) {
        // разбиваем описание на линии
        vector<string> descLines = splitString(item.description, widthDesc);
        size_t maxLines = descLines.size();

        // выводим в цикле
        for (size_t i = 0; i < maxLines; ++i) {
            cout << "| ";
            if (i == 0) {
                cout << left << setw(widthID) << item.id
                    << "| " << left << setw(widthTitle) << item.title.substr(0, widthTitle - 1)
                    << "| " << left << setw(widthDate) << item.dueDate
                    << "| " << left << setw(widthStatus) << (item.isDone ? "Выполнена" : "Нет")
                    << "| " << left << setw(widthDesc) << descLines[i] << " |" << endl;
            }
            else {
                // для остальных строк описание
                cout << left << setw(widthID) << " "
                    << "| " << left << setw(widthTitle) << " "
                    << "| " << left << setw(widthDate) << " "
                    << "| " << left << setw(widthStatus) << " "
                    << "| " << left << setw(widthDesc) << descLines[i] << " |" << endl;
            }
        }
    }
    cout << "+" << string(totalWidth - 2, '-') << "+" << endl;
}


//----------------------------------------------
//  Функции для работы с бинарными файлами
//----------------------------------------------
void loadUsersFromBinary(vector<User>& users) {
    ifstream file("users.dat", ios::binary);
    if (file.is_open()) {
        User user;
        while (file.read(reinterpret_cast<char*>(&user), sizeof(User))) {
            users.push_back(user);
        }
        file.close();
    }
}

void saveUsersToBinary(const vector<User>& users) {
    ofstream file("users.dat", ios::binary);
    if (file.is_open()) {
        for (const auto& user : users) {
            file.write(reinterpret_cast<const char*>(&user), sizeof(User));
        }
        file.close();
    }
}

void loadTasksFromBinary(vector<ToDoItem>& todoList, int& nextId, const string& username) {
    string filename = username + "_tasks.dat";
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        ToDoItem item;
        while (file.read(reinterpret_cast<char*>(&item), sizeof(ToDoItem))) {
            todoList.push_back(item);
            if (item.id >= nextId) nextId = item.id + 1;
        }
        file.close();
    }
}

void saveTasksToBinary(const vector<ToDoItem>& todoList, const string& username) {
    string filename = username + "_tasks.dat";
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        for (const auto& item : todoList) {
            file.write(reinterpret_cast<const char*>(&item), sizeof(ToDoItem));
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
