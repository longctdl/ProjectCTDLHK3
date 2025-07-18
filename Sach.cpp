#include "Sach.h"
#include "UIConsole.h"
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

vector<int> randomCodes;

void loadRandomCodes(const string &filename)
{
    ifstream file(filename);
    int code;
    while (file >> code)
    {
        randomCodes.push_back(code);
    }
    file.close();
    srand(time(NULL));
}

int getRandomCode()
{
    if (randomCodes.empty())
        return -1;

    int index = rand() % randomCodes.size();
    int code = randomCodes[index];
    randomCodes.erase(randomCodes.begin() + index);
    return code;
}

int nhapThongTinDauSach(DauSach *ds)
{
    ShowCur(true);
    clrscr();
    SetColor(14);
    CreateBoxDouble(40, 2, "   THEM DAU SACH MOI   ", 35);
    SetColor(7);

    const int MAX_FIELDS = 6;
    const int MAX_OPTIONS = MAX_FIELDS + 3; // thêm LUU, HUY, QUAY LAI

    string fields[MAX_FIELDS] = {"ISBN", "Ten sach", "So trang", "Tac gia", "Nam xuat ban", "The loai"};
    string values[MAX_FIELDS] = {"", "", "", "", "", ""};
    int y_positions[MAX_FIELDS] = {6, 8, 10, 12, 14, 16};

    int x_input = 45;
    int btn_y = 18;
    int current = 0;

    while (true)
    {
        // Hiển thị các trường nhập
        for (int i = 0; i < MAX_FIELDS; i++)
        {
            gotoxy(30, y_positions[i]);
            cout << fields[i] << ": ";
            gotoxy(x_input, y_positions[i]);
            cout << string(30, ' ') << "\b";
            gotoxy(x_input, y_positions[i]);
            cout << values[i];
        }

        // Hiển thị các nút
        string buttons[3] = {"[ LUU ]", "[ QUAY LAI ]"};
        for (int i = 0; i < 2; i++)
        {
            gotoxy(45 + i * 10, btn_y);
            if (current == MAX_FIELDS + i)
                SetColor(i == 0 ? 11 : (i == 1 ? 12 : 14));
            cout << buttons[i];
            SetColor(7);
        }

        // Đặt vị trí con trỏ chính xác
        if (current < MAX_FIELDS)
            gotoxy(x_input + values[current].length(), y_positions[current]);
        else
            gotoxy(45 + (current - MAX_FIELDS) * 10, btn_y);

        ShowCur(current < MAX_FIELDS);

        char ch = _getch();
        if (ch == -32)
        {
            ch = _getch();
            if (ch == 72) // ↑
                current = (current - 1 + MAX_OPTIONS) % MAX_OPTIONS;
            else if (ch == 80) // ↓
                current = (current + 1) % MAX_OPTIONS;
        }
        else if (ch == 13) // Enter
        {
            if (current == MAX_FIELDS) // LUU
            {
                bool valid = true;
                for (int i = 0; i < MAX_FIELDS; i++)
                {
                    if (values[i].empty())
                    {
                        gotoxy(80, y_positions[i]);
                        SetColor(12);
                        cout << "Khong duoc bo trong!";
                        SetColor(7);
                        valid = false;
                    }
                    else if ((i == 2 || i == 4) && !all_of(values[i].begin(), values[i].end(), ::isdigit))
                    {
                        gotoxy(80, y_positions[i]);
                        SetColor(12);
                        cout << "Chi nhap so!";
                        SetColor(7);
                        valid = false;
                    }
                }

                if (!valid)
                    continue;

                ds->ISBN = values[0];
                ds->tenSach = values[1];
                ds->soTrang = stoi(values[2]);
                ds->tacGia = values[3];
                ds->namXuatBan = stoi(values[4]);
                ds->theLoai = values[5];
                ds->slmuon = 0;
                ds->dms = nullptr;

                gotoxy(40, btn_y + 2);
                SetColor(10);
                cout << "Da luu dau sach thanh cong!";
                SetColor(7);
                getch();
                return 1;
            }

            else if (current == MAX_FIELDS + 1)
            {
                return -1;
            }
        }
        else if (ch == 8 && current < MAX_FIELDS)
        {
            if (!values[current].empty())
            {
                values[current].pop_back();
                gotoxy(x_input + values[current].length(), y_positions[current]);
                cout << " \b";
            }
        }
        else if (isprint(ch) && current < MAX_FIELDS)
        {
            values[current] += ch;
            cout << ch;
        }
    }
}

void ThemDauSach(DS_DauSach &ds)
{
    if (ds.soluong >= MAX_DAUSACH)
    {
        gotoxy(35, 20);
        SetColor(12);
        cout << "Danh sach da day. Khong the them!";
        SetColor(7);
        getch();
        return;
    }

    DauSach newDS;
    int result = nhapThongTinDauSach(&newDS);

    if (result == 1)
    {
        // Kiểm tra trùng ISBN
        for (int i = 0; i < ds.soluong; ++i)
        {
            if (ds.nodes[i].ISBN == newDS.ISBN)
            {
                gotoxy(35, 20);
                SetColor(12);
                cout << "ISBN da ton tai! Khong the them.";
                SetColor(7);
                getch();
                return;
            }
        }

        // Hỏi số lượng sách vật lý cần thêm
        clrscr();
        gotoxy(35, 5);
        cout << "Nhap so luong cuon sach muon them: ";
        int soLuong;
        cin >> soLuong;
        cin.ignore();

        string viTri;
        gotoxy(35, 6);
        cout << "Nhap vi tri cho cac cuon sach: ";
        getline(cin, viTri);

        // Tạo danh mục sách với mã sách tự động
        for (int i = 1; i <= soLuong; ++i)
        {
            NodeSach *node = new NodeSach;
            int maSach = getRandomCode();
            if (maSach == -1)
            {
                gotoxy(35, 12);
                SetColor(12);
                cout << "Het ma sach trong file! Khong the them.";
                SetColor(7);
                getch();
                return;
            }
            node->data.maSach = maSach;
            node->data.trangThai = 0;
            node->data.viTri = viTri;
            node->next = newDS.dms;
            newDS.dms = node;
        }

        ds.nodes[ds.soluong++] = newDS;

        gotoxy(35, 10);
        SetColor(10);
        cout << "Da them dau sach va " << soLuong << " ma sach!";
        SetColor(7);
        getch();
    }
    else if (result == 0)
    {
        // Hủy nhập – thông báo đã hiển thị
    }
    else if (result == -1)
    {
        // Quay lại – không làm gì cả
    }
}

void ghiDanhSachDauSachRaFile(DS_DauSach &ds, const string &filename)
{
    ofstream outFile(filename, ios::trunc);
    if (!outFile)
    {
        cerr << "Khong the mo file " << filename << " de ghi!" << endl;
        return;
    }

    // Tạo bản sao để sắp xếp
    DauSach *danhSachTam[MAX_DAUSACH];
    for (int i = 0; i < ds.soluong; ++i)
    {
        danhSachTam[i] = &ds.nodes[i];
    }

    // Sắp xếp tăng dần theo tenSach (Bubble Sort hoặc Selection Sort)
    for (int i = 0; i < ds.soluong - 1; ++i)
    {
        for (int j = i + 1; j < ds.soluong; ++j)
        {
            if (danhSachTam[i]->tenSach > danhSachTam[j]->tenSach)
            {
                DauSach *tmp = danhSachTam[i];
                danhSachTam[i] = danhSachTam[j];
                danhSachTam[j] = tmp;
            }
        }
    }

    // Ghi ra file theo thứ tự đã sắp xếp
    for (int i = 0; i < ds.soluong; ++i)
    {
        const DauSach &d = *danhSachTam[i];
        outFile << d.ISBN << endl;
        outFile << d.tenSach << endl;
        outFile << d.tacGia << endl;
        outFile << d.soTrang << endl;
        outFile << d.namXuatBan << endl;
        outFile << d.theLoai << endl;
        outFile << d.slmuon << endl;

        NodeSach *p = d.dms;
        while (p != nullptr)
        {
            outFile << p->data.maSach;
            if (p->next != nullptr)
                outFile << ",";
            p = p->next;
        }
        outFile << endl;

        outFile << endl;
    }

    outFile.close();
}

// -----------In Danh Sach Dau Sach--------------

void docDanhSachDauSachTuFile(const string &filename, DS_DauSach &ds)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Khong the mo file: " << filename << endl;
        return;
    }

    ds.soluong = 0;
    string line;
    while (!file.eof() && ds.soluong < MAX_DAUSACH)
    {
        DauSach d;

        getline(file, d.ISBN);
        if (d.ISBN.empty())
            break;

        getline(file, d.tenSach);
        getline(file, d.tacGia);
        getline(file, line);
        d.soTrang = stoi(line);
        getline(file, line);
        d.namXuatBan = stoi(line);
        getline(file, d.theLoai);
        getline(file, line);
        d.slmuon = stoi(line);

        getline(file, line);
        stringstream ss(line);
        string token;
        while (getline(ss, token, ','))
        {
            NodeSach *node = new NodeSach;
            node->data.maSach = stoi(token);
            node->data.trangThai = 0;
            node->data.viTri = "Chua xac dinh";
            node->next = d.dms;
            d.dms = node;
        }

        ds.nodes[ds.soluong++] = d;

        getline(file, line);
    }

    file.close();
}

// Hàm phụ để so sánh chuỗi theo thứ tự từ điển
bool soSanhTenSachTangDan(const DauSach &a, const DauSach &b)
{
    return a.tenSach < b.tenSach;
}

// Hàm đổi chỗ 2 phần tử
void hoanVi(DauSach &a, DauSach &b)
{
    DauSach temp = a;
    a = b;
    b = temp;
}

void sapXepTenSach(DauSach arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i].tenSach > arr[j].tenSach)
                hoanVi(arr[i], arr[j]);
}

bool daTonTaiTheLoai(string arr[], int soLuong, string theLoai)
{
    for (int i = 0; i < soLuong; i++)
    {
        if (arr[i] == theLoai)
            return true;
    }
    return false;
}

void InDanhSachDauSachTheoTheLoai(const DS_DauSach &ds)
{
    if (ds.soluong == 0)
    {
        ThongBao("Khong co dau sach nao!");
        return;
    }

    string dsTheLoai[MAX_DAUSACH];
    int soTheLoai = 0;

    for (int i = 0; i < ds.soluong; ++i)
    {
        string theLoai = ds.nodes[i].theLoai;
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
        cout << "Danh sach the loai:";
        SetColor(7);
        for (int i = 0; i < soTheLoai; ++i)
        {
            gotoxy(12, 4 + i);
            cout << i + 1 << ". " << dsTheLoai[i];
        }

        gotoxy(10, 6 + soTheLoai);
        cout << "Nhap STT the loai muon in (ESC de thoat): ";

        string input = "";
        char c = getch();

        if (c == 27)
            return; // ESC thoát

        // Nếu người dùng nhập số, cho phép nhập nhiều chữ số (ví dụ "10")
        while (isdigit(c))
        {
            input += c;
            cout << c;
            c = getch();
        }

        if (input.empty())
        {
            ThongBao(("Chi nhap so tu 1 den " + to_string(soTheLoai)).c_str());
            continue;
        }

        int chon = stoi(input);
        if (chon < 1 || chon > soTheLoai)
        {
            ThongBao("Lua chon khong hop le!");
            continue;
        }

        string theLoaiChon = dsTheLoai[chon - 1];

        DauSach danhSachCon[MAX_DAUSACH];
        int dem = 0;
        for (int i = 0; i < ds.soluong; ++i)
        {
            if (ds.nodes[i].theLoai == theLoaiChon)
            {
                danhSachCon[dem++] = ds.nodes[i];
            }
        }

        sapXepTenSach(danhSachCon, dem);

        clrscr();
        int x = 4, y = 2;

        SetColor(11);
        CreateBoxDouble(x, y, "The loai: " + theLoaiChon, 80);
        y += 3;

        SetColor(14);
        gotoxy(x, y++);
        cout << setw(15) << left << "ISBN"
             << setw(30) << left << "Ten sach"
             << setw(10) << left << "So trang"
             << setw(20) << left << "Tac gia"
             << setw(6) << left << "Nam";

        SetColor(7);
        for (int k = 0; k < dem; ++k)
        {
            if (y >= 28)
            {
                gotoxy(x, y++);
                ThongBao("Nhan phim bat ky de tiep tuc...");
                getch();
                clrscr();
                y = 2;
            }

            gotoxy(x, y++);
            cout << setw(15) << left << danhSachCon[k].ISBN
                 << setw(30) << left << danhSachCon[k].tenSach
                 << setw(10) << left << danhSachCon[k].soTrang
                 << setw(20) << left << danhSachCon[k].tacGia
                 << setw(6) << left << danhSachCon[k].namXuatBan;
        }

        gotoxy(x, y + 2);
        ThongBao("Da in xong danh sach! Nhan ESC de quay lai...");
        while (getch() != 27)
            ;
    }
}

// Search Sách

string NhapChuoi(int x, int y, int doDaiToiDa)
{
    string s = "";
    char c;
    while (true)
    {
        gotoxy(x + s.length(), y);
        c = getch();
        if (c == 13)
            break;
        else if (c == 27)
            return "";
        else if (c == 8)
        {
            if (!s.empty())
            {
                s.pop_back();
                gotoxy(x + s.length(), y);
                cout << " ";
                gotoxy(x + s.length(), y);
            }
        }
        else if (isprint(c) && s.length() < doDaiToiDa)
        {
            s += c;
            cout << c;
        }
    }
    return s;
}

string toLowerStr(const string &s)
{
    string result = s;
    for (char &c : result)
        c = tolower(c);
    return result;
}

void TimSachTheoTen(const DS_DauSach &ds)
{
    clrscr();
    if (ds.soluong == 0)
    {
        ThongBao("Khong co dau sach nao!");
        return;
    }

    // ==== VẼ KHUNG NHẬP TÊN SÁCH ====
    int x = 10, y = 5, w = 60;
    SetColor(11);
    CreateBoxSingle(x, y, "Nhap ten sach", w);

    SetColor(7);
    gotoxy(x + 2, y - 2);
    string tuKhoa = NhapChuoi(x + 2, y + 1, w - 4);

    string tuKhoaLower = toLowerStr(tuKhoa);

    // ==== DANH SÁCH GỢI Ý ====
    DauSach danhSachTimThay[MAX_DAUSACH];
    int soLuongTimThay = 0;

    for (int i = 0; i < ds.soluong; ++i)
    {
        string tenSachLower = toLowerStr(ds.nodes[i].tenSach);
        if (tenSachLower.find(tuKhoaLower) != string::npos)
        {
            danhSachTimThay[soLuongTimThay++] = ds.nodes[i];
        }
    }

    if (soLuongTimThay == 0)
    {
        ThongBao("Khong tim thay sach nao co ten nay!");
        return;
    }

    while (true)
    {
        clrscr();
        SetColor(11);
        CreateBoxDouble(5, 1, "CAC SACH TIM THAY", 50);
        SetColor(14);
        int x = 4, y = 4;

        gotoxy(x, y++);
        cout << setw(3) << left << "STT"
             << setw(15) << left << "ISBN"
             << setw(30) << left << "Ten sach"
             << setw(20) << left << "Tac gia"
             << setw(6) << left << "Nam"
             << setw(15) << left << "The loai";

        SetColor(7);
        for (int i = 0; i < soLuongTimThay; ++i)
        {
            gotoxy(x, y++);
            cout << setw(3) << left << i + 1
                 << setw(15) << left << danhSachTimThay[i].ISBN
                 << setw(30) << left << danhSachTimThay[i].tenSach
                 << setw(20) << left << danhSachTimThay[i].tacGia
                 << setw(6) << left << danhSachTimThay[i].namXuatBan
                 << setw(15) << left << danhSachTimThay[i].theLoai;
        }

        gotoxy(x, y + 1);
        cout << "Nhap so STT sach de xem chi tiet (ESC de thoat): ";
        int key = getch();
        if (key == 27)
            return;

        if (key >= '1' && key <= '9')
        {
            int chon = key - '0';
            if (chon > soLuongTimThay)
            {
                ThongBao("Lua chon vuot qua danh sach!");
                continue;
            }

            // ==== In thông tin chi tiết ====
            DauSach s = danhSachTimThay[chon - 1];
            clrscr();
            SetColor(11);
            CreateBoxDouble(5, 1, "THONG TIN CHI TIET", 50);
            SetColor(7);
            int y = 4;

            gotoxy(x, y++);
            cout << "ISBN       : " << s.ISBN;
            gotoxy(x, y++);
            cout << "Ten sach   : " << s.tenSach;
            gotoxy(x, y++);
            cout << "Tac gia    : " << s.tacGia;
            gotoxy(x, y++);
            cout << "Nam XB     : " << s.namXuatBan;
            gotoxy(x, y++);
            cout << "The loai   : " << s.theLoai;

            y += 2;
            SetColor(14);
            gotoxy(x, y++);
            cout << setw(5) << left << "STT"
                 << setw(20) << left << "Ma sach"
                 << setw(15) << left << "Trang thai";

            SetColor(7);
            if (s.dms == nullptr)
            {
                gotoxy(x, y++);
                cout << "Khong co cuon sach nao!";
            }
            else
            {
                PTRDMS p = s.dms;
                int stt = 1;
                while (p != nullptr)
                {
                    gotoxy(x, y++);
                    cout << setw(5) << left << stt++
                         << setw(20) << left << p->data.maSach
                         << setw(15) << left;

                    switch (p->data.trangThai)
                    {
                    case 0:
                        cout << "Co the muon";
                        break;
                    case 1:
                        cout << "Da cho muon";
                        break;
                    case 2:
                        cout << "Thanh ly";
                        break;
                    default:
                        cout << "Khong xac dinh";
                        break;
                    }

                    p = p->next;
                }
            }

            // === ĐỢI ESC ===
            gotoxy(x, y + 2);
            SetColor(10);
            cout << "Nhan ESC de quay lai danh sach...";

            while (true)
            {
                int c = getch();
                if (c == 27)
                    break; // Chỉ ESC mới thoát
            }
        }
        else
        {
            ThongBao("Chi nhap so STT trong danh sach!");
        }
    }
}
