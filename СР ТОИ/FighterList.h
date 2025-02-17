#ifndef FIGHTER_LIST_H
#define FIGHTER_LIST_H
#include <string>
#include "BestMMAFighters.h"
using namespace std;


/// <summary>
/// Структура элемента линейного списка
/// </summary>
struct FighterList {
    BestMMAFighters fighter; // Данные о бойце
    FighterList* next;       // Указатель на следующий элемент списка
};


#endif
