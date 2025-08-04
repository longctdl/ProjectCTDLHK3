#define NOMINMAX 1
#define byte _win_byte
#include <windows.h>
#undef byte
#include <iostream>
#include <iomanip>
using namespace std;

void SetColor(WORD color){
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &info);
    WORD attr = info.wAttributes;
    attr &= 0xfff0;
    attr |= color;
    SetConsoleTextAttribute(hConsoleOutput, attr);
}

void gotoxy(short x, short y){
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {x, y};
    SetConsoleCursorPosition(hConsoleOutput, pos);
}

void ShowCur(bool CursorVisibility){
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor = {1, CursorVisibility};
    SetConsoleCursorInfo(handle, &cursor);
}

void clrscr(){
    system("cls");
}

void resizeConsole(int width, int height){
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void fixConsoleWindowSize(int width, int height){
    HWND console = GetConsoleWindow();
    LONG style = GetWindowLong(console, GWL_STYLE);
    style &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX);
    SetWindowLong(console, GWL_STYLE, style);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = {(SHORT)width, (SHORT)height};
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    SMALL_RECT windowSize = {0, 0, (SHORT)(width - 1), (SHORT)(height - 1)};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

void SetBGColor(WORD color){
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &info);
    WORD attr = info.wAttributes;
    color <<= 4;
    attr &= 0xff0f;
    attr |= color;
    SetConsoleTextAttribute(hConsoleOutput, attr);
}

void CreateBoxSingle(int x, int y, string text, int length){
    gotoxy(x - 2, y - 1);
    cout << char(218) << setw(length) << setfill(char(196)) << char(196) << char(191); // ┌───┐

    gotoxy(x - 2, y);
    cout << char(179) << text << setw(length - text.length() + 1) << setfill(' ') << char(179); // │ text │

    gotoxy(x - 2, y + 1);
    cout << char(192) << setw(length) << setfill(char(196)) << char(196) << char(217); // └───┘
}
// Vẽ hộp với khung **kép**
void CreateBoxDouble(int x, int y, string text, int length){
    gotoxy(x - 2, y - 1);
    cout << char(201) << setw(length) << setfill(char(205)) << char(205) << char(187); // ╔═══╗

    gotoxy(x - 2, y);
    cout << char(186) << text << setw(length - text.length() + 1) << setfill(' ') << char(186); // ║ text ║

    gotoxy(x - 2, y + 1);
    cout << char(200) << setw(length) << setfill(char(205)) << char(205) << char(188); // ╚═══╝
}

int wherex(void){
    HANDLE hConsoleOutput;

    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
    return screen_buffer_info.dwCursorPosition.X;
}

int wherey(void){
    HANDLE hConsoleOutput;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
    return screen_buffer_info.dwCursorPosition.Y;
}

void clreol(){
    COORD coord;
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(hConsole, &info);
    coord.X = info.dwCursorPosition.X;
    coord.Y = info.dwCursorPosition.Y;
    DWORD numCharsToWrite = info.dwSize.X - info.dwCursorPosition.X;

    FillConsoleOutputCharacter(
        hConsole,
        ' ',
        numCharsToWrite,
        coord,
        &written);

    // Đặt lại con trỏ về vị trí ban đầu của dòng đó
    gotoxy(info.dwCursorPosition.X, info.dwCursorPosition.Y);
}

void ThongBao(const char *s){
    int x = wherex(), y = wherey();
    gotoxy(10, 24);
    cout << s;
    Sleep(2000);
    gotoxy(10, 24);
    clreol();
    gotoxy(x, y);
}

void ThongBaoMuon(const char *s){
    int x = wherex(), y = wherey();
    gotoxy(104, 30);
    cout << s;
    Sleep(2000);
    gotoxy(104, 30);
    clreol();
    gotoxy(x, y);
}
