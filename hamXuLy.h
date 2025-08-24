#ifndef HAMXULY_H
#define HAMXULY_H

#include <string>
#include "UIConsole.h"
#include <conio.h>

const string INPUT_CANCELLED = "##USER_CANCELLED##";

string chuanHoaTen(string s);
string inputName(int x, int y, int maxLen, bool allowSpace);
string inputNumber(int x, int y, int maxLen);
string inputViTri(int x, int y, int maxLen);
char GetYesNoInput(int x, int y);
bool isValidGender(string s);

#endif