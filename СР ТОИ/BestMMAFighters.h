#ifndef BEST_MMA_FIGHTERS_H
#define BEST_MMA_FIGHTERS_H
#include <string>
using namespace std;


/// <summary>
/// Структура для информации о бойце
/// </summary>
struct BestMMAFighters {
    string firstName;
    string lastName;
    int birthYear;
    int wins;
    int losses;
    string weightClass;

    // Конструктор по умолчанию
    BestMMAFighters() : firstName(""), lastName(""), birthYear(-1), wins(-1), losses(-1), weightClass("") {}

    // Конструктор с параметрами
    BestMMAFighters(string firstName, string lastName, int birthYear, int wins, int losses, string weightClass)
        : firstName(firstName), lastName(lastName), birthYear(birthYear), wins(wins), losses(losses), weightClass(weightClass) {
    }
};



#endif
