#ifndef BINARY_TREE_H
#define BINARY_TREE_H
#include <string>
using namespace std;


/// <summary>
/// ��������� ��� ��������� ������
/// </summary>
struct BinaryTree {
    string keyLastName; // �������� ���� (�������)
    int keyWins; // �������� ���� (���������� �����)
    int dataIndex; // ������ �������� � �������
    BinaryTree* left;
    BinaryTree* right;
};


#endif