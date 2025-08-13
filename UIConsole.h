#define NOMINMAX 1
#define byte _win_byte
#include <windows.h>
#undef byte
#include <iostream>
#include <iomanip>
using namespace std;

void SetColor(WORD color);

void gotoxy(short x, short y);

void ShowCur(bool CursorVisibility);

void clrscr();

void resizeConsole(int width, int height);

void fixConsoleWindowSize(int width, int height);

void SetBGColor(WORD color);

void CreateBoxSingle(int x, int y, string text, int length);
// Vẽ hộp với khung **kép**
void CreateBoxDouble(int x, int y, string text, int length);

void ThongBao(const char *s);

void ThongBaoMuon(const char *s);

int wherex(void);

int wherey(void);

void clreol();