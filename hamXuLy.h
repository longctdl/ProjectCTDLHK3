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


string inputName(int x, int y, int maxLen, bool allowSpace = false) {
    string s;
    char c;
    gotoxy(x, y);
    
    while (true) {
        c = _getch();

        if(c==27){ //ESC
            return INPUT_CANCELLED;
        }

        if (c == 13) break; // ENTER
        else if (c == 8 && !s.empty()) { //BACKSPACE
            cout << "\b \b";
            s.pop_back();
        }
        else if(isalpha(c) && s.length() < maxLen){
            s+= c;
            cout << c;
        }
        else if (c == ' ' && s.length() < maxLen) {
            if(allowSpace){
                if(!s.empty() && s.back() != ' '){
                    s+= c;
                    cout<<c;
                }
            }
        }
    }
    gotoxy(x+s.length(), y);
    return chuanHoaTen(s);
}

string inputNumber(int x, int y, int maxLen) {
    string s;
    char c;
    gotoxy(x, y);
    
    while (true) {
        c = _getch();

        if (c == 27) { // ESC key
            return INPUT_CANCELLED;
        }

        if (c == 13) { // Enter key
            break; // Stop input and return the string
        } else if (c == 8 && !s.empty()) { // Backspace key
            cout << "\b \b"; 
            s.pop_back();
        }
        else if (isdigit(c) && s.length() < maxLen) {
            s += c;
            cout << c;
        }
        
    }
    
    return s;
}


bool isValidGender(string s){
    for(char &c : s) c = tolower(c);
    return (s == "nam" || s == "nu");
}

