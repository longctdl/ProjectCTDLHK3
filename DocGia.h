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

TREE_DOCGIA createNode(TheDocGia docgia);
void InsertNode(TREE_DOCGIA &root, TheDocGia docgia);
int getMaTheTuFile(const char* filename);
void VeKhungDanhSach();
void VeKhungNhapLieu();
void XoaKhungNhapLieu();
bool Enter_DocGia(TheDocGia &docgia);
void HienThiTrangDocGia(TheDocGia** Array, int page, int totalPages, int totalNode);
TheDocGia** Insert_Order(TheDocGia** oldArray, int oldSize, TheDocGia* newElement);
void ThemDocGia(TREE_DOCGIA &root);
void freeBST(TREE_DOCGIA &root);

// luu danh sach doc gia va danh sach muon tra
string formatDate(const Date& d);
void write_DSMuonTra(ofstream &fout, PTRMT First);
void write_Docgia(ofstream &fout, const TheDocGia &docgia);
void save_File(TREE_DOCGIA root, const char* filename);

// doc danh sach doc gia va danh sach muon tra vao file
Date parseDate(const string &str);
PTRMT read_DSMuonTra(ifstream &fin);
bool read_Docgia(ifstream &fin, TheDocGia &docgia);
void read_File(TREE_DOCGIA &root, const char* filename);

//ham ho tro hien thi danh sach va phan trang
int countNodeDocGia(TREE_DOCGIA root);
void collectDocGia(TREE_DOCGIA root, TheDocGia dsDocGia[], int& index);
void InsertTreeToArray(TREE_DOCGIA root, TheDocGia** Array, int& index);
bool SortDocGiaByName(const TheDocGia* a, const TheDocGia* b);
void QuickSortDocGia(TheDocGia* A[], int left, int right);
void HienThiDanhSachDocGia(TheDocGia** Array, int page, int totalPages, int totalNode);
void XuLyInDanhSachDocGia(TREE_DOCGIA &root, bool sortByName);

// Xoa Doc Gia
TREE_DOCGIA Search(TREE_DOCGIA root, int maThe);
TREE_DOCGIA findMin(TREE_DOCGIA root);
bool check_MuonSach(PTRMT ds);
TREE_DOCGIA Delete_DocGia(TREE_DOCGIA root, int maThe);
void XoaDocGia(TREE_DOCGIA &root);

// Sua Doc Gia
void SuaDocGia(TREE_DOCGIA &root);
void HienThiFormSua(int x, int y, const string &ho, const string &ten, const string &gioitinh, int trangthai, int currentField);


// Muon sach
bool check_QuaHan(const Date& ngayMuon);
bool check_Muon(TheDocGia& docgia);
void InsertMuonTra(TheDocGia& docgia, int maSach, Date ngayMuon);
bool updateSach(DS_DauSach& ds, int maSach, int newTrangThai);
void draw_GiaoDienMuonSach();
void HienThi_List_Sach(Sach** Array, DS_DauSach& ds, int page, int totalPages, int totalNode);
void draw_EnterMuon();
void Delete_KhungEnter_Muon();
bool Enter_Muon(TREE_DOCGIA root, DS_DauSach& ds, TREE_DOCGIA &pDocGia, int &maSachOut, int &IndexDauSach);
int count_List_Sach(DS_DauSach& ds);
void InsertSachToArray(DS_DauSach& ds, Sach** Array, int &index);
void BorrowBook(TREE_DOCGIA root, DS_DauSach& ds);
void muonSach(TREE_DOCGIA root, DS_DauSach& ds);


// In danh sach muon cua doc gia
string Search_NameBook(DS_DauSach &ds, int maSach);
int countNodeMuon(PTRMT First);
void collectMuon(PTRMT First, MuonTra dsMuon[], int &index);
void draw_GiaoDienXemDanhSachMuon();
void XoaKhuVucMuonTra(int startX, int startY);
void HienThiDanhSachMuon(MuonTra** Array, DS_DauSach &ds, int page, int totalPages, int totalNode, int startX, int startY);
void print_DsDangMuon(TREE_DOCGIA root, DS_DauSach &ds);
#endif