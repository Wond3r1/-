#ifndef FIGHTER_LIST_H
#define FIGHTER_LIST_H
#include <string>
#include "BestMMAFighters.h"
using namespace std;


/// <summary>
/// ��������� �������� ��������� ������
/// </summary>
struct FighterList {
    BestMMAFighters fighter; // ������ � �����
    FighterList* next;       // ��������� �� ��������� ������� ������
};


#endif
