#include <iostream>
#include <conio.h>
#include "Sach.h"
#include "DocGia.h"
#include "UIConsole.h"

using namespace std;

#define UP 72
#define DOWN 80
#define ENTER 13

void giaoDienInDanhSachDocGia(TREE_DOCGIA &root)
{
    clrscr();
    ShowCur(false);
    SetColor(14);
    gotoxy(35, 2);
    cout << "=== IN DANH SACH DOC GIA ===";
    SetColor(7);

    const char *menu[] = {
        "1. In DS theo ten/ho tang dan",
        "2. In DS theo ma the tang dan",
        "3. Quay lai"};
    int soCN = sizeof(menu) / sizeof(char *);
    int chon = 0;

    while (true)
    {
        ShowCur(false);
        for (int i = 0; i < soCN; ++i)
        {
            if (i == chon)
            {
                SetColor(0);
                SetBGColor(11);
            }
            else
            {
                SetColor(15);
                SetBGColor(0);
            }
            CreateBoxDouble(35, 5 + i * 3, menu[i], 30);
            SetColor(15);
            SetBGColor(0);
        }
        int key = _getch();
        if (key == 224)
        {
            key = _getch();
            if (key == UP)
                chon = (chon - 1 + soCN) % soCN;
            if (key == DOWN)
                chon = (chon + 1) % soCN;
        }
        else if (key == ENTER)
        {
            clrscr();
            if (chon == 0)
            { // Sap xep theo ten + ho
                XuLyInDanhSachDocGia(root, true);
            }
            else if (chon == 1)
            { // Sap xep theo ma the
                XuLyInDanhSachDocGia(root, false);
            }
            else if (chon == soCN - 1)
            {
                break;
            }
            clrscr();
            SetColor(14);
            gotoxy(35, 2);
            cout << "=== IN DANH SACH DOC GIA ===";
            SetColor(7);
        }
    }
    clrscr();
    ShowCur(true);
}

void giaoDienQuanLyDocGia(TREE_DOCGIA &root)
{
    clrscr();
    ShowCur(false);
    SetColor(14);
    gotoxy(35, 2);
    cout << "=== QUAN LY THE DOC GIA ===";
    SetColor(7);

    const char *menu[] = {
        "1. Them the doc gia",
        "2. Sua thong tin doc gia",
        "3. Xoa the doc gia",
        "4. In danh sach doc gia",
        "5. Quay lai"};
    int soCN = sizeof(menu) / sizeof(char *);
    int chon = 0;

    while (true)
    {
        ShowCur(false);
        for (int i = 0; i < soCN; ++i)
        {
            if (i == chon)
            {
                SetColor(0);
                SetBGColor(11);
            }
            else
            {
                SetColor(15);
                SetBGColor(0);
            }
            CreateBoxDouble(35, 5 + i * 3, menu[i], 30);
            SetColor(15);
            SetBGColor(0);
        }
        int key = _getch();
        if (key == 224)
        {
            key = _getch();
            if (key == UP)
                chon = (chon - 1 + soCN) % soCN;
            if (key == DOWN)
                chon = (chon + 1) % soCN;
        }
        else if (key == ENTER)
        {
            clrscr();
            if (chon == 0)
            {
                ThemDocGia(root);
            }
            else if (chon == 1)
            { // Sua doc gia
                SuaDocGia(root);
            }
            else if (chon == 2)
            { // xoa doc gia
                XoaDocGia(root);
            }
            else if (chon == 3)
            {
                giaoDienInDanhSachDocGia(root);
            }
            else if (chon == soCN - 1)
            {
                break;
            }
            clrscr();
            SetColor(14);
            gotoxy(35, 2);
            cout << "=== QUAN LY THE DOC GIA ===";
            SetColor(7);
        }
    }
    clrscr();
    ShowCur(true);
}

void giaoDienQuanLyDauSach(DS_DauSach &dsDauSach)
{
    clrscr();
    ShowCur(false);
    SetColor(14);
    gotoxy(35, 2);
    cout << "=== QUAN LY DAU SACH ===";
    SetColor(7);

    const char *menu[] = {
        "1. Them dau sach",
        "2. In danh sach dau sach",
        "3. Tim kiem sach theo Ten",
        "4. Quay lai"};
    int soCN = sizeof(menu) / sizeof(char *);
    int chon = 0;

    while (true)
    {
        ShowCur(false);
        for (int i = 0; i < soCN; ++i)
        {
            if (i == chon)
            {
                SetColor(0);
                SetBGColor(11);
            }
            else
            {
                SetColor(15);
                SetBGColor(0);
            }
            CreateBoxDouble(35, 5 + i * 3, menu[i], 35);
            SetColor(15);
            SetBGColor(0);
        }

        int key = _getch();
        if (key == 224)
        {
            key = _getch();
            if (key == UP)
                chon = (chon - 1 + soCN) % soCN;
            if (key == DOWN)
                chon = (chon + 1) % soCN;
        }
        else if (key == ENTER)
        {
            clrscr();
            if (chon == 0)
            {
                docDanhSachDauSachTuFile("txt\\DanhSachDauSach.txt", dsDauSach);
                ThemDauSach(dsDauSach);
                ghiDanhSachDauSachRaFile(dsDauSach, "txt\\DanhSachDauSach.txt");
                gotoxy(35, 10);
                getch();
            }
            else if (chon == 1)
            {
                // Gọi hàm in danh sách đầu sách
                docDanhSachDauSachTuFile("txt\\DanhSachDauSach.txt", dsDauSach);
                InDanhSachDauSachTheoTheLoai(dsDauSach);
                gotoxy(35, 10);
                getch();
            }
            else if (chon == 2)
            {
                // Gọi hàm tìm kiếm sách theo TenTen
                docDanhSachDauSachTuFile("txt\\DanhSachDauSach.txt", dsDauSach);
                TimSachTheoTen(dsDauSach);
                gotoxy(35, 10);
                getch();
            }
            else if (chon == soCN - 1)
            {
                break;
            }

            clrscr();
            SetColor(14);
            gotoxy(35, 2);
            cout << "=== QUAN LY DAU SACH ===";
            SetColor(7);
        }
    }
    clrscr();
    ShowCur(true);
}

void menu(TREE_DOCGIA &root, DS_DauSach &dsDauSach)
{
    clrscr();
    ShowCur(false);
    gotoxy(30, 2);
    SetColor(14);
    cout << "=== CHUONG TRINH QUAN LY THU VIEN ===";
    SetColor(7);
    const char *chucNang[] = {
        "Quan ly the doc gia",
        "Quan ly dau sach",
        "Muon sach",
        "Tra sach",
        "Cac sach doc gia dang muon",
        "Thong ke top 10 sach",
        "Thoat"};
    int chon = 0;
    int soCN = sizeof(chucNang) / sizeof(char *);

    while (true)
    {
        ShowCur(false);
        for (int i = 0; i < soCN; ++i)
        {
            if (i == chon)
            {
                SetColor(0);
                SetBGColor(11);
            }
            else
            {
                SetColor(15);
                SetBGColor(0);
            }
            CreateBoxDouble(35, 5 + i * 3, chucNang[i], 30);
            SetColor(15);
            SetBGColor(0);
        }

        int key = _getch();
        if (key == 224)
        {
            key = _getch();
            if (key == UP)
                chon = (chon - 1 + soCN) % soCN;
            if (key == DOWN)
                chon = (chon + 1) % soCN;
        }
        else if (key == ENTER)
        {
            clrscr();
            if (chon == 0)
            {
                giaoDienQuanLyDocGia(root);
            }
            else if (chon == 1)
            {
                giaoDienQuanLyDauSach(dsDauSach);
            }
            else if (chon == 2)
            { // Muon sach
            }
            else if (chon == 3)
            { // Tra sach
            }
            else if (chon == 4)
            {
                ShowCur(true);
                print_DsDangMuon(root, dsDauSach);
                getch();
            }
            else if (chon == 5)
            {
            }
            else if (chon == soCN - 1)
            {
                freeBST(root);
                return;
            }
            else
            {
                gotoxy(35, 8);
                cout << "Chuc nang nay chua lam!";
                getch();
            }
            clrscr();
            SetColor(14);
            gotoxy(30, 2);
            cout << "=== CHUONG TRINH QUAN LY THU VIEN ===";
            SetColor(7);
        }
    }
}

int main()
{
    resizeConsole(800, 600);
    fixConsoleWindowSize(100, 30);
    DS_DauSach ds;

    TREE_DOCGIA root = nullptr;
    read_File(root, "txt\\DanhSachDocGia.txt");
    menu(root, ds);
    freeBST(root);
}