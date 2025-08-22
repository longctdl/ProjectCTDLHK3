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

int maSoSach = 0;
DS_DauSach danhSachCon;

string toLowerStr(const string &s)
{
    string result = s;
    for (int i = 0; i < result.length(); i++)
    {
        result[i] = tolower(result[i]);
    }
    return result;
}

void capNhatMaSoSachTuDanhSach(DS_DauSach &ds)
{
    maSoSach = 0;

    for (int i = 0; i < ds.soluong; ++i)
    {
        NodeSach *p = ds.nodes[i].dms;
        while (p != nullptr)
        {
            if (p->data.maSach > maSoSach)
                maSoSach = p->data.maSach;

            p = p->next;
        }
    }
}

int nhapThongTinDauSach(DauSach *ds, const DS_DauSach &dsList)
{
    ShowCur(true);
    clrscr();
    SetColor(14);
    CreateBoxDouble(40, 2, "   THEM DAU SACH MOI   ", 35);
    SetColor(7);

    const int MAX_FIELDS = 6;
    string labels[MAX_FIELDS] = {"ISBN", "Ten sach", "So trang", "Tac gia", "Nam xuat ban", "The loai"};
    string values[MAX_FIELDS] = {"", "", "", "", "", ""};
    int y_pos[MAX_FIELDS] = {6, 8, 10, 12, 14, 16};

    int current = 0, x_input = 45, btn_y = 18;

    while (true)
    {
        // Vẽ các nhãn và nội dung đã nhập
        for (int i = 0; i < MAX_FIELDS; i++)
        {
            gotoxy(30, y_pos[i]);
            cout << labels[i] << ": ";
            gotoxy(x_input, y_pos[i]);
            cout << setfill(' ') << setw(30) << left << values[i];
        }

        // Vẽ các nút điều khiển
        string buttons[2] = {"[ LUU ]", "[ QUAY LAI ]"};
        for (int i = 0; i < 2; i++)
        {
            gotoxy(45 + i * 12, btn_y);
            if (current == MAX_FIELDS + i)
                SetColor(i == 0 ? 11 : 12);
            cout << buttons[i];
            SetColor(7);
        }

        gotoxy(current < MAX_FIELDS ? x_input + values[current].length() : 0, current < MAX_FIELDS ? y_pos[current] : btn_y);
        ShowCur(current < MAX_FIELDS);

        char ch = _getch();

        if (ch == -32)
        {
            ch = _getch();
            current = (ch == 72)   ? (current - 1 + MAX_FIELDS + 2) % (MAX_FIELDS + 2)
                      : (ch == 80) ? (current + 1) % (MAX_FIELDS + 2)
                                   : current;
        }
        else if (ch == 13) // Enter
        {
            if (current == MAX_FIELDS) // LUU
            {
                bool valid = true;

                for (int i = 0; i < MAX_FIELDS; ++i)
                {
                    gotoxy(80, y_pos[i]);
                    cout << "                         ";
                }

                for (int i = 0; i < MAX_FIELDS; ++i)
                {
                    if (values[i].empty())
                    {
                        gotoxy(80, y_pos[i]);
                        SetColor(12);
                        cout << "Khong duoc bo trong!";
                        SetColor(7);
                        valid = false;
                    }
                    else if (i == 0) // Kiểm tra ISBN
                    {
                        if (!all_of(values[i].begin(), values[i].end(), ::isdigit))
                        {
                            gotoxy(80, y_pos[i]);
                            SetColor(12);
                            cout << "ISBN chi duoc chua so!";
                            SetColor(7);
                            valid = false;
                        }
                        else if (values[i].length() != 5)
                        {
                            gotoxy(80, y_pos[i]);
                            SetColor(12);
                            cout << "ISBN phai co 5 ky tu!";
                            SetColor(7);
                            valid = false;
                        }
                    }
                    else if ((i == 2 || i == 4) && !all_of(values[i].begin(), values[i].end(), ::isdigit))
                    {
                        gotoxy(80, y_pos[i]);
                        SetColor(12);
                        cout << "Chi nhap so!";
                        SetColor(7);
                        valid = false;
                    }
                }

                // Kiểm tra ISBN trùng
                for (int i = 0; i < dsList.soluong; i++)
                {
                    if (dsList.nodes[i].ISBN == values[0])
                    {
                        gotoxy(80, y_pos[0]);
                        SetColor(12);
                        cout << "ISBN da ton tai!";
                        SetColor(7);
                        valid = false;
                        break;
                    }
                }

                if (!valid)
                    continue;

                // Lưu dữ liệu
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
            else if (current == MAX_FIELDS + 1) // QUAY LAI
            {
                return -1;
            }
        }
        else if (ch == 8 && current < MAX_FIELDS) // Backspace
        {
            if (!values[current].empty())
            {
                values[current].pop_back();
                gotoxy(x_input + values[current].length(), y_pos[current]);
                cout << " \b";
            }
        }
        else if (isprint(ch) && current < MAX_FIELDS)
        {
            if (values[current].empty() && ch == ' ')
                continue;

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
    int result = nhapThongTinDauSach(&newDS, ds); // Truyền ds để kiểm tra trùng ISBN

    if (result != 1){
        return; // Không lưu hoặc quay lại
        cout<<right;
    }

    clrscr();
    gotoxy(35, 5);
    cout << "Nhap so luong cuon sach muon them: ";
    int soLuong;
    while (!(cin >> soLuong) || soLuong <= 0)
    {
        cin.clear();
        cin.ignore(100, '\n');
        gotoxy(35, 6);
        SetColor(12);
        cout << "Vui long nhap so nguyen duong!      ";
        gotoxy(35, 5);
        SetColor(7);
        cout << "Nhap so luong cuon sach muon them: ";
    }
    cin.ignore();

    gotoxy(35, 7);
    cout << "Nhap vi tri cho cac cuon sach: ";
    string viTri;
    getline(cin, viTri);
    while (viTri.empty() || viTri.find_first_not_of(' ') == string::npos)
    {
        gotoxy(35, 8);
        SetColor(12);
        cout << "Vi tri khong duoc de trong!       ";
        SetColor(7);

        gotoxy(35, 7);
        cout << "Nhap vi tri cho cac cuon sach: ";
        getline(cin, viTri);
    }

    for (int i = 1; i <= soLuong; ++i)
    {
        NodeSach *node = new NodeSach;
        node->data.maSach = ++maSoSach;
        node->data.trangThai = 0;
        node->data.viTri = viTri;
        node->next = newDS.dms;
        newDS.dms = node;
    }

    // Chèn vào danh sách đã sắp xếp theo tên sách (tăng dần)
    int pos = ds.soluong;
    while (pos > 0 && toLowerStr(ds.nodes[pos - 1].tenSach) > toLowerStr(newDS.tenSach))
    {
        ds.nodes[pos] = ds.nodes[pos - 1];
        pos--;
    }
    ds.nodes[pos] = newDS;
    ds.soluong++;

    gotoxy(35, 10);
    SetColor(10);
    cout << "Da them dau sach va " << soLuong << " ma sach!";
    SetColor(7);
    getch();
}

//=========================Doc Ghi File============================
void ghiDanhSachDauSachRaFile(DS_DauSach &ds, const string &filename)
{
    ofstream outFile(filename, ios::trunc);
    if (!outFile)
    {
        cerr << "Khong the mo file " << filename << " de ghi!" << endl;
        return;
    }

    for (int i = 0; i < ds.soluong; ++i)
    {
        DauSach &d = ds.nodes[i];

        outFile << d.ISBN << '\n';
        outFile << d.tenSach << '\n';
        outFile << d.tacGia << '\n';
        outFile << d.soTrang << '\n';
        outFile << d.namXuatBan << '\n';
        outFile << d.theLoai << '\n';
        outFile << d.slmuon << '\n';

        NodeSach *p = d.dms;
        while (p != nullptr)
        {
            outFile << p->data.maSach << "|" << p->data.viTri << "|" << p->data.trangThai << '\n';
            p = p->next;
        }

        outFile << "#\n";
    }

    outFile.close();
}

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
    int maxMa = 0;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        DauSach d;
        d.ISBN = line;
        getline(file, d.tenSach);
        getline(file, d.tacGia);

        getline(file, line);
        d.soTrang = stoi(line);
        getline(file, line);
        d.namXuatBan = stoi(line);
        getline(file, d.theLoai);
        getline(file, line);
        d.slmuon = stoi(line);

        d.dms = nullptr;

        // Đọc từng mã sách cho đến khi gặp dấu "#"
        while (getline(file, line))
        {
            if (line == "#")
                break;

            stringstream ss(line);
            string token;
            NodeSach *node = new NodeSach;

            getline(ss, token, '|');
            node->data.maSach = stoi(token);
            maxMa = max(maxMa, node->data.maSach);

            getline(ss, node->data.viTri, '|');
            getline(ss, token, '|');
            node->data.trangThai = stoi(token);

            node->next = d.dms;
            d.dms = node;
        }

        ds.nodes[ds.soluong++] = d;
    }

    maSoSach = maxMa;
    file.close();
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

        gotoxy(x, y + 1);
        ThongBao("Nhan ESC de quay lai danh sach the loai...");
        while (getch() != 27)
            ;
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
