#ifndef DOCGIA_H
#define DOCGIA_H

#include <iostream>
#include <string>
#include "Sach.h"

struct Date{
    int ngay;
    int thang;
    int nam;
};

struct MuonTra{
    int maSach;
    Date ngayMuon;
    Date ngayTra;
    int trangThai;
};

struct NodeMT{
    MuonTra data;
    NodeMT* next = nullptr;
};

typedef NodeMT* PTRMT;

struct TheDocGia{
    int maThe;
    string ho;
    string ten;
    string gioitinh;
    int trangThai;
    PTRMT dsMuonTra = nullptr;
};

struct NodeTheDocGia{
    TheDocGia data;
    NodeTheDocGia* left= nullptr;
    NodeTheDocGia* right= nullptr;
};

typedef NodeTheDocGia* TREE_DOCGIA;

int read_MaThe(int dsMaThe[], const char* filename = "txt\\MaTheDocGia.txt");
void write_MaThe(int dsMaThe[], int n, const char* filename = "txt\\MaTheDocGia.txt");
int get_MaThe(int dsMaThe[], int &n);
bool Enter_DocGia(TheDocGia &docgia);
void InsertNodeDocGia(TREE_DOCGIA &root, TheDocGia data);
void ThemDocGia(TREE_DOCGIA &root);
void write_DSDocGia(const TheDocGia &docgia, const char* filename = "txt\\DanhSachDocGia.txt");
int countNodeDocGia(TREE_DOCGIA root);

#endif