#include "Source.hpp"

int main() {
    setlocale(LC_ALL, "rus");
    vector<User> users;
    loadUsersFromFile(users);
    User* currentUser = nullptr; // Указатель на текущего пользователя

    string currentUsername; // Для хранения имени текущего пользователя

    int authChoice;
    do {
        clearScreen();
        cout << "--- Добро пожаловать ---" << endl;
        cout << "1. Войти" << endl;
        cout << "2. Зарегистрироваться" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";
        cin >> authChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (authChoice == 1) {
            if (loginUser(users, currentUsername, currentUser)) { // передаем указатель
                break;
            }
        }
        else if (authChoice == 2) {
            if (registerUser(users)) {
                loadUsersFromFile(users); // После регистрации получить свежий список
            }
        }
        else if (authChoice == 0) {
            return 0;
        }
        else {
            cout << "Некорректный выбор." << endl;
            cin.get();
        }
    } while (true);

    if (currentUser == nullptr) {
        cout << "Ошибка входа." << endl;
        return 0;
    }

    // Загружаем задачи текущего пользователя
    vector<ToDoItem> todoList;
    int nextId = 1;
    loadTasksFromFile(todoList, nextId, currentUser->username);

    int choice;
    do {
        choice = showRenderMenu();
        switch (choice) {
        case 1:
            addTask(todoList, nextId);
            break;
        case 2:
            renderTaskTable(todoList);
            break;
        case 3:
            markTaskAsDone(todoList);
            break;
        case 4:
            editTask(todoList);
            break;
        case 5:
            deleteTask(todoList);
            break;
        case 6:
            saveTasksToFile(todoList, currentUser->username);
            break;
        case 7:
            loadTasksFromFile(todoList, nextId, currentUser->username);
            break;
        case 8:
            cout << "Выход. До свидания!\n";
            break;
        default:
            cout << "Неверный выбор.\n";
        }
        cout << "Нажмите Enter чтобы продолжить...";
        cin.get();
    } while (choice != 8);

    return 0;
}
