#ifndef BEST_MMA_FIGHTERS_H
#define BEST_MMA_FIGHTERS_H
#include <string>
using namespace std;


/// <summary>
/// ��������� ��� ���������� � �����
/// </summary>
struct BestMMAFighters {
    string firstName;
    string lastName;
    int birthYear;
    int wins;
    int losses;
    string weightClass;

    // ����������� �� ���������
    BestMMAFighters() : firstName(""), lastName(""), birthYear(-1), wins(-1), losses(-1), weightClass("") {}

    // ����������� � �����������
    BestMMAFighters(string firstName, string lastName, int birthYear, int wins, int losses, string weightClass)
        : firstName(firstName), lastName(lastName), birthYear(birthYear), wins(wins), losses(losses), weightClass(weightClass) {
    }
};



#endif
