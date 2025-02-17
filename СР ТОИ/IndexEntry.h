#ifndef INDEX_ENTRY_H
#define INDEX_ENTRY_H
#include <string>
using namespace std;


/// <summary>
/// Структура для индекса
/// </summary>
struct IndexEntry {
    string keyString; // Для строк
    int originalIndex; // Индекс в исходном массиве
    int keyInt;         // Для целых чисел

    // Конструктор по умолчанию
    IndexEntry() : keyString(""), originalIndex(-1), keyInt(-1) {}

    // Конструктор для строкового ключа
    IndexEntry(string keyString, int originalIndex) : keyString(keyString), originalIndex(originalIndex), keyInt(0) {}

    // Конструктор для числового ключа
    IndexEntry(int keyInt, int originalIndex) : keyString(""), originalIndex(originalIndex), keyInt(keyInt) {}
};


#endif
