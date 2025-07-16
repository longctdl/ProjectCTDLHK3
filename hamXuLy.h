#include <string>
#include "UIConsole.h"
#include <conio.h>

using namespace std;

const string INPUT_CANCELLED = "##USER_CANCELLED##";

string chuanHoaTen(string s){
    while(!s.empty() && s.front() == ' ') s.erase(0, 1);
    while(!s.empty() && s.back() == ' ') s.pop_back();

    for(char &c : s) c = tolower(c);
    
    bool cap = true;
    for(char &c : s){
        if(isspace(c)){
            cap = true;
        } else if(cap){
            c = toupper(c);
            cap = false;
        }
    }
    return s;
}


string inputName(int x, int y, int maxLen, bool allowSpaceOnce = false) {
    string s;
    char c;
    int spaceCount = 0;
    gotoxy(x, y);
    
    while (true) {
        c = _getch();

        if(c==27){ //ESC
            return INPUT_CANCELLED;
        }

        if (c == 13) break; // ENTER
        else if (c == 8 && !s.empty()) { //BACKSPACE
            if (s.back() == ' ') spaceCount--;
            s.pop_back();
            cout << "\b \b";
        }
        else if ((isalpha(c) || (c == ' ' && allowSpaceOnce && spaceCount < 1)) && s.length() < maxLen) {
            s += c;
            if (c == ' ') spaceCount++;
            cout << c;
        }
    }
    gotoxy(x+s.length(), y);
    return chuanHoaTen(s);
}


bool isValidGender(string s){
    for(char &c : s) c = tolower(c);
    return (s == "nam" || s == "nu");
}

