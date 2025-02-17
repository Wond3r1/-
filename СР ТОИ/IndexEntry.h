#ifndef INDEX_ENTRY_H
#define INDEX_ENTRY_H
#include <string>
using namespace std;


/// <summary>
/// ��������� ��� �������
/// </summary>
struct IndexEntry {
    string keyString; // ��� �����
    int originalIndex; // ������ � �������� �������
    int keyInt;         // ��� ����� �����

    // ����������� �� ���������
    IndexEntry() : keyString(""), originalIndex(-1), keyInt(-1) {}

    // ����������� ��� ���������� �����
    IndexEntry(string keyString, int originalIndex) : keyString(keyString), originalIndex(originalIndex), keyInt(0) {}

    // ����������� ��� ��������� �����
    IndexEntry(int keyInt, int originalIndex) : keyString(""), originalIndex(originalIndex), keyInt(keyInt) {}
};


#endif
