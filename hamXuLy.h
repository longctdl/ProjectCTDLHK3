#ifndef HAMXULY_H
#define HAMXULY_H

#include <string>
#include "UIConsole.h"
#include <conio.h>
#include <ctime>

using namespace std;

const string INPUT_CANCELLED = "##USER_CANCELLED##";

// New struct for Date
struct Date {
    int ngay;
    int thang;
    int nam;
};

string chuanHoaTen(string s);
string inputName(int x, int y, int maxLen, bool allowSpace = false);
string inputNumber(int x, int y, int maxLen);
char GetYesNoInput(int x, int y);
bool isValidGender(string s);
int compareDates(Date d1, Date d2); // New function prototype for date comparison

#endif