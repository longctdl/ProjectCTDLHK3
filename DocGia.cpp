#include "DocGia.h"
#include "UIConsole.h"
#include "hamXuLy.h"
#include <iostream>
#include <fstream>

using namespace std;
const int MAX_MATHE = 10000;

int read_MaThe(int dsMaThe[], const char* filename){
    ifstream fin(filename);
    int maThe, n = 0;
    while(fin >> maThe && n < MAX_MATHE){
        dsMaThe[n++] = maThe;
    }
    fin.close();
    return n;
}

void write_MaThe(int dsMaThe[], int n, const char* filename){
    ofstream fout(filename);
    for(int i = 0 ; i < n; i++){
        fout<< dsMaThe[i] << endl;
    }
    fout.close();
}

int get_MaThe(int dsMaThe[], int &n){
    if(n==0) return -1;
    int mid = n/2;
    int maThe = dsMaThe[mid];
    for(int i = mid; i< n-1; i++){
        dsMaThe[i] = dsMaThe[i+1];
    }
    n--;
    write_MaThe(dsMaThe, n);
    return maThe;
}

bool Enter_DocGia(TheDocGia &docgia) {
    ShowCur(true);
    clrscr();

    // --- Vẽ giao diện tĩnh ---
    SetColor(14);
    CreateBoxDouble(40, 2, "   THEM DOC GIA MOI   ", 25);
    SetColor(7);

    gotoxy(15, 5); cout << char(218) << string(70, char(196)) << char(191);
    for (int i = 6; i < 14; ++i) { 
        gotoxy(15, i); cout << char(179);
        gotoxy(86, i); cout << char(179);
    }
    gotoxy(15, 14); cout << char(192) << string(70, char(196)) << char(217);

    SetColor(8);
    gotoxy(20, 15); cout << "Meo: Nhan phim [ESC] de huy bo va thoat.";
    SetColor(7);

    gotoxy(20, 7);  cout << "Ho         : ";
    gotoxy(20, 9);  cout << "Ten        : ";
    gotoxy(20, 11); cout << "Gioi tinh  : ";
    gotoxy(55, 11); cout<<"(Nam/Nu)";

    const int INPUT_X= 33;

    string ho, ten, gioitinh;

    while (true) {
        ho = inputName(INPUT_X, 7, 20, true);
        if(ho == INPUT_CANCELLED) return false;
        if (!ho.empty()) break;
        ThongBao("Ho khong duoc de trong. Vui long nhap lai!");
    }

    while (true) {
        ten = inputName(INPUT_X, 9, 20, false);
        if(ten == INPUT_CANCELLED) return false;
        if (!ho.empty()) break;
        ThongBao("Ten khong duoc de trong. Vui long nhap lai!");
    }

    while (true) {
        gioitinh = inputName(INPUT_X, 11, 3, false);
        if(gioitinh == INPUT_CANCELLED) return false;
        if (isValidGender(gioitinh)) {
            gotoxy(55, 11); cout << "          ";
            break; 
        } else {
            ThongBao("Gioi tinh khong hop le. Chi duoc nhap 'Nam' hoac 'Nu'.");
            gotoxy(INPUT_X, 11);
            cout << "       ";
        }
    }

    docgia.ho = ho;
    docgia.ten = ten;
    docgia.gioitinh = chuanHoaTen(gioitinh);
    docgia.trangThai = 1;
    docgia.dsMuonTra = nullptr;

    ShowCur(false);
    return true;
}

void InsertNodeDocGia(TREE_DOCGIA &root, TheDocGia data){
    if(root == nullptr){
        root = new NodeTheDocGia{data, nullptr, nullptr};
        return;
    }
    if(data.maThe < root->data.maThe) InsertNodeDocGia(root->left, data);
    else if(data.maThe > root->data.maThe) InsertNodeDocGia(root->right, data);
}

void ThemDocGia(TREE_DOCGIA &root){
    TheDocGia docgia;
    if(!Enter_DocGia(docgia)){
        ThongBao("DA HUY THAO TAC THEM DOC GIA");
        ShowCur(false);
        return;
    }

    int dsMaThe[MAX_MATHE];
    int n = read_MaThe(dsMaThe);
    int maThe = get_MaThe(dsMaThe, n);

    if(maThe == -1){
        ThongBao("DA HET MA THE, KHONG THE THEM DOC GIA MOI");
        return;
    }

    docgia.maThe = maThe;
    InsertNodeDocGia(root, docgia);
    write_DSDocGia(docgia);
    SetColor(10);
    ThongBao("THEM DOC GIA THANH CONG!");
    SetColor(7);
}

void write_DSDocGia(const TheDocGia &docgia, const char* filename){
    ofstream fout(filename, ios::app);
    if(!fout.is_open()){
        ThongBao("Khong the mo file de ghi danh sach doc gia!");
        return;
    }

    fout<< docgia.maThe << "\n";
    fout<< docgia.ho << "\n";
    fout<< docgia.ten << "\n";
    fout<< docgia.gioitinh << "\n";
    fout<< docgia.trangThai << "\n";
    fout<< "\n";

    fout.close();
}

void read_DSDocGia(TREE_DOCGIA &root, const char* filename){
    ifstream fin(filename);
    if(!fin.is_open()){
        ThongBao("Khong the mo file danh sach doc gia!");
        return;
    }

    while(!fin.eof()){
        TheDocGia docgia;
        string maTheStr, trangThaiStr;

        getline(fin, maTheStr);
        if(maTheStr.empty()) break;
        docgia.maThe = stoi(maTheStr);

        getline(fin, docgia.ho);
        getline(fin, docgia.ten);
        getline(fin, docgia.gioitinh);
        getline(fin, trangThaiStr);
        docgia.trangThai = stoi(trangThaiStr);
        docgia.dsMuonTra = nullptr;

        string emptyLine;
        getline(fin, emptyLine);
        InsertNodeDocGia(root, docgia);
    }
    fin.close();
}

int countNodeDocGia(TREE_DOCGIA root){
    if(root == nullptr) return 0;
    return 1 + countNodeDocGia(root->left) + countNodeDocGia(root->right);
}