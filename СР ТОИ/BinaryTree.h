#ifndef BINARY_TREE_H
#define BINARY_TREE_H
#include <string>
using namespace std;

/// <summary>
/// Структура для бинарного дерева
/// </summary>
struct BinaryTree {
    string keyLastName; // Ключевое поле (фамилия)
    int keyWins;        // Ключевое поле (количество побед)
    int dataIndex;      // Индекс элемента в массиве
    BinaryTree* left;
    BinaryTree* right;

    BinaryTree() : keyLastName(""), keyWins(-1), dataIndex(-1), left(nullptr), right(nullptr) {}
};

#endif