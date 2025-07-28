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
};

int nhapThongTinDauSach(DauSach *ds, const DS_DauSach &dsList);
void ThemDauSach(DS_DauSach &ds);

void ghiDanhSachDauSachRaFile(DS_DauSach &ds, const string &filename);
void docDanhSachDauSachTuFile(const string &filename, DS_DauSach &ds);
void capNhatMaSoSachTuDanhSach(DS_DauSach &ds);
void InDanhSachDauSachTheoTheLoai(DS_DauSach &ds);
void TimSachTheoTen(DS_DauSach &ds);
#endif