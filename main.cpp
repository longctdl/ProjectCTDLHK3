#include <iostream>
#include <conio.h>
#include "Sach.h"
#include "DocGia.h"
#include "UIConsole.h"

using namespace std;

#define UP 72
#define DOWN 80
#define ENTER 13


void giaoDienInDanhSachDocGia(TREE_DOCGIA &root){
    clrscr();
    ShowCur(false);
    SetColor(14);
    gotoxy(35, 2); cout << "=== IN DANH SACH DOC GIA ===";
    SetColor(7);

    const char* menu[] ={
        "1. In DS theo ten/ho tang dan",
        "2. In DS theo ma the tang dan",
        "3. Quay lai"
    };
    int soCN = sizeof(menu) / sizeof(char*);
    int chon = 0;

    while (true) {
        ShowCur(false);
        for (int i = 0; i < soCN; ++i) {
            if (i == chon) {
                SetColor(0); SetBGColor(11);
            } else {
                SetColor(15); SetBGColor(0);
            }
            CreateBoxDouble(35, 5 + i * 3, menu[i], 30);
            SetColor(15); SetBGColor(0);
        }
        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == UP) chon = (chon - 1 + soCN) % soCN;
            if (key == DOWN) chon = (chon + 1) % soCN;
        } else if (key == ENTER) {
            clrscr();
            if(chon == 0){ //Sap xep theo ten + ho
                XuLyInDanhSachDocGia(root, true);
            } else if(chon == 1){ // Sap xep theo ma the
                XuLyInDanhSachDocGia(root, false);
            }
            else if(chon == soCN -1){
                break;
            } else{
                gotoxy(35, 8);
                cout << "Chuc nang nay chua lam!";
                getch();
            }
            clrscr();
            SetColor(14);
            gotoxy(35, 2); cout << "=== IN DANH SACH DOC GIA ===";
            SetColor(7);
        }
    }
    clrscr();
    ShowCur(true);
}

void giaoDienQuanLyDocGia(TREE_DOCGIA &root) {
    clrscr();
    ShowCur(false);
    SetColor(14);
    gotoxy(35, 2); cout << "=== QUAN LY THE DOC GIA ===";
    SetColor(7);

    const char* menu[] = {
        "1. Them the doc gia",
        "2. Sua thong tin doc gia",
        "3. Xoa the doc gia",
        "4. In danh sach doc gia",
        "5. Quay lai"
    };
    int soCN = sizeof(menu) / sizeof(char*);
    int chon = 0;

    while (true) {
        ShowCur(false);
        for (int i = 0; i < soCN; ++i) {
            if (i == chon) {
                SetColor(0); SetBGColor(11);
            } else {
                SetColor(15); SetBGColor(0);
            }
            CreateBoxDouble(35, 5 + i * 3, menu[i], 30);
            SetColor(15); SetBGColor(0);
        }
        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == UP) chon = (chon - 1 + soCN) % soCN;
            if (key == DOWN) chon = (chon + 1) % soCN;
        } else if (key == ENTER) {
            clrscr();
            if(chon == 0){
                ThemDocGia(root);
            } else if(chon == 3){
                giaoDienInDanhSachDocGia(root);
            }
            else if(chon == soCN -1){
                break;
            } else{
                gotoxy(35, 8);
                cout << "Chuc nang nay chua lam!";
                getch();
            }
            clrscr();
            SetColor(14);
            gotoxy(35, 2); cout << "=== QUAN LY THE DOC GIA ===";
            SetColor(7);
        }
    }
    clrscr();
    ShowCur(true);
}

void menu(TREE_DOCGIA &root) {
    clrscr();
    ShowCur(false);
    gotoxy(30, 2); SetColor(14); cout << "=== CHUONG TRINH QUAN LY THU VIEN ==="; SetColor(7);
    const char* chucNang[] = {
        "Quan ly the doc gia",
        "Quan ly dau sach",
        "Muon sach",
        "Tra sach",
        "Thong ke",
        "Thoat"
    };
    int chon = 0;
    int soCN = sizeof(chucNang) / sizeof(char*);

    while (true) {
        // clrscr();
        ShowCur(false);
        for (int i = 0; i < soCN; ++i) {
            if (i == chon) {
                SetColor(0); SetBGColor(11);
            } else {
                SetColor(15); SetBGColor(0);
            }
            CreateBoxDouble(35, 5 + i * 3, chucNang[i], 30);
            SetColor(15); SetBGColor(0);
        }

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == UP) chon = (chon - 1 + soCN) % soCN;
            if (key == DOWN) chon = (chon + 1) % soCN;
        } else if (key == ENTER) {
            clrscr();
            if (chon == 0) {
                giaoDienQuanLyDocGia(root);
            } else if (chon == soCN - 1) {
                return;
            } else {
                gotoxy(35, 8);
                cout << "Chuc nang nay chua lam!";
                getch();
            }
            clrscr();
            SetColor(14);
            gotoxy(30, 2); cout << "=== CHUONG TRINH QUAN LY THU VIEN ===";
            SetColor(7);
        }
    }
}

int main(){
    resizeConsole(800, 600);
    fixConsoleWindowSize(100, 30);
    DS_DauSach dsDauSach;
    TREE_DOCGIA root = nullptr;
    read_DSDocGia(root);
    menu(root);
}