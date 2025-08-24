#ifndef SACH_H
#define SACH_H

#include <iostream>
#include <string>
using namespace std;

const int MAX_DAUSACH = 10000;

struct Sach
{
    int maSach;
    int trangThai;
    string viTri;
};

struct NodeSach
{
    Sach data;
    NodeSach *next;
};

typedef NodeSach *PTRDMS;

struct DauSach
{
    string ISBN;
    string tenSach;
    int soTrang;
    string tacGia;
    int namXuatBan;
    string theLoai;
    PTRDMS dms = nullptr;
    int slmuon;
};

struct DS_DauSach
{
    int soluong = 0;
    DauSach nodes[MAX_DAUSACH];
    int maxMaSach = 0;
};

// Them sach
string toLowerStr(const string &s);
bool check_trung_ISBN(string isbn, DS_DauSach &ds);
void insertLast_Sach(PTRDMS &First, Sach data);
void insertOrder_Sach(DS_DauSach &ds, DauSach newSach);
void draw_ThemSach();
bool Enter_Sach(DS_DauSach &ds);
void ghiDanhSachDauSachRaFile(DS_DauSach &ds, const string &filename);
void read_DauSach(DS_DauSach &ds, const string &filename);
void ThemSach(DS_DauSach &ds);

// In danh sach theo the loai
void InDanhSachDauSachTheoTheLoai(DS_DauSach &ds);

// tim sach theo ten
void TimSachTheoTen(DS_DauSach &ds);
#endif