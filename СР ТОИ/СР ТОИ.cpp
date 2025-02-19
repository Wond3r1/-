#include <iostream>
#include <string>
#include <sstream> // Для функции ввода числа с клавиатуры
#include <fstream> // Для функции считывания информации с файла
#include <windows.h>
#include "BestMMAFighters.h"
#include "IndexEntry.h"
#include "BinaryTree.h"
#include "FighterList.h"
using namespace std;


/// <summary>
/// Функция для проверки правильно введенного символа в строку
/// </summary>
/// <param name="c"></param>
/// <returns></returns>
bool isValidChar(char c) {
    // Проверяем, является ли символ буквой русского или английского алфавита или пробелом
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
        (c >= 'А' && c <= 'Я') || (c >= 'а' && c <= 'я') ||
        (c == ' '));
}


/// <summary>
/// Функция для корректного ввода строки с клавиатуры
/// </summary>
/// <param name="prompt"></param>
/// <returns></returns>
string getStringInput(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;

        getline(cin, input);

        // Проверяем каждый символ в строке
        bool isValid = true;
        for (char c : input) {
            if (!isValidChar(c)) {
                isValid = false;
                break;
            }
        }

        if (isValid) {
            return input;
        }
        else {
            cout << "\nОшибка: строка должна содержать только буквы русского и английского алфавита и пробелы.\n";
        }
    }
}


/// <summary>
/// Функция для ввода чисел с клавиатуры
/// </summary>
/// <param name="lowerbound"></param>
/// <param name="upperbound"></param>
/// <returns></returns>
int getIntNumber(int lowerbound, int upperbound) {
    int num;
    bool isConvert;
    string input;

    do {
        cout << "\nВведите число от " << lowerbound << " до " << upperbound << "\n" << endl;
        getline(cin, input);

        stringstream ss(input); // Парсинг строки

        // Рассмотрение случаев неправильного ввода числа
        if (ss >> num) { 
            isConvert = true;

            if (num < lowerbound || num > upperbound) {
                cout << "\nВведено число не из диапазона" << endl;
                isConvert = false; 
            }
        }
        else {
            isConvert = false;
            cout << "\nЧисло введено неверно. Повторите ввод" << endl;
        }

    } while (!isConvert);

    return num;
}


/// <summary>
/// Функция для считывания информации о бойцах с файла
/// </summary>
/// <param name="filename"></param>
/// <param name="fighters"></param>
/// <param name="numFighters"></param>
void readFightersFromFile(const string& filename, BestMMAFighters fighters[], int& numFighters) {
    ifstream file(filename); // Открываем файл для чтения
    if (!file.is_open()) {
        cerr << "\nОшибка: не удалось открыть файл " << filename << endl;
        return;
    }

    numFighters = 0; // Счетчик бойцов
    string line; // Переменная для хранения строки из файла


    while (getline(file, line)) { // Читаем файл построчно
        if (line.empty()) {
            continue; // Пропускаем пустые строки
        }

        istringstream iss(line); // Создаем поток для разбора строки
        BestMMAFighters fighter;

        // Разбираем строку на отдельные данные
        if (iss >> fighter.firstName >> fighter.lastName >> fighter.birthYear >> fighter.wins >> fighter.losses >> fighter.weightClass) {
            fighters[numFighters] = fighter;
            numFighters++;
        }
        else {
            cerr << "\nОшибка: некорректный формат строки в файле: " << line << endl;
        }
    }

    file.close(); // Закрываем файл
}


//------------------------------------
// Задание 1
//------------------------------------


/// <summary>
/// Функция ввода данных в массив
/// </summary>
/// <param name="fighters"></param>
/// <param name="numFighters"></param>
void inputFighters(BestMMAFighters fighters[], int numFighters) { 
    // Ввод информации о всех бойцах
    for (int i = 0; i < numFighters; ++i) {
        cout << "Ввод данных о " << i + 1 << " бойце:" << endl;
        fighters[i].firstName = getStringInput("\nИмя: ");

        fighters[i].lastName = getStringInput("\nФамилия: ");

        cout << "\nГод рождения: ";
        fighters[i].birthYear = getIntNumber(1900, 2025);

        cout << "\nКоличество побед: ";
        fighters[i].wins = getIntNumber(0, 100);

        cout << "\nКоличество поражений: ";
        fighters[i].losses = getIntNumber(0, 100); 

        fighters[i].weightClass = getStringInput("\nВесовая категория: ");
        system("cls");
    }
}


/// <summary>
/// Функция для создания индекса по фамилии в порядке возрастания
/// </summary>
/// <param name="fighters"></param>
/// <param name="numFighters"></param>
/// <param name="index"></param>
void createLastNameIndex(const BestMMAFighters fighters[], int numFighters, IndexEntry index[]) {
    for (int i = 0; i < numFighters; ++i) {
        index[i].keyString = fighters[i].lastName;
        index[i].originalIndex = i;
    }

    // Сортировка методом вставки по возрастанию фамилии
    for (int i = 1; i < numFighters; ++i) {
        IndexEntry key = index[i];
        int j = i - 1;
        while (j >= 0 && index[j].keyString > key.keyString) {
            index[j + 1] = index[j];
            j--;
        }
        index[j + 1] = key;
    }
}


/// <summary>
/// Функция для создания индекса по количеству побед в порядке убывания
/// </summary>
/// <param name="fighters"></param>
/// <param name="numFighters"></param>
/// <param name="index"></param>
void createWinsIndex(const BestMMAFighters fighters[], int numFighters, IndexEntry index[]) {
    for (int i = 0; i < numFighters; ++i) {
        index[i] = IndexEntry(fighters[i].wins, i);
    }

    // Сортировка методом вставки по убыванию количества побед
    for (int i = 1; i < numFighters; ++i) {
        IndexEntry key = index[i];
        int j = i - 1;
        while (j >= 0 && index[j].keyInt < key.keyInt) {
            index[j + 1] = index[j];
            j--;
        }
        index[j + 1] = key;
    }
}


/// <summary>
/// Функция для вывода данных по индексу в порядке убывания фамилий
/// </summary>
/// <param name="fighters"></param>
/// <param name="index"></param>
/// <param name="numFighters"></param>
void printFightersByLastNamesAscending(const BestMMAFighters fighters[], const IndexEntry index[], int numFighters) {
    cout << "Сортировка бойцов по возрастанию фамилии в алфавитном порядке:" << endl;
    for (int i = 0; i < numFighters; ++i) {
        const BestMMAFighters& fighter = fighters[index[i].originalIndex];
        cout << "\nИмя и фамилия: " << fighter.firstName << " " << fighter.lastName
            << ", Год рождения: " << fighter.birthYear
            << ", Количество побед: " << fighter.wins
            << ", Количество поражений: " << fighter.losses
            << ", Весовая категория: " << fighter.weightClass;
        cout << endl;
    }
}


/// <summary>
/// Функция для вывода данных по индексу в порядке убыванию количества побед
/// </summary>
/// <param name="fighters"></param>
/// <param name="index"></param>
/// <param name="numFighters"></param>
void printFightersByWinsNumberDescending(const BestMMAFighters fighters[], const IndexEntry index[], int numFighters) {
    cout << "Сортировка бойцов по убыванию количества побед:" << endl;
    for (int i = 0; i < numFighters; ++i) {
        const BestMMAFighters& fighter = fighters[index[i].originalIndex];
        cout << "\nИмя и фамилия: " << fighter.firstName << " " << fighter.lastName
            << ", Год рождения: " << fighter.birthYear
            << ", Количество побед: " << fighter.wins
            << ", Количество поражений: " << fighter.losses
            << ", Весовая категория: " << fighter.weightClass;
        cout << endl;
    }
}


/// <summary>
/// Функция вывода данных бойца по индексу из исходного массива
/// </summary>
/// <param name="fighters"></param>
/// <param name="index"></param>
void printFighterInfo(const BestMMAFighters fighters[], int index) {
    // Вывод информации о бойце
    if (index != -1) {
        const BestMMAFighters& fighter = fighters[index];
        cout << "\nИмя и фамилия: " << fighter.firstName << " " << fighter.lastName
            << ", Год рождения: " << fighter.birthYear
            << ", Количество побед: " << fighter.wins
            << ", Количество поражений: " << fighter.losses
            << ", Весовая категория: " << fighter.weightClass << endl;
    }
    else {
        cout << "Боец не найден" << endl;
    }
}


/// <summary>
/// Функция вывода данных о всех бойцах массива
/// </summary>
/// <param name="fighters"></param>
/// <param name="numFighters"></param>
void printInfoAboutFighters(const BestMMAFighters fighters[], int numFighters) {
    // Вывод информации о всех бойцах массива
    for (int i = 0; i < numFighters; ++i) {
        cout << "\nИмя и фамилия: " << fighters[i].firstName << " " << fighters[i].lastName
            << ", Год рождения: " << fighters[i].birthYear
            << ", Количество побед: " << fighters[i].wins
            << ", Количество поражений: " << fighters[i].losses
            << ", Весовая категория: " << fighters[i].weightClass << endl;
    }
}


/// <summary>
/// Рекурсивная функция бинарного поиска по фамилии
/// </summary>
/// <param name="index"></param>
/// <param name="low"></param>
/// <param name="high"></param>
/// <param name="key"></param>
/// <returns></returns>
int binarySearchLastNameRecur(const IndexEntry index[], int low, int high, const string& searchLastName) {
    if (low > high)
        return -1;

    // Поиск индекса, по которому расположена заданная фамилия
    int mid = low + (high - low) / 2;
    if (index[mid].keyString == searchLastName) {
        return index[mid].originalIndex;
    }
    else if (index[mid].keyString < searchLastName)
        return binarySearchLastNameRecur(index, mid + 1, high, searchLastName);
    else
        return binarySearchLastNameRecur(index, low, mid - 1, searchLastName);
}

/// <summary>
/// Итерационная функция бинарного поиска по количеству побед
/// </summary>
/// <param name="index"></param>
/// <param name="numFighters"></param>
/// <param name="searchLastName"></param>
/// <returns></returns>
int binarySearchWinsIter(const IndexEntry index[], int numFighters, int searchWins) {

    int low = 0;
    int high = numFighters - 1;

    // Поиск индекса, по которому расположено заданное количество побед
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (index[mid].keyInt == searchWins)
            return index[mid].originalIndex;
        else if (index[mid].keyInt < searchWins)
            high = mid - 1;
        else
            low = mid + 1;
    }

    return -1;
}


/// <summary>
/// Функция для редактирования записи в массиве
/// </summary>
/// <param name="fighters"></param>
/// <param name="lastNameIndex"></param>
/// <param name="winsIndex"></param>
/// <param name="numFighters"></param>
void editFighter(BestMMAFighters fighters[], IndexEntry lastNameIndex[], IndexEntry winsIndex[], int numFighters) {
    cout << "Ввод номера бойца для изменения фамилии и количества побед: ";
    int numOfFighter = getIntNumber(1, numFighters);

    BestMMAFighters& fighter = fighters[numOfFighter - 1];

    cout << "\nТекущие данные:" << endl;
    printFighterInfo(fighters, numOfFighter - 1);

    cout << "\nВведите новые данные (фамилию и количество побед):" << endl;
    fighter.lastName = getStringInput("\nВвод фамилии : ");
    cout << "\nВвод количества побед: ";
    fighter.wins = getIntNumber(0, 100);

    cout << "\nБоец после изменения фамилии и количества побед:\n" << endl;
    printFighterInfo(fighters, numOfFighter - 1);

    // Обновление индексов
    createLastNameIndex(fighters, numFighters, lastNameIndex);
    createWinsIndex(fighters, numFighters, winsIndex);
}


/// <summary>
/// Функция удаления записи из массива по фамилии или по количеству побед
/// </summary>
/// <param name="fighters"></param>
/// <param name="lastNameIndex"></param>
/// <param name="winsIndex"></param>
/// <param name="numFighters"></param>
/// <param name="choice - Для выбора удаления (по фамилии или количеству побед)"></param>
void deleteFighterByLastNameOrByWins(BestMMAFighters fighters[], IndexEntry lastNameIndex[], IndexEntry winsIndex[], int& numFighters, int choice) {
    string lastNameToDelete;
    int indexToDelete = 0;
    int check = numFighters;
    if (choice == 1) {
        lastNameToDelete = getStringInput("Введите фамилию бойца, информацию о котором хотите удалить: ");
        do {       
            indexToDelete = binarySearchLastNameRecur(lastNameIndex, 0, numFighters - 1, lastNameToDelete);
            // Сдвиг всех элементов после удаленного на одну позицию назад
            if (indexToDelete != -1) {
                for (int i = indexToDelete; i < numFighters - 1; ++i) {
                    fighters[i] = fighters[i + 1];
                }
                numFighters--;
                createLastNameIndex(fighters, numFighters, lastNameIndex);
                createWinsIndex(fighters, numFighters, winsIndex);
            }
            
        } while (indexToDelete != -1);
    }
    else {
        cout << "Введите количество побед бойца, информацию о котором хотите удалить: ";
        int searchWins = getIntNumber(0, 100);
        indexToDelete = binarySearchWinsIter(winsIndex, numFighters, searchWins);
        do {
            indexToDelete = binarySearchWinsIter(winsIndex, numFighters, searchWins);
            // Сдвиг всех элементов после удаленного на одну позицию назад
            if (indexToDelete != -1) {
                for (int i = indexToDelete; i < numFighters - 1; ++i) {
                    fighters[i] = fighters[i + 1];
                }
                numFighters--;
                createLastNameIndex(fighters, numFighters, lastNameIndex);
                createWinsIndex(fighters, numFighters, winsIndex);
            }

        } while (indexToDelete != -1);
    }

    if (indexToDelete == -1 && numFighters == check) {
        cout << "\nБоец не найден." << endl;
        return;
    }

    if (numFighters == 0) {
        cout << "\nМассив остался пустым" << endl;
        return;
    }

    cout << "\nСписок бойцов после удаления:" << endl;
    printInfoAboutFighters(fighters, numFighters);
}


//------------------------------------
// Задание 2
//------------------------------------



/// <summary>
/// Функция создания нового узла дерева
/// </summary>
/// <param name="keyLastName"></param>
/// <param name="keyWins"></param>
/// <param name="dataIndex"></param>
/// <returns></returns>
BinaryTree* createTreeNode(string keyLastName, int keyWins, int dataIndex) {
    // Создание узла дерева
    BinaryTree* newNode = new BinaryTree;
    newNode->keyLastName = keyLastName;
    newNode->keyWins = keyWins;
    newNode->dataIndex = dataIndex;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}


/// <summary>
/// Функция создания дерева с сортировкой фамилий по возрастанию
/// </summary>
/// <param name="root"></param>
/// <param name="lastName"></param>
/// <param name="wins"></param>
/// <param name="dataIndex"></param>
/// <returns></returns>
BinaryTree* insertNodeByLastName(BinaryTree* root, string lastName, int wins, int dataIndex) {
    // Вставка узла в дерево с сортировкой по фамилии
    if (root == nullptr) {
        return createTreeNode(lastName, wins, dataIndex);
    }
    if (lastName < root->keyLastName) {
        root->left = insertNodeByLastName(root->left, lastName, wins, dataIndex);
    }
    else {
        root->right = insertNodeByLastName(root->right, lastName, wins, dataIndex);
    }
    return root;
}


/// <summary>
/// Функция создания дерева с сортировкой количества побед по убыванию
/// </summary>
/// <param name="root"></param>
/// <param name="wins"></param>
/// <param name="lastName"></param>
/// <param name="dataIndex"></param>
/// <returns></returns>
BinaryTree* insertNodeByWins(BinaryTree* root, int wins, string lastName, int dataIndex) {
    // Вставка узла в дерево с сортировкой по фамилии
    if (root == nullptr) {
        return createTreeNode(lastName, wins, dataIndex);
    }
    if (wins > root->keyWins) {
        root->left = insertNodeByWins(root->left, wins, lastName, dataIndex);
    }
    else {
        root->right = insertNodeByWins(root->right, wins, lastName, dataIndex);
    }
    return root;
}


/// <summary>
/// Функция вывода дерева в порядке возрастания фамилий
/// </summary>
/// <param name="root"></param>
/// <param name="fighters"></param>
void printAscendingByLastName(BinaryTree* root, const BestMMAFighters fighters[]) {
    if (root != nullptr) {
        printAscendingByLastName(root->left, fighters);
        printFighterInfo(fighters, root->dataIndex);
        printAscendingByLastName(root->right, fighters);
    }
}


/// <summary>
/// Функция вывода дерева в порядке убывания фамилий
/// </summary>
/// <param name="root"></param>
/// <param name="fighters"></param>
void printDescendingByLastName(BinaryTree* root, const BestMMAFighters fighters[]) {
    if (root != nullptr) {
        printDescendingByLastName(root->right, fighters);
        printFighterInfo(fighters, root->dataIndex);
        printDescendingByLastName(root->left, fighters);
    }
}


/// <summary>
/// Функция вывода дерева в порядке возрастания по количеству побед
/// </summary>
/// <param name="root"></param>
/// <param name="fighters"></param>
void printAscendingByWins(BinaryTree* root, const BestMMAFighters fighters[]) {
    if (root != nullptr) {
        printAscendingByWins(root->right, fighters);
        printFighterInfo(fighters, root->dataIndex);
        printAscendingByWins(root->left, fighters);
    }
}


/// <summary>
/// Функция вывода дерева в порядке убывания по количеству побед
/// </summary>
/// <param name="root"></param>
/// <param name="fighters"></param>
void printDescendingByWins(BinaryTree* root, const BestMMAFighters fighters[]) {
    if (root != nullptr) {
        printDescendingByWins(root->left, fighters);
        printFighterInfo(fighters, root->dataIndex);
        printDescendingByWins(root->right, fighters);
    }
}


/// <summary>
/// Реурсивная функция бинарного поиска узла в дереве по фамилии
/// </summary>
/// <param name="root"></param>
/// <param name="lastName"></param>
/// <returns></returns>
BinaryTree* searchTreeNodeByLastNameIter(BinaryTree* root, string lastName) {
    BinaryTree* current = root;
    // Поиск узла в дереве с заданным количеством побед
    while (current != nullptr) {
        if (lastName == current->keyLastName) {
            return current;
        }
        else if (lastName > current->keyLastName) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return nullptr;
}


/// <summary>
/// Итерационная функция бинарного поиска узла в дереве по количеству побед
/// </summary>
/// <param name="root"></param>
/// <param name="wins"></param>
/// <returns></returns>
void searchTreeNodeByWinsRecur(BinaryTree* root, const BestMMAFighters fighters[], int wins, int& counter) {
    if (root->keyWins == wins) {
        counter++;
        if (counter == 1) {
            cout << "\nНайден боец (бойцы) в списке:" << endl;
        }
        cout << "\nБоец №:" << counter << endl;
        printFighterInfo(fighters, root->dataIndex);
    }

    // Поиск узла в дереве с заданной фамилией
    if (wins < root->keyWins) {
        searchTreeNodeByWinsRecur(root->left, fighters, wins, counter);
    }
    else {
        searchTreeNodeByWinsRecur(root->right, fighters, wins, counter);
    }   
}


/// <summary>
/// Функция для нахождения минимального узла в дереве (для удаления)
/// </summary>
/// <param name="root"></param>
/// <returns></returns>
BinaryTree* findMinNode(BinaryTree* root) {
    // Поиск минимального узла в дереве
    while (root->left != nullptr) {
        root = root->left;
    }
    return root;
}


/// <summary>
/// Функция удаления узла из дерева по фамилии
/// </summary>
/// <param name="rootLastName"></param>
/// <param name="lastName"></param>
/// <param name="dataIndex"></param>
/// <param name="fighters"></param>
/// <param name="numFighters"></param>
/// <returns></returns>
BinaryTree* deleteNodeByLastName(BinaryTree* rootLastName, string lastName, int dataIndex, BestMMAFighters fighters[], int& numFighters) {
    if (rootLastName == nullptr) {
        return rootLastName;
    }

    if (lastName > rootLastName->keyLastName) {
        rootLastName->left = deleteNodeByLastName(rootLastName->left, lastName, dataIndex, fighters, numFighters);
    }
    else if (lastName < rootLastName->keyLastName) {
        rootLastName->right = deleteNodeByLastName(rootLastName->right, lastName, dataIndex, fighters, numFighters);
    }
    else {
        // Если keyWins совпадает, проверяем dataIndex
        if (rootLastName->dataIndex == dataIndex) {
            // Удаляем узел
            if (rootLastName->left == nullptr) {
                BinaryTree* temp = rootLastName->right;
                delete rootLastName;
                return temp;
            }
            else if (rootLastName->right == nullptr) {
                BinaryTree* temp = rootLastName->left;
                delete rootLastName;
                return temp;
            }
            else {
                // Находим минимальный узел в правом поддереве
                BinaryTree* minNode = findMinNode(rootLastName->right);

                // Копируем данные минимального узла
                rootLastName->keyLastName = minNode->keyLastName;
                rootLastName->keyWins = minNode->keyWins;
                rootLastName->dataIndex = minNode->dataIndex;

                // Удаляем минимальный узел
                rootLastName->right = deleteNodeByLastName(rootLastName->right, minNode->keyLastName, minNode->dataIndex, fighters, numFighters);
            }
        }
        else {
            // Продолжаем поиск в правом поддереве
            rootLastName->right = deleteNodeByLastName(rootLastName->right, lastName, dataIndex, fighters, numFighters);
        }
    }
    return rootLastName;
}


/// <summary>
/// Функция удаления узла из дерева по количеству побед
/// </summary>
/// <param name="rootWins"></param>
/// <param name="wins"></param>
/// <param name="dataIndex"></param>
/// <param name="fighters"></param>
/// <param name="numFighters"></param>
/// <returns></returns>
BinaryTree* deleteNodeByWins(BinaryTree* rootWins, int wins, int dataIndex, BestMMAFighters fighters[], int& numFighters) {
    if (rootWins == nullptr) {
        return rootWins;
    }

    if (wins > rootWins->keyWins) {
        rootWins->left = deleteNodeByWins(rootWins->left, wins, dataIndex, fighters, numFighters);
    }
    else if (wins < rootWins->keyWins) {
        rootWins->right = deleteNodeByWins(rootWins->right, wins, dataIndex, fighters, numFighters);
    }
    else {
        // Если keyWins совпадает, проверяем dataIndex
        if (rootWins->dataIndex == dataIndex) {
            // Удаляем узел
            if (rootWins->left == nullptr) {
                BinaryTree* temp = rootWins->right;
                delete rootWins;
                return temp;
            }
            else if (rootWins->right == nullptr) {
                BinaryTree* temp = rootWins->left;
                delete rootWins;
                return temp;
            }
            else {
                // Находим минимальный узел в правом поддереве
                BinaryTree* minNode = findMinNode(rootWins->right);

                // Копируем данные минимального узла
                rootWins->keyLastName = minNode->keyLastName;
                rootWins->keyWins = minNode->keyWins;
                rootWins->dataIndex = minNode->dataIndex;

                // Удаляем минимальный узел
                rootWins->right = deleteNodeByWins(rootWins->right, minNode->keyWins, minNode->dataIndex, fighters, numFighters);
            }
        }
        else {
            // Продолжаем поиск в правом поддереве
            rootWins->right = deleteNodeByWins(rootWins->right, wins, dataIndex, fighters, numFighters);
        }
    }
    return rootWins;
}


/// <summary>
/// Функция удаления узлов из дерева
/// </summary>
/// <param name="rootLastName"></param>
/// <param name="rootWins"></param>
/// <param name="lastName"></param>
/// <param name="wins"></param>
/// <param name="fighters"></param>
/// <param name="numFighters"></param>
/// <param name="choice"></param>
/// <returns></returns>
BinaryTree* deleteNode(BinaryTree* rootLastName, BinaryTree*& rootWins, string lastName, int wins, BestMMAFighters fighters[], int& numFighters, int choice) {
    bool deleted;

    deleted = false;
    BinaryTree* current = rootLastName;
    BinaryTree* parent = nullptr;

    if (choice == 0) {
        do {

            deleted = false;
            current = rootLastName;
            parent = nullptr;

            // Поиск узла для удаления
            while (current != nullptr && current->keyLastName != lastName) {
                parent = current;
                if (lastName < current->keyLastName) {
                    current = current->left;
                }
                else {
                    current = current->right;
                }
            }

            // Если узел найден, удаляем его
            if (current != nullptr) {
                deleted = true;

                // Количество побед для удаления
                int helpWins = current->keyWins;
                int dataIndex = current->dataIndex;

                // Удаляем соответствующий узел из дерева по количеству побед
                rootWins = deleteNodeByWins(rootWins, helpWins, dataIndex, fighters, numFighters);

                // Убавляем количество бойцов
                numFighters--;

                // Удаление узла
                if (current->left == nullptr) {
                    BinaryTree* temp = current->right;
                    if (parent == nullptr) {
                        rootLastName = temp;
                    }
                    else if (parent->left == current) {
                        parent->left = temp;
                    }
                    else {
                        parent->right = temp;
                    }
                    delete current;
                }
                else if (current->right == nullptr) {
                    BinaryTree* temp = current->left;
                    if (parent == nullptr) {
                        rootLastName = temp;
                    }
                    else if (parent->left == current) {
                        parent->left = temp;
                    }
                    else {
                        parent->right = temp;
                    }
                    delete current;
                }
                else {
                    // Находим минимальный узел
                    BinaryTree* minNode = findMinNode(current->right);
                    BinaryTree* minParent = current;

                    // Итеративно ищем родителя минимального узла
                    BinaryTree* temp = current->right;
                    while (temp->left != nullptr) {
                        minParent = temp;
                        temp = temp->left;
                    }

                    // Копируем данные минимального узла
                    current->keyLastName = minNode->keyLastName;
                    current->keyWins = minNode->keyWins;
                    current->dataIndex = minNode->dataIndex;

                    // Удаляем минимальный узел
                    if (minParent->left == minNode) {
                        minParent->left = minNode->right;
                    }
                    else {
                        minParent->right = minNode->right;
                    }
                    delete minNode;
                }
            }           
        } while (deleted); // Повторяем, пока есть узлы для удаления

        return rootLastName;
    }

    else {
        do {
            deleted = false;
            current = rootWins;
            parent = nullptr;

            // Поиск узла для удаления
            while (current != nullptr && current->keyWins != wins) {
                parent = current;
                if (wins > current->keyWins) {
                    current = current->left;
                }
                else {
                    current = current->right;
                }
            }

            // Если узел найден, удаляем его
            if (current != nullptr) {
                deleted = true;

                // Фамилия для удаления
                string helpLastName = current->keyLastName;
                int dataIndex = current->dataIndex;

                // Удаляем соответствующий узел из дерева по фамилиям
                rootLastName = deleteNodeByLastName(rootLastName, helpLastName, dataIndex, fighters, numFighters);

                // Убавляем количество бойцов
                numFighters--;         

                // Удаление узла
                if (current->left == nullptr) {
                    BinaryTree* temp = current->right;
                    if (parent == nullptr) {
                        rootWins = temp;
                    }
                    else if (parent->left == current) {
                        parent->left = temp;
                    }
                    else {
                        parent->right = temp;
                    }
                    delete current;
                }
                else if (current->right == nullptr) {
                    BinaryTree* temp = current->left;
                    if (parent == nullptr) {
                        rootWins = temp;
                    }
                    else if (parent->left == current) {
                        parent->left = temp;
                    }
                    else {
                        parent->right = temp;
                    }
                    delete current;
                }
                else {
                    // Находим минимальный узел
                    BinaryTree* minNode = findMinNode(current->right);
                    BinaryTree* minParent = current;

                    // Итеративно ищем родителя минимального узла
                    BinaryTree* temp = current->right;
                    while (temp->left != nullptr) {
                        minParent = temp;
                        temp = temp->left;
                    }

                    // Копируем данные минимального узла
                    current->keyLastName = minNode->keyLastName;
                    current->keyWins = minNode->keyWins;
                    current->dataIndex = minNode->dataIndex;

                    // Удаляем минимальный узел
                    if (minParent->left == minNode) {
                        minParent->left = minNode->right;
                    }
                    else {
                        minParent->right = minNode->right;
                    }
                    delete minNode;
                }
            } 
        } while (deleted); // Повторяем, пока есть узлы для удаления

        return rootWins;
    }   
}


//------------------------------------
// Задание 3
//------------------------------------


/// <summary>
/// Функция создания нового элемента списка
/// </summary>
/// <param name="fighter"></param>
/// <returns></returns>
FighterList* createFighterItem(const BestMMAFighters& fighter) {
    FighterList* newItem = new FighterList;
    newItem->fighter = fighter;
    newItem->next = nullptr;
    return newItem;
}


/// <summary>
/// Функция создания списка в порядке ввода
/// </summary>
/// <param name="head"></param>
/// <param name="fighter"></param>
void insertUnsorted(FighterList** head, const BestMMAFighters& fighter) {
    FighterList* newItem = createFighterItem(fighter);
    if (*head == nullptr) {
        *head = newItem;
    }
    else {
        FighterList* current = *head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newItem;
    }
}


/// <summary>
/// Функция вывода списка в порядке ввода
/// </summary>
/// <param name="head"></param>
void printListInOrder(FighterList* head) {
    FighterList* current = head;
    // Вывод списка бойцов в порядке ввода
    while (current != nullptr) {
        cout << "\nИмя и фамилия: " << current->fighter.firstName << " " << current->fighter.lastName
            << ", Год рождения: " << current->fighter.birthYear
            << ", Количество побед: " << current->fighter.wins
            << ", Количество поражений: " << current->fighter.losses
            << ", Весовая категория: " << current->fighter.weightClass << endl;
        current = current->next;
    }
}


/// <summary>
/// Функция создания списка в порядке возрастания фамилий
/// </summary>
/// <param name="head"></param>
/// <param name="fighter"></param>
void insertSortedByLastName(FighterList** head, const BestMMAFighters& fighter) {
    FighterList* newItem = createFighterItem(fighter);
    FighterList* current;
    // Если список пуст или новый узел должен быть вставлен в начало
    if (*head == nullptr || fighter.lastName < (*head)->fighter.lastName) {
        newItem->next = *head;
        *head = newItem;
    }
    else {
        // Находим место для вставки
        current = *head;
        while (current->next != nullptr && current->next->fighter.lastName < fighter.lastName) {
            current = current->next;
        }
        newItem->next = current->next;
        current->next = newItem;
    }
}


/// <summary>
/// Функция создания списка в порядке убывания количества побед
/// </summary>
/// <param name="head"></param>
/// <param name="fighter"></param>
void insertSortedByWins(FighterList** head, const BestMMAFighters& fighter) {
    FighterList* newItem = createFighterItem(fighter);
    FighterList* current;
    // Если список пуст или новый узел должен быть вставлен в начало
    if (*head == nullptr || fighter.wins > (*head)->fighter.wins) {
        newItem->next = *head;
        *head = newItem;
    }
    else {
        // Находим место для вставки
        current = *head;
        while (current->next != nullptr && current->next->fighter.wins > fighter.wins) {
            current = current->next;
        }
        newItem->next = current->next;
        current->next = newItem;
    }
}


/// <summary>
/// Функция вывода списка в порядке возрастания фамилий
/// </summary>
/// <param name="head"></param>
void printListSortedByLastName(FighterList* head) {
    FighterList* sortedHead = nullptr;
    FighterList* current = head;
    while (current != nullptr) {
        insertSortedByLastName(&sortedHead, current->fighter);
        current = current->next;
    }
    printListInOrder(sortedHead);
}


/// <summary>
/// Функция вывода списка в порядке убывания количества побед
/// </summary>
/// <param name="head"></param>
void printListSortedByWins(FighterList* head) {
    FighterList* sortedHead = nullptr;
    FighterList* current = head;
    while (current != nullptr) {
        insertSortedByWins(&sortedHead, current->fighter);
        current = current->next;
    }
    printListInOrder(sortedHead);
}


/// <summary>
/// Итерационная функция поиска бойца по фамилии и вывода информации о нем
/// </summary>
/// <param name="head"></param>
/// <param name="lastName"></param>
void searchAndPrintItemByLastNameIter(FighterList* head, const string& lastName) {
    FighterList* current = head;
    int counter = 0;
    bool found = false;
    while (current != nullptr) {
        if (current->fighter.lastName == lastName) {
            counter++;
            if (counter == 1) {
                cout << "\nНайден боец (бойцы) в списке:" << endl;
            }
            cout << "\nБоец №:" << counter << endl;
            cout << "\nИмя и фамилия: " << current->fighter.firstName << " " << current->fighter.lastName
                << ", Год рождения: " << current->fighter.birthYear
                << ", Количество побед: " << current->fighter.wins
                << ", Количество поражений: " << current->fighter.losses
                << ", Весовая категория: " << current->fighter.weightClass << endl;
            found = true;
        }
        current = current->next;
    }
    if (!found) {
        cout << "\nБоец с фамилией " << lastName << " не найден в списке." << endl;
    }
}


/// <summary>
/// Рекурсивная функция поиска бойца по количеству побед и вывода информации о нем
/// </summary>
/// <param name="current"></param>
/// <param name="wins"></param>
/// <param name="found"></param>
void searchAndPrintItemByWinsRecur(FighterList* current, int wins, bool& found, int counter) {
    if (current == nullptr) {
        return;
    }
    if (current->fighter.wins == wins) {
        counter++;
        if (counter == 1) {
            cout << "\nНайден боец (бойцы) в списке:" << endl;
        }
        cout << "\nБоец №:" << counter << endl;
        cout << "\nИмя и фамилия: " << current->fighter.firstName << " " << current->fighter.lastName
            << ", Год рождения: " << current->fighter.birthYear
            << ", Количество побед: " << current->fighter.wins
            << ", Количество поражений: " << current->fighter.losses
            << ", Весовая категория: " << current->fighter.weightClass << endl;
        found = true;
    }
    searchAndPrintItemByWinsRecur(current->next, wins, found, counter);
}


/// <summary>
/// Вспомогательная функция для вызова рекурсивной функции поиска и вывода и рассмотрения ошибки
/// </summary>
/// <param name="head"></param>
/// <param name="wins"></param>
void searchAndPrintItemByWins(FighterList* head, int wins) {
    FighterList* current = head;
    bool found = false;
    // Вызов рекурсивной функции поиска и вывода
    searchAndPrintItemByWinsRecur(head, wins, found, 0);
    // Вывод ошибки в случае отсутствия заданного количества побед в списке
    if (!found) {
        cout << "\nБоец с количеством побед " << wins << " не найден в списке." << endl;
    }
}


/// <summary>
/// Удаление бойца из списка по фамилии
/// </summary>
/// <param name="head"></param>
/// <param name="lastName"></param>
/// <param name="fighters"></param>
/// <param name="numFighters"></param>
/// <returns></returns>
FighterList* deleteFromListByLastName(FighterList* head, const string& lastName, BestMMAFighters fighters[], int& numFighters) {
    FighterList* current = head;
    FighterList* prev = nullptr;

    while (current != nullptr) {
        if (current->fighter.lastName == lastName) {
            // Удаляем элемент
            if (prev == nullptr) {
                // Удаляем первый элемент
                head = current->next;
                delete current;
                current = head; // Перемещаем current на новый head
            }
            else {
                // Удаляем элемент из середины или конца
                prev->next = current->next;
                delete current;
                current = prev->next; // Перемещаем current на следующий элемент
            }
            numFighters--; // Уменьшаем счетчик бойцов
        }
        else {
            // Переходим к следующему элементу
            prev = current;
            current = current->next;
        }
    }

    return head;
}


/// <summary>
/// Удаление бойца из списка по количеству побед
/// </summary>
/// <param name="head"></param>
/// <param name="wins"></param>
/// <param name="fighters"></param>
/// <param name="numFighters"></param>
/// <returns></returns>
FighterList* deleteFromListByWins(FighterList* head, int wins, BestMMAFighters fighters[], int& numFighters) {
    FighterList* current = head;
    FighterList* prev = nullptr;

    while (current != nullptr) {
        if (current->fighter.wins == wins) {
            // Удаляем элемент
            if (prev == nullptr) {
                // Удаляем первый элемент
                head = current->next;
                delete current;
                current = head; // Перемещаем current на новый head
            }
            else {
                // Удаляем элемент из середины или конца
                prev->next = current->next;
                delete current;
                current = prev->next; // Перемещаем current на следующий элемент
            }
            numFighters--; // Уменьшаем счетчик бойцов
        }
        else {
            // Переходим к следующему элементу
            prev = current;
            current = current->next;
        }
    }

    return head;
}


int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");
    int inputNumToFirstMenu, inputNumToSecondMenu, inputNumToThirdMenu, inputNumToFourthMenu;
    int numFighters = 0;
    do {
        cout << "1. Первая часть программы\n2. Вторая часть программы"
            << "\n3. Третья часть программы\n4. Завершить работу программы" << endl;
        inputNumToFirstMenu = getIntNumber(1, 4);
        system("cls");

        IndexEntry lastNameIndex[30];
        IndexEntry winsIndex[30];
        BestMMAFighters fighters[30];

        switch (inputNumToFirstMenu) {
        case 1: {
            //------------------------------------
            // Задание 1
            //------------------------------------
            do {
                cout << "1. Получить информацию о бойцах из текстового файла\n2. Ввести бойцов вручную" << endl;
                inputNumToSecondMenu = getIntNumber(1, 2);
                switch (inputNumToSecondMenu) {
                case 1: {
                    numFighters = 10;
                    readFightersFromFile("fightersInfo.txt", fighters, numFighters);
                    break;
                }
                case 2: {
                    system("cls");
                    cout << "Ввод количества бойцов : ";
                    numFighters = getIntNumber(1, 30);
                    system("cls");

                    // Ввод данных о бойцах
                    inputFighters(fighters, numFighters);
                }
                }
            } while (numFighters == 0);
            

            // Создаем индексы
            createLastNameIndex(fighters, numFighters, lastNameIndex);
            createWinsIndex(fighters, numFighters, winsIndex);

            system("cls");
            cout << "Список бойцов:" << endl;
            printInfoAboutFighters(fighters, numFighters);

            do {
                cout << "\n1. Вывести информацию о бойцах в порядке ввода\n2. Вывести отсортированную информацию по фамилии"
                    << "\n3. Вывести отсортированную информацию по количеству побед"<< "\n4. Поиск бойца по фамилии"
                    << "\n5. Поиск бойца по количеству побед\n6. Изменение информации о бойце\n7. Удаление информации о бойце по фамилии"
                    << "\n8. Удаление информации о бойце по количеству побед\n9. Выход в главное меню" << endl;
                inputNumToThirdMenu = getIntNumber(1, 9);
                system("cls");

                switch (inputNumToThirdMenu) {
                case 1: {
                    // Вывод бойцов в порядке ввода
                    cout << "Вывод списка в порядке ввода:" << endl;
                    printInfoAboutFighters(fighters, numFighters);
                    break;
                }
                case 2: {
                    // Вывод отсортированной информации по фамилии
                    printFightersByLastNamesAscending(fighters, lastNameIndex, numFighters);
                    break;
                }
                case 3: {
                    // Вывод отсортированной информации по количеству побед
                    printFightersByWinsNumberDescending(fighters, winsIndex, numFighters);
                    break;
                }
                case 4: {
                    // Поиск по фамилии
                    string searchLastName = getStringInput("Введите фамилию бойца для поиска: ");
                    int foundLastNameIndex = binarySearchLastNameRecur(lastNameIndex, 0, numFighters - 1, searchLastName);
                    if (foundLastNameIndex != -1) {
                        cout << "\nНайден боец:" << endl;
                        printFighterInfo(fighters, foundLastNameIndex);
                    }
                    else
                        cout << "\nБоец с количеством побед " << searchLastName << " не найден." << endl;
                    break;
                }
                case 5: {
                    // Поиск по количеству побед
                    cout << "Ввод количества побед бойца для поиска: ";
                    int searchWins = getIntNumber(0, 100);
                    int foundWinsIndex = binarySearchWinsIter(winsIndex, numFighters, searchWins);
                    if (foundWinsIndex != -1) {
                        cout << "\nНайден боец:" << endl;
                        printFighterInfo(fighters, foundWinsIndex);
                    }
                    else 
                        cout << "\nБоец с количеством побед " << searchWins << " не найден." << endl;
                    break;
                }
                case 6: {
                    // Изменение информации о бойце            
                    editFighter(fighters, lastNameIndex, winsIndex, numFighters);
                    break;
                }
                case 7: {
                    // Удаление информации о бойце по фамилии
                    deleteFighterByLastNameOrByWins(fighters, lastNameIndex, winsIndex, numFighters, 1);
                    break;
                }
                case 8: {
                    // Удаление информации о бойце по количеству побед
                    deleteFighterByLastNameOrByWins(fighters, lastNameIndex, winsIndex, numFighters, 0);
                    break;
                }
                }
            } while (inputNumToThirdMenu != 9);
            break;
        }
        case 2: {
            //------------------------------------
            // Задание 2
            //------------------------------------

            do {
                cout << "1. Получить информацию о бойцах из текстового файла\n2. Ввести бойцов вручную" << endl;
                inputNumToSecondMenu = getIntNumber(1, 2);
                switch (inputNumToSecondMenu) {
                case 1: {
                    numFighters = 10;
                    readFightersFromFile("fightersInfo.txt", fighters, numFighters);
                    break;
                }
                case 2: {
                    system("cls");
                    cout << "Ввод количества бойцов : ";
                    numFighters = getIntNumber(1, 30);
                    system("cls");

                    // Ввод данных о бойцах
                    inputFighters(fighters, numFighters);
                }
                }
            } while (numFighters == 0);

            // Создаем индексы
            createLastNameIndex(fighters, numFighters, lastNameIndex);
            createWinsIndex(fighters, numFighters, winsIndex);

            // Создание дерева на основе фамилий
            BinaryTree* rootLastName = nullptr;
            for (int i = 0; i < numFighters; ++i) {
                rootLastName = insertNodeByLastName(rootLastName, fighters[i].lastName, fighters[i].wins, i);
            }

            // Создание дерева на основе количества побед
            BinaryTree* rootWins = nullptr;
            for (int i = 0; i < numFighters; ++i) {
                rootWins = insertNodeByWins(rootWins, fighters[i].wins, fighters[i].lastName, i);
            }
            system("cls");
            cout << "Список бойцов:" << endl;
            printInfoAboutFighters(fighters, numFighters);

            do {
                cout << "\n1. Вывод данных по возрастанию фамилии\n2. Вывод данных по убыванию фамилии"
                    << "\n3. Вывод данных по возрастанию количества побед\n4. Вывод данных по убыванию количества побед"
                    << "\n5. Поиск бойца по фамилии\n6. Поиск бойца по количеству побед\n7. Удаление информации о бойце (бойцах) по фамилии"
                    << "\n8. Удаление информации о бойце (бойцах) по количеству побед\n9. Выход в главное меню" << endl;
                inputNumToThirdMenu = getIntNumber(1, 9);
                system("cls");

                int counter1 = 0, counter2 = 0; // Для правильного порядка удаления

                switch (inputNumToThirdMenu) {
                case 1: {                                        
                    // Вывод данных по возрастанию фамилии
                    cout << "Вывод данных по возрастанию фамилии:" << endl;
                    printAscendingByLastName(rootLastName, fighters);
                    break;
                }
                case 2: {
                    // Вывод данных по убыванию фамилии
                    cout << "Вывод данных по убыванию фамилии:" << endl;
                    printDescendingByLastName(rootLastName, fighters);
                    break;
                }
                case 3: {
                    // Вывод данных по возрастанию количества побед
                    cout << "Вывод данных по возрастанию количества побед:" << endl;
                    printAscendingByWins(rootWins, fighters);
                    break;
                }
                case 4: {
                    // Вывод данных по убыванию количества побед
                    cout << "Вывод данных по убыванию количества побед:" << endl;
                    printDescendingByWins(rootWins, fighters);
                    break;
                }
                case 5: {
                    // Поиск по фамилии
                    string searchLastName = getStringInput ("Введите фамилию для поиска: ");                   
                    BinaryTree* foundNodeByWins = searchTreeNodeByLastNameIter(rootLastName, searchLastName);
                    if (foundNodeByWins != nullptr) {
                        cout << "\nНайден боец:" << endl;
                        printFighterInfo(fighters, foundNodeByWins->dataIndex);
                    }
                    else {
                        cout << "\nБоец с фамилией " << searchLastName << " не найден." << endl;
                    }
                    break;
                }
                case 6: {
                    // Поиск по количеству побед
                    int searchWins;
                    cout << "Введите количество побед для поиска: ";
                    searchWins = getIntNumber(0, 100);
                    int counter = 0;
                    searchTreeNodeByWinsRecur(rootWins, fighters, searchWins, counter);
                    if (counter == 0)
                        cout << "\nБоец с количеством побед " << searchWins << " не найден." << endl;
                    break;
                }
                case 7: {
                    // Счетчик для правильного порядка удаления
                    counter1++;

                    // Удаление узла по фамилии
                    string lastNameToDelete = getStringInput ("Введите фамилию для удаления: ");
                    if (counter1 > counter2) {
                        rootLastName = deleteNode(rootLastName, rootWins, lastNameToDelete, 0, fighters, numFighters, 0);
                    }
                    else {
                        rootLastName = deleteNode(rootLastName, rootWins, lastNameToDelete, 0, fighters, numFighters, 1);
                    }
                    if (numFighters != 0) {
                        // Вывод дерева после удаления
                        system("cls");
                        cout << "Дерево по фамилиям после удаления:" << endl;
                        printAscendingByLastName(rootLastName, fighters);
                        cout << "\nДерево по количеству побед после удаления:" << endl;
                        printDescendingByWins(rootWins, fighters);
                    }
                    else {
                        cout << "\nДерево осталось пустым" << endl;
                    }
                    counter1 = 0;
                    break;
                }
                case 8: {
                    // Счетчик для правильного порядка удаления
                    counter2++;

                    // Удаление узла по количеству побед
                    int winsToDelete;
                    cout << "Введите количество побед для удаления: ";
                    winsToDelete = getIntNumber(0, 100);
                    if (counter1 > counter2) {
                        rootLastName = deleteNode(rootLastName, rootWins, "", winsToDelete, fighters, numFighters, 0);
                    }
                    else {
                        rootLastName = deleteNode(rootLastName, rootWins, "", winsToDelete, fighters, numFighters, 1);
                    }
                    if (numFighters != 0) {
                        // Вывод дерева после удаления
                        system("cls");
                        cout << "Дерево по количеству побед после удаления:" << endl;
                        printDescendingByWins(rootWins, fighters);
                        cout << "\nДерево по фамилиям после удаления:" << endl;
                        printAscendingByLastName(rootLastName, fighters);
                    }
                    else {
                        cout << "\nДерево осталось пустым" << endl;
                    }
                    counter2 = 0;
                    break;
                }
                }
            } while (inputNumToThirdMenu != 9 && rootWins != nullptr);
            break;
        }
        case 3: {
            //------------------------------------
            // Задание 3
            //------------------------------------

            do {
                cout << "1. Получить информацию о бойцах из текстового файла\n2. Ввести бойцов вручную" << endl;
                inputNumToSecondMenu = getIntNumber(1, 2);
                switch (inputNumToSecondMenu) {
                case 1: {
                    numFighters = 10;
                    readFightersFromFile("fightersInfo.txt", fighters, numFighters);
                    break;
                }
                case 2: {
                    system("cls");
                    cout << "Ввод количества бойцов : ";
                    numFighters = getIntNumber(1, 30);
                    system("cls");

                    // Ввод данных о бойцах
                    inputFighters(fighters, numFighters);
                }
                }
            } while (numFighters == 0);

            // Создаем индексы
            createLastNameIndex(fighters, numFighters, lastNameIndex);
            createWinsIndex(fighters, numFighters, winsIndex);

            // Создание линейного списка
            FighterList* head = nullptr;
            for (int i = 0; i < numFighters; ++i) {
                insertUnsorted(&head, fighters[i]);
            }

            system("cls");
            cout << "Список бойцов:" << endl;
            printListInOrder(head);

            do {
                cout << "\n1. Вывод списка в порядке ввода\n2. Вывод списка в порядке возрастания фамилии"
                    << "\n3. Вывод списка в порядке убывания количества побед\n4. Поиск бойца по фамилии"
                    << "\n5. Поиск бойца (бойцов) по количеству побед\n6. Добавление нового бойца (бойцов) в список"
                    << "\n7. Удаление информации о бойце (бойцах) по фамилии\n8. Удаление информации о бойце (бойцах) по количеству побед"
                    << "\n9. Выход в главное меню" << endl;
                inputNumToThirdMenu = getIntNumber(1, 9);
                system("cls");

                switch (inputNumToThirdMenu) {
                case 1: {
                    // Вывод списка в порядке ввода
                    cout << "Вывод списка в порядке ввода:" << endl;
                    printListInOrder(head);
                    break;
                }
                case 2: {
                    // Вывод списка в порядке возрастания фамилии
                    cout << "Вывод списка в порядке возрастания фамилии:" << endl;
                    printListSortedByLastName(head);
                    break;
                }
                case 3: {
                    // Вывод списка в порядке убывания количества побед
                    cout << "Вывод списка в порядке убывания количества побед:" << endl;
                    printListSortedByWins(head);
                    break;
                }
                case 4: {
                    // Поиск и вывод по фамилии в списке
                    string searchLastNameList = getStringInput ("Введите фамилию для поиска в списке: ");
                    searchAndPrintItemByLastNameIter(head, searchLastNameList);
                    break;
                }
                case 5: {
                    // Поиск и вывод по количеству побед в списке 
                    cout << "Введите количество побед для поиска в списке: ";
                    int searchWinsList = getIntNumber(0, 100);
                    searchAndPrintItemByWins(head, searchWinsList);
                    break;
                }
                case 6: {
                    // Добавление новых(ого) бойцов(а) в список и вывод нового списка
                    cout << "Введите количество бойцов для добавления в список:" << endl;
                    int numOfNewFighters = getIntNumber(1, 10);
                    BestMMAFighters newFighters[10];
                    inputFighters(newFighters, numOfNewFighters);
                    createLastNameIndex(fighters, numFighters, lastNameIndex);
                    createWinsIndex(fighters, numFighters, winsIndex);
                    system("cls");
                    cout << "Новые бойцы для добавления в список:" << endl;
                    printInfoAboutFighters(newFighters, numOfNewFighters);
                    int count = 0;
                    do {
                        cout << "\n1. Вывести новый список с помощью сортировки по фамилии"
                            << "\n2. Вывести новый список с помощью сортировки по количеству побед" << endl;
                        inputNumToFourthMenu = getIntNumber(1, 2);
                        system("cls");

                        switch (inputNumToFourthMenu) {
                        case 1: {
                            for (int i = 0; i < numOfNewFighters; i++) {
                                fighters[numFighters] = newFighters[i];
                                insertSortedByLastName(&head, newFighters[i]);
                                numFighters++;
                            }
                            cout << "Новый список после добавления бойцов и сортировки по фамилии:" << endl;
                            printListSortedByLastName(head);
                            count++;
                            break;
                        }
                        case 2: {
                            for (int i = 0; i < numOfNewFighters; i++) {
                                fighters[numFighters] = newFighters[i];
                                insertSortedByWins(&head, newFighters[i]);
                                numFighters++;
                            }
                            cout << "Новый список после добавления бойцов и сортировки по количеству побед:" << endl;
                            printListSortedByWins(head);
                            count++;
                            break;
                        }
                        }
                    } while (count == 0);
                    break;
                }
                case 7: {
                    // Удаление записи из списка по фамилии
                    string lastNameToDeleteList = getStringInput ("Введите фамилию для удаления из списка: ");
                    int check = numFighters;
                    head = deleteFromListByLastName(head, lastNameToDeleteList, fighters, numFighters);
                    system("cls");
                    if (check == numFighters) {
                    }
                    else if (numFighters != 0) {
                        cout << "Список после удаления:" << endl;
                        printListInOrder(head);
                    }
                    else {
                        cout << "Список остался пустым" << endl;
                    }
                    break;
                }
                case 8: {
                    // Удаление записи из списка по количеству побед
                    int winsToDeleteList;
                    cout << "Введите количество побед для удаления из списка: ";
                    winsToDeleteList = getIntNumber(0, 100);
                    int check = numFighters;
                    head = deleteFromListByWins(head, winsToDeleteList, fighters, numFighters);
                    system("cls");
                    if (check == numFighters) {
                    }
                    else if (numFighters != 0) {
                        cout << "Список после удаления:" << endl;
                        printListInOrder(head);
                    }
                    else {
                        cout << "Список остался пустым" << endl;
                    }
                    break;
                }
                }
            } while (inputNumToThirdMenu != 9 && head != nullptr);
            break;
        }
        case 4:
            cout << "\nПрограмма завершила работу\n";
        }
    } while (inputNumToFirstMenu != 4);
}