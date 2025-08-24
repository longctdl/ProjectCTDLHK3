#include "Sach.h"
#include "UIConsole.h"
#include "hamXuLy.h"
#include <iostream>
#include <conio.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <ctime>
#include <string>
#include <sstream>
#include <cstring>
#include <iomanip>

using namespace std;

int maSoSach = 0;
DS_DauSach danhSachCon;

string toLowerStr(const string &s){
    string result = s;
    for (int i = 0; i < result.length(); i++)
    {
        result[i] = tolower(result[i]);
    }
    return result;
}

bool check_trung_ISBN(string isbn, DS_DauSach &ds){
    for(int i = 0; i<ds.soluong; i++){
        if(ds.nodes[i].ISBN == isbn) return true;
    }
    return false;
}

void insertLast_Sach(PTRDMS &First, Sach data){
    PTRDMS node = new NodeSach{data, nullptr};
    if(First == nullptr) First = node;
    else{
        PTRDMS p = First;
        while(p->next != nullptr){
            p = p->next;
        }
        p->next = node;
    }

}

void insertOrder_Sach(DS_DauSach &ds, DauSach newSach){
    if(ds.soluong >= MAX_DAUSACH){
        ThongBao("Danh sach da day khong the them sach!");
        return;
    }

    int pos = ds.soluong;
    while(pos > 0 && toLowerStr(ds.nodes[pos -1].tenSach) > toLowerStr(newSach.tenSach)){
        ds.nodes[pos] = ds.nodes[pos - 1];
        pos--;
    }
    ds.nodes[pos] = newSach;
    ds.soluong++;
}

void draw_ThemSach(){
    clrscr();
    SetColor(14);
    CreateBoxDouble(40, 1, "   THEM SACH   ", 15);
    SetColor(7);

    gotoxy(30, 5); cout<<"ISBN: ";
    gotoxy(30, 6); cout<<"Ten sach: ";
    gotoxy(30, 7); cout<<"So trang: ";
    gotoxy(30, 8); cout<<"Tac gia: ";
    gotoxy(30, 9); cout<<"Nam xuat ban: ";
    gotoxy(30, 10); cout<<"The loai: ";
    gotoxy(30, 11); cout<<"So luong: ";
    gotoxy(30, 12); cout<<"Vi tri: ";

    gotoxy(20, 17); cout<<"Nhan [ESC] de thoat";
}

bool Enter_Sach(DS_DauSach &ds){
    ShowCur(true);
    draw_ThemSach();
    
    const int INPUT_ISBN = 30 + string("ISBN: ").length();
    const int INPUT_BOOK = 30 + string("Ten sach: ").length();
    const int INPUT_SL_TRANG = 30 + string("So trang: ").length();
    const int INPUT_TACGIA = 30 + string("Tac gia: ").length();
    const int INPUT_NAM = 30 + string("Nam xuat ban: ").length();
    const int INPUT_THELOAI = 30 + string("The loai: ").length();
    const int INPUT_SOLUONG = 30 + string("So luong: ").length();
    const int INPUT_VITRI = 30 + string("Vi tri: ").length();
    string ISBN, tenSach, soTrang, tacGia, namXuatBan, theLoai, soLuong, viTri;

    while(true){
        gotoxy(INPUT_ISBN, 5); cout<<string(10, ' ');
        ISBN = inputNumber(INPUT_ISBN, 5, 5);
        if(ISBN == INPUT_CANCELLED){
            ShowCur(false);
            cout<<right;
            return false;
        }
        if(ISBN.empty()){
            ThongBao("Vui long nhap ma ISBN!");
            continue;
        }
        if(ISBN.length() != 5){
            ThongBao("Ma ISBN phai la 5 chu so!");
            continue;
        }
        if(check_trung_ISBN(ISBN, ds)){
            ThongBao("Ma ISBN nay da duoc su dung!");
            continue;
        }
        break;
    }

    while(true){
        gotoxy(INPUT_BOOK, 6); cout<<string(100, ' ');
        tenSach = inputName(INPUT_BOOK, 6, 100, true);
        if(tenSach == INPUT_CANCELLED){
            ShowCur(false);
            cout<<right;
            return false;
        }
        if(tenSach.empty()){
            ThongBao("Vui long nhap ten sach!");
            continue;
        }
        break;
    }

    while(true){
        gotoxy(INPUT_SL_TRANG, 7); cout<<string(10, ' ');
        soTrang = inputNumber(INPUT_SL_TRANG, 7, 6);
        if(soTrang == INPUT_CANCELLED){
            ShowCur(false);
            cout<<right;
            return false;
        }
        if(soTrang.empty() || stoi(soTrang) <= 0){
            ThongBao("So trang phai la so nguyen duong!");
            continue;
        }
        if(stoi(soTrang) < 10 || stoi(soTrang) > 2000){
            ThongBao("So trang khong hop le (10–2000)!");
            continue;
        }
        break;
    }

    while(true){
        gotoxy(INPUT_TACGIA, 8); cout<<string(50, ' ');
        tacGia = inputName(INPUT_TACGIA, 8, 50, true);
        if(tacGia == INPUT_CANCELLED){
            ShowCur(false);
            cout<<right;
            return false;
        }
        if(tacGia.empty()){
            ThongBao("Vui long nhap ten tac gia!");
            continue;
        }
        break;
    }

    while(true){
        gotoxy(INPUT_NAM, 9); cout<<string(10, ' ');
        namXuatBan = inputNumber(INPUT_NAM, 9, 4);
        if(namXuatBan == INPUT_CANCELLED){
            ShowCur(false);
            cout<<right;
            return false;
        }
        if(namXuatBan.empty()){
            ThongBao("Vui long nhap nam xuat ban!");
            continue;
        }
        int nam = stoi(namXuatBan);
        int currentYear = 2025;
        if(nam < 1000 || nam > currentYear){
            ThongBao("Nam xuat ban khong hop le!");
            continue;
        }
        break;
    }

    while(true){
        gotoxy(INPUT_THELOAI, 10); cout<<string(50, ' ');
        theLoai = inputName(INPUT_THELOAI, 10, 50, true);
        if(theLoai == INPUT_CANCELLED){
            ShowCur(false);
            cout<<right;
            return false;
        }
        if(theLoai.empty()){
            ThongBao("Vui long nhap the loai!");
            continue;
        }
        break;
    }

    while(true){
        gotoxy(INPUT_SOLUONG, 11); cout<<string(6, ' ');
        soLuong = inputNumber(INPUT_SOLUONG, 11, 6);
        if(soLuong == INPUT_CANCELLED){
            ShowCur(false);
            cout<<right;
            return false;
        }
        if(soLuong.empty() || stoi(soLuong) <= 0){
            ThongBao("So luong phai la so nguyen duong va khong duoc bo trong!");
            continue;
        }
        break;
    }

    while(true){
        gotoxy(INPUT_VITRI, 12); cout<<string(50, ' ');
        viTri = inputViTri(INPUT_VITRI, 12, 50);
        if(viTri == INPUT_CANCELLED){
            ShowCur(false);
            cout<<right;
            return false;
        }
        if(viTri.empty()){
            ThongBao("Vui long nhap vi tri cua sach!");
            continue;
        }
        break;
    }

    gotoxy(30, 13); cout<<"Ban co chac muon them sach nay khong? (y/n): ";
    char confirmChar = GetYesNoInput(30 + string("Ban co chan muon them sach nay khong? (y/n): ").length(), 13);
    if(confirmChar == 'y'){
        DauSach newSach;
        newSach.ISBN = ISBN;
        newSach.tenSach = chuanHoaTen(tenSach);
        newSach.soTrang = stoi(soTrang);
        newSach.tacGia = chuanHoaTen(tacGia);
        newSach.namXuatBan = stoi(namXuatBan);
        newSach.theLoai = chuanHoaTen(theLoai);
        newSach.slmuon = 0;

        Sach newNode;
        for(int i =0; i < stoi(soLuong); i++){
            newNode.maSach = ++ds.maxMaSach;
            newNode.trangThai = 0;
            newNode.viTri = chuanHoaTen(viTri);

            insertLast_Sach(newSach.dms, newNode);
        }
        insertOrder_Sach(ds, newSach);
        ghiDanhSachDauSachRaFile(ds, "txt\\DanhSachDauSach.txt");
        ThongBao("Them sach thanh cong!");
        ShowCur(false);
        return true;
    } else {
        ShowCur(false);
        ThongBao("DA HUY THAO TAC THEM SACH!");
        cout<<right;
        return false;
    }
}

void ghiDanhSachDauSachRaFile(DS_DauSach &ds, const string &filename){
    ofstream outFile(filename, ios::trunc);
    if (!outFile){
        cerr << "Khong the mo file " << filename << " de ghi!" << endl;
        return;
    }

    outFile << ds.maxMaSach << '\n';
    for (int i = 0; i < ds.soluong; ++i){
        DauSach &d = ds.nodes[i];

        outFile << d.ISBN << '\n';
        outFile << d.tenSach << '\n';
        outFile << d.tacGia << '\n';
        outFile << d.soTrang << '\n';
        outFile << d.namXuatBan << '\n';
        outFile << d.theLoai << '\n';
        outFile << d.slmuon << '\n';

        PTRDMS p = d.dms;
        while (p != nullptr){
            outFile << p->data.maSach << "|" << p->data.viTri << "|" << p->data.trangThai << '\n';
            p = p->next;
        }

        outFile << "#\n";
    }

    outFile.close();
}

void read_DauSach(DS_DauSach &ds, const string &filename){
    ifstream file(filename);
    if (!file.is_open()){
        cerr << "Khong the mo file: " << filename << endl;
        return;
    }

    string line, token;
    Sach tmp;

    if(!getline(file, line)) return;
    ds.maxMaSach = stoi(line);

    while(getline(file, line)){
        if(line.empty()) continue;

        DauSach d;
        d.ISBN = line;
        getline(file, d.tenSach);
        getline(file, d.tacGia);

        getline(file, line); d.soTrang = stoi(line);
        getline(file, line); d.namXuatBan = stoi(line);
        getline(file, d.theLoai);
        getline(file, line); d.slmuon = stoi(line);

        while (getline(file, line) && line != "#") {
            stringstream ss(line);
            getline(ss, token, '|'); tmp.maSach = stoi(token);
            getline(ss, tmp.viTri, '|');
            getline(ss, token, '|'); tmp.trangThai = stoi(token);
            insertLast_Sach(d.dms, tmp);
        }

        ds.nodes[ds.soluong++] = d; 
    }
}

void ThemSach(DS_DauSach &ds){
    if(ds.soluong >= MAX_DAUSACH){
        ThongBao("Danh sach da day khong the them!");
        cout<<right;
        return;
    }
    while(true){
        if(!Enter_Sach(ds)) break;
    }
}

//============================In Danh Sach=============================

string ChuanHoaChuoi(string s)
{
    while (!s.empty() && s.front() == ' ')
        s.erase(s.begin());
    while (!s.empty() && s.back() == ' ')
        s.pop_back();

    bool vietHoa = true;
    for (int i = 0; i < s.length(); i++)
    {
        if (isspace(s[i]))
        {
            vietHoa = true;
        }
        else if (vietHoa)
        {
            s[i] = toupper(s[i]);
            vietHoa = false;
        }
        else
        {
            s[i] = tolower(s[i]);
        }
    }
    return s;
}

void hoanVi(DauSach &a, DauSach &b)
{
    DauSach temp = a;
    a = b;
    b = temp;
}

bool daTonTaiTheLoai(string arr[], int soLuong, string theLoai)
{
    theLoai = ChuanHoaChuoi(theLoai);
    for (int i = 0; i < soLuong; i++)
    {
        if (ChuanHoaChuoi(arr[i]) == theLoai)
            return true;
    }
    return false;
}

PTRDMS cloneDanhMucSach(PTRDMS src)
{
    if (src == NULL)
        return NULL;

    PTRDMS head = new NodeSach;
    head->data = src->data;
    head->next = NULL;

    PTRDMS p = head;
    src = src->next;

    while (src != NULL)
    {
        PTRDMS node = new NodeSach;
        node->data = src->data;
        node->next = NULL;
        p->next = node;
        p = node;
        src = src->next;
    }

    return head;
}

int demSoSach(PTRDMS dms)
{
    int count = 0;
    while (dms != NULL)
    {
        count++;
        dms = dms->next;
    }
    return count;
}

void InDanhSachDauSachTheoTheLoai(DS_DauSach &ds)
{
    if (ds.soluong == 0)
    {
        ThongBao("Khong co dau sach nao!");
        return;
    }

    // Tạo danh sách các thể loại duy nhất
    string dsTheLoai[MAX_DAUSACH];
    int soTheLoai = 0;

    for (int i = 0; i < ds.soluong; ++i)
    {
        string theLoai = ChuanHoaChuoi(ds.nodes[i].theLoai);
        if (!daTonTaiTheLoai(dsTheLoai, soTheLoai, theLoai))
        {
            dsTheLoai[soTheLoai++] = theLoai;
        }
    }

    while (true)
    {
        clrscr();
        SetColor(11);
        gotoxy(10, 2);
        ShowCur(true);
        cout << "Chon the loai muon in:";
        SetColor(7);

        for (int i = 0; i < soTheLoai; ++i)
        {
            gotoxy(12, 4 + i);
            cout << i + 1 << ". " << dsTheLoai[i];
        }

        gotoxy(10, 6 + soTheLoai);
        cout << "Nhap so (ESC de thoat): ";

        string input = "";
        char c;

        while (true)
        {
            c = getch();
            if (c == 27)
                return; // ESC
            if (c == '\r')
                break; // Enter
            if (isdigit(c))
            {
                input += c;
                cout << c;
            }
        }

        if (input.empty())
            continue;

        int chon = stoi(input);
        if (chon < 1 || chon > soTheLoai)
        {
            ThongBao("Lua chon khong hop le!");
            continue;
        }

        string theLoaiChon = dsTheLoai[chon - 1];

        // Tạo danh sách con theo thể loại
        danhSachCon.soluong = 0;

        for (int i = 0; i < ds.soluong; ++i)
        {
            if (ChuanHoaChuoi(ds.nodes[i].theLoai) == theLoaiChon)
            {
                int viTri = danhSachCon.soluong;

                // Tạo bản sao từng trường (tránh copy con trỏ)
                danhSachCon.nodes[viTri].ISBN = ds.nodes[i].ISBN;
                danhSachCon.nodes[viTri].tenSach = ds.nodes[i].tenSach;
                danhSachCon.nodes[viTri].soTrang = ds.nodes[i].soTrang;
                danhSachCon.nodes[viTri].tacGia = ds.nodes[i].tacGia;
                danhSachCon.nodes[viTri].namXuatBan = ds.nodes[i].namXuatBan;
                danhSachCon.nodes[viTri].theLoai = ds.nodes[i].theLoai;
                danhSachCon.nodes[viTri].dms = cloneDanhMucSach(ds.nodes[i].dms);

                danhSachCon.soluong++;
            }
        }

        if (danhSachCon.soluong == 0)
        {
            ThongBao("Khong co dau sach nao thuoc the loai nay!");
            continue;
        }

        // In danh sách
        clrscr();
        int x = 3, y = 2;
        SetColor(11);
        CreateBoxDouble(x, y, " THE LOAI: " + theLoaiChon + " ", 100);
        y += 3;

        // Vẽ dòng trên bảng
        gotoxy(x, y++);
        cout << char(218) << string(6, char(196)) << char(194)
             << string(15, char(196)) << char(194)
             << string(30, char(196)) << char(194)
             << string(20, char(196)) << char(194)
             << string(6, char(196)) << char(194)
             << string(14, char(196)) << char(191);

        // Tiêu đề cột
        cout << setfill(' ');
        gotoxy(x, y++);
        cout << char(179) << setw(6) << left << " STT " << char(179)
             << setw(15) << left << " ISBN" << char(179)
             << setw(30) << left << " Ten sach" << char(179)
             << setw(20) << left << " Tac gia" << char(179)
             << setw(6) << left << " Nam" << char(179)
             << setw(14) << left << " So luong sach" << char(179);
        // Dòng phân cách
        gotoxy(x, y++);
        cout << char(195) << string(6, char(196)) << char(197)
             << string(15, char(196)) << char(197)
             << string(30, char(196)) << char(197)
             << string(20, char(196)) << char(197)
             << string(6, char(196)) << char(197)
             << string(14, char(196)) << char(180);

        // In dữ liệu
        for (int i = 0; i < danhSachCon.soluong; ++i)
        {
            if (y >= 27)
            {
                gotoxy(x, y++);
                ThongBao("Nhan phim bat ky de tiep tuc...");
                getch();
                clrscr();
                y = 2;
            }

            DauSach &d = danhSachCon.nodes[i];
            gotoxy(x, y++);
            cout << setfill(' ') // đặt lại fill là khoảng trắng
                 << char(179) << " " << setw(5) << left << i + 1 << char(179)
                 << setw(15) << left << d.ISBN << char(179)
                 << setw(30) << left << d.tenSach << char(179)
                 << setw(20) << left << d.tacGia << char(179)
                 << setw(6) << left << d.namXuatBan << char(179)
                 << setw(14) << left << demSoSach(d.dms) << char(179);
        }

        // Vẽ dòng dưới bảng
        gotoxy(x, y++);
        cout << char(192) << string(6, char(196)) << char(193)
             << string(15, char(196)) << char(193)
             << string(30, char(196)) << char(193)
             << string(20, char(196)) << char(193)
             << string(6, char(196)) << char(193)
             << string(14, char(196)) << char(217);

        gotoxy(x, y + 1); cout << "Nhan ESC de quay lai danh sach the loai...";
        while (getch() != 27);
    }
}

// Search====================

string NhapChuoi_SearchSach(int x, int y, int maxLength)
{
    string s = "";
    char c;

    while (true)
    {
        // gotoxy(x, y);
        cout << left << s << " ";
        // gotoxy(x + s.length(), y);
        c = getch();

        if (c == 13)
            break; // ENTER
        else if (c == 27)
            return "";                 // ESC
        else if (c == 8 && !s.empty()) // Backspace
        {
            s.pop_back();
        }
        else if (isdigit(c))
        {
            ThongBao("Khong duoc nhap so!");
            continue;
        }
        else if (isprint(c) && s.length() < maxLength)
        {
            s += c;
        }
    }

    return s;
}
void TimSachTheoTen(DS_DauSach &ds)
{
    if (ds.soluong == 0)
    {
        ThongBao("Khong co dau sach nao!");
        return;
    }

    string tuKhoa = "";
    char c;
    int x = 5, y = 4;
    int w = 60;
    int chon = 0;
    int trangHienTai = 0;
    const int soDongMoiTrang = 10;

    while (true)
    {
        clrscr();
        SetColor(11);
        CreateBoxDouble(x, 1, "TIM KIEM TEN SACH", 100);

        SetColor(14);
        gotoxy(x, y);
        cout << "Nhap ten sach: ";
        cout << setfill(' ') << setw(w) << left << tuKhoa << " ";

        // ====== Tìm kiếm ======
        string tuKhoaLower = toLowerStr(tuKhoa);
        int chiSoTimThay[MAX_DAUSACH];
        int soLuongTimThay = 0;

        for (int i = 0; i < ds.soluong; ++i)
        {
            string tenSachLower = toLowerStr(ds.nodes[i].tenSach);
            if (tenSachLower.find(tuKhoaLower) != string::npos)
            {
                chiSoTimThay[soLuongTimThay++] = i;
            }
        }

        // Số trang
        int tongTrang = (soLuongTimThay + soDongMoiTrang - 1) / soDongMoiTrang;
        if (tongTrang == 0)
            tongTrang = 1;
        if (trangHienTai >= tongTrang)
            trangHienTai = tongTrang - 1;

        // Chỉ số bắt đầu của trang hiện tại
        int batDau = trangHienTai * soDongMoiTrang;
        int ketThuc = min(batDau + soDongMoiTrang, soLuongTimThay);

        // Giữ con trỏ chọn trong trang
        chon = min(chon, ketThuc - batDau - 1);
        if (chon < 0)
            chon = 0;

        // ====== Header bảng kết quả ======
        int yTable = y + 2;
        SetColor(7);
        gotoxy(x, yTable++);
        cout << char(218) << string(7, char(196)) << char(194)
             << string(15, char(196)) << char(194)
             << string(30, char(196)) << char(194)
             << string(20, char(196)) << char(194)
             << string(6, char(196)) << char(194)
             << string(15, char(196)) << char(191);

        gotoxy(x, yTable++);
        cout << setfill(' ') << char(179) << setw(7) << left << "STT" << char(179)
             << setw(15) << left << "ISBN" << char(179)
             << setw(30) << left << "Ten sach" << char(179)
             << setw(20) << left << "Tac gia" << char(179)
             << setw(6) << left << "Nam" << char(179)
             << setw(15) << left << "The loai" << char(179);

        gotoxy(x, yTable++);
        cout << char(195) << string(7, char(196)) << char(197)
             << string(15, char(196)) << char(197)
             << string(30, char(196)) << char(197)
             << string(20, char(196)) << char(197)
             << string(6, char(196)) << char(197)
             << string(15, char(196)) << char(180);

        // ====== In danh sách kết quả ======
        for (int i = batDau; i < ketThuc; ++i)
        {
            int index = chiSoTimThay[i];
            DauSach &s = ds.nodes[index];

            gotoxy(x, yTable++);
            if (i - batDau == chon)
                SetBGColor(3);
            else
                SetBGColor(0);

            cout << setfill(' ')
                 << char(179) << setw(7) << left << i + 1 << char(179)
                 << setw(15) << left << s.ISBN << char(179)
                 << setw(30) << left << s.tenSach << char(179)
                 << setw(20) << left << s.tacGia << char(179)
                 << setw(6) << left << s.namXuatBan << char(179)
                 << setw(15) << left << s.theLoai << char(179);
        }
        SetBGColor(0);

        gotoxy(x, yTable + 1);
        SetColor(10);
        cout << "ESC: Thoat | ENTER: Chi tiet | BACKSPACE: Xoa | UP-DOWN: Dieu huong | LEFT-RIGHT: Trang ("
             << trangHienTai + 1 << "/" << tongTrang << ")";

        // ====== Xử lý phím ======
        c = _getch();
        if (c == -32 || c == 224)
        {
            c = _getch();
            if (c == 72 && chon > 0) // Up
                chon--;
            else if (c == 80 && chon < ketThuc - batDau - 1) // Down
                chon++;
            else if (c == 75 && trangHienTai > 0) // Left
            {
                trangHienTai--;
                chon = 0;
            }
            else if (c == 77 && trangHienTai < tongTrang - 1) // Right
            {
                trangHienTai++;
                chon = 0;
            }
            continue;
        }
        else if (c == 27) // ESC
            return;
        else if (c == 8 && !tuKhoa.empty()) // Backspace
        {
            tuKhoa.pop_back();
            chon = 0;
            trangHienTai = 0;
        }
        else if (c == 13) // Enter
        {
            if (soLuongTimThay == 0)
            {
                ThongBao("Khong co ket qua nao!");
                continue;
            }

            int index = chiSoTimThay[batDau + chon];
            DauSach &s = ds.nodes[index];

            clrscr();
            SetColor(11);
            CreateBoxDouble(5, 1, "CHI TIET DAU SACH", 70);
            SetColor(14);
            gotoxy(x, y);
            cout << "ISBN       : " << s.ISBN;
            gotoxy(x, y + 1);
            cout << "Ten sach   : " << s.tenSach;
            gotoxy(x, y + 2);
            cout << "Tac gia    : " << s.tacGia;
            gotoxy(x, y + 3);
            cout << "Nam XB     : " << s.namXuatBan;
            gotoxy(x, y + 4);
            cout << "The loai   : " << s.theLoai;

            int yStart = y + 6;
            gotoxy(x, yStart++);
            SetColor(11);
            cout << "Danh sach ma sach trong thu vien:";
            SetColor(7);

            PTRDMS p = s.dms;
            while (p != nullptr)
            {
                gotoxy(x, yStart++);
                cout << "- Ma sach: " << p->data.maSach << " | Trang thai: ";
                switch (p->data.trangThai)
                {
                case 0:
                    cout << "Co san";
                    break;
                case 1:
                    cout << "Da cho muon";
                    break;
                case 2:
                    cout << "Het Sach";
                    break;
                default:
                    cout << "Khong ro";
                    break;
                }
                p = p->next;
            }

            gotoxy(x, yStart + 1);
            SetColor(10);
            cout << "Nhan phim bat ky de quay lai...";
            _getch();
        }
        else if (isprint(c))
        {
            tuKhoa += c;
            chon = 0;
            trangHienTai = 0;
        }
    }
}
