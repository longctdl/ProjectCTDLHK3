#include "DocGia.h"
#include "UIConsole.h"
#include "hamXuLy.h"
#include "Sach.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>
#include <queue>

using namespace std;
// const int MAX_MATHE = 10000;

TREE_DOCGIA createNode(TheDocGia docgia){
    TREE_DOCGIA newNode = new NodeTheDocGia;
    newNode->data = docgia;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}


void InsertNode(TREE_DOCGIA &root, TheDocGia docgia){
    if(root == nullptr){
        root = createNode(docgia);
        return;
    }
    if(docgia.maThe < root->data.maThe) InsertNode(root->left, docgia);
    else if(docgia.maThe > root->data.maThe) InsertNode(root->right, docgia);
}

int getMaTheTuFile(const char* filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "Không thể mở file mã thẻ!" << endl;
        return -1;
    }
    string maTheStr;
    getline(fin, maTheStr);
    if (maTheStr.empty()) {
        cout << "Hết mã thẻ để cấp!" << endl;
        fin.close();
        return -1;
    }
    string cacMaTheConLai;
    string dong;
    while (getline(fin, dong)) {
        cacMaTheConLai += dong + "\n";
    }
    fin.close();

    ofstream fout(filename);
    if (!fout) {
        cout << "Không thể ghi vào file mã thẻ!" << endl;
        return -1;
    }
    fout << cacMaTheConLai;
    fout.close();

    return stoi(maTheStr);
}

void VeKhungDanhSach() {
    SetColor(14); // Màu vàng
    CreateBoxDouble(40, 1, "   THEM DOC GIA   ", 25);
    cout << setfill(' ');
    SetColor(7); // Màu trắng
    
    gotoxy(4, 3);
    cout << char(218) << string(6, char(196)) << char(194) << string(10, char(196)) << char(194)
         << string(30, char(196)) << char(194) << string(12, char(196)) << char(194)
         << string(15, char(196)) << char(191);
    
    gotoxy(4, 4);
    cout << char(179) << " " << left << setw(5) << "STT" << char(179) << " " << left << setw(9) << "Ma The"
         << char(179) << " " << left << setw(29) << "Ho va Ten" << char(179) << " " << left << setw(11) << "Gioi Tinh"
         << char(179) << " " << left << setw(14) << "Trang Thai" << char(179);
    
    gotoxy(4, 5);
    cout << char(195) << string(6, char(196)) << char(197) << string(10, char(196)) << char(197)
         << string(30, char(196)) << char(197) << string(12, char(196)) << char(197)
         << string(15, char(196)) << char(180);
}

void VeKhungNhapLieu() {
    SetColor(14);
    CreateBoxDouble(97, 2, "   NHAP THONG TIN   ", 20);
    SetColor(7);

    gotoxy(87, 5); cout << char(218) << string(130 - 87 - 1, char(196)) << char(191);
    for (int i = 6; i < 14; ++i) { 
        gotoxy(87, i); cout << char(179);
        gotoxy(130, i); cout << char(179);
    }
    gotoxy(87, 14); cout << char(192) << string(130 - 87 - 1, char(196)) << char(217);

    gotoxy(92, 7);  cout << "Ho         : ";
    gotoxy(92, 9);  cout << "Ten        : ";
    gotoxy(92, 11); cout << "Gioi tinh  : ";
    gotoxy(117, 11); cout << "(Nam/Nu)";

    SetColor(8);
    gotoxy(94, 15); cout << "Meo: Nhan [ESC] de huy bo.";
    SetColor(7);
}

void XoaKhungNhapLieu() {
    for (int i = 0; i <= 17; ++i) {
        gotoxy(87, i);
        cout << string(45, ' ');
    }
}

bool Enter_DocGia(TheDocGia &docgia) {
    ShowCur(true);
    VeKhungNhapLieu();

    const int INPUT_X = 105;
    string ho, ten, gioitinh;

    while (true) {
        ho = inputName(INPUT_X, 7, 25, true);
        if (ho == INPUT_CANCELLED) { ShowCur(false); return false; }
        if (!ho.empty()) break;
        ThongBao("Ho khong duoc de trong. Vui long nhap lai!");
    }
    while (true) {
        ten = inputName(INPUT_X, 9, 10, false);
        if (ten == INPUT_CANCELLED) { ShowCur(false); return false; }
        if (!ten.empty()) break;
        ThongBao("Ten khong duoc de trong. Vui long nhap lai!");
    }
    while (true) {
        gioitinh = inputName(INPUT_X, 11, 3, false);
        if (gioitinh == INPUT_CANCELLED) { ShowCur(false); return false; }
        if (isValidGender(gioitinh)) {
            gotoxy(117, 11); cout << "         ";
            break;
        } else {
            ThongBao("Gioi tinh khong hop le. Chi duoc nhap 'Nam' hoac 'Nu'.");
            gotoxy(INPUT_X, 11); cout << "      ";
            gotoxy(INPUT_X, 11);
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

void HienThiTrangDocGia(TheDocGia** Array, int page, int totalPages, int totalNode) {
    const int ITEMS_PER_PAGE = 15;
    const int START_Y = 6;
    
    for (int i = 0; i < ITEMS_PER_PAGE + 1; ++i) {
        gotoxy(4, START_Y + i);
        cout << string(80, ' ');
    }

    int startIndex = page * ITEMS_PER_PAGE;
    int endIndex = min(startIndex + ITEMS_PER_PAGE, totalNode);
    
    for (int i = startIndex; i < endIndex; ++i) {
        int row = START_Y + (i - startIndex);
        gotoxy(4, row); cout << char(179);
        cout << " " << left << setw(5) << i + 1 << char(179);
        cout << " " << left << setw(9) << Array[i]->maThe << char(179);
        string hoTen = Array[i]->ho + " " + Array[i]->ten;
        cout << " " << left << setw(29) << hoTen << char(179);
        cout << " " << left << setw(11) << Array[i]->gioitinh << char(179);
        string trangThai = (Array[i]->trangThai == 1) ? "Hoat dong" : "Bi khoa";
        cout << " " << left << setw(14) << trangThai << char(179);
    }
    
    int soDongDaIn = endIndex - startIndex;
    if (soDongDaIn > 0) {
        gotoxy(4, START_Y + soDongDaIn);
        cout << char(192) << string(6, char(196)) << char(193) << string(10, char(196)) << char(193)
             << string(30, char(196)) << char(193) << string(12, char(196)) << char(193)
             << string(15, char(196)) << char(217);
    }
    
    gotoxy(35, 23); cout << "Trang " << page + 1 << " / " << totalPages;
}

TheDocGia** Insert_Order(TheDocGia** oldArray, int oldSize, TheDocGia* newElement){
    int insert_pos = 0;
    string newHoTen = newElement->ten + " " + newElement->ho;
    string oldHoTen;
    while(insert_pos < oldSize){
        oldHoTen = oldArray[insert_pos]->ten + " " + oldArray[insert_pos]->ho;
        if(newHoTen < oldHoTen) break;
        insert_pos++;
    }

    TheDocGia** newArray = new TheDocGia*[oldSize + 1];
    for(int i = 0; i<insert_pos; i++) newArray[i] = oldArray[i];

    newArray[insert_pos] = newElement;

    for(int i = insert_pos; i < oldSize; i++) newArray[i+1] = oldArray[i];
    return newArray;
}

void ThemDocGia(TREE_DOCGIA &root) {
    clrscr();
    ShowCur(false);

    int totalNode = countNodeDocGia(root);
    TheDocGia** Array = new TheDocGia*[totalNode];
    int index = 0;
    InsertTreeToArray(root, Array, index);
    QuickSortDocGia(Array, 0, totalNode -1);

    const int ITEMS_PER_PAGE = 15;
    int currentPage = 0;
    int totalPages = (totalNode == 0) ? 1 : (totalNode - 1) / ITEMS_PER_PAGE + 1;
    
    VeKhungDanhSach();
    HienThiTrangDocGia(Array, currentPage, totalPages, totalNode);

    SetColor(8);
    gotoxy(10, 22); cout << "[<-] [->]: Chuyen trang | [ENTER]: Them moi | [ESC]: Thoat";
    SetColor(7);

    while (true) {
        int key = _getch();

        if (key == 224) {
            key = _getch();
            if (key == 75 && currentPage > 0) {
                currentPage--;
            } else if (key == 77 && currentPage < totalPages - 1) {
                currentPage++;
            }
            HienThiTrangDocGia(Array, currentPage, totalPages, totalNode);
        } else if (key == 13) { // Phím ENTER
            TheDocGia docgia;
            if (Enter_DocGia(docgia)) {
                int maThe = getMaTheTuFile("txt\\MaTheDocGia.txt");
                if (maThe == -1) {
                    ThongBao("DA HET MA THE, KHONG THE THEM DOC GIA MOI");
                } else {
                    docgia.maThe = maThe;
                    InsertNode(root, docgia);
                    save_File(root, "txt\\DanhSachDocGia.txt");
                    ThongBao("THEM DOC GIA THANH CONG!");

                    TheDocGia** newArray = Insert_Order(Array, totalNode, &docgia);
                    delete[] Array;
                    Array = newArray;
                    totalNode++;
                    totalPages = (totalNode == 0) ? 1 : (totalNode - 1) / ITEMS_PER_PAGE + 1;
                    currentPage = 0;
                }
            } else {
                // Người dùng hủy bằng ESC
                ThongBao("DA HUY THAO TAC THEM DOC GIA");
            }
            XoaKhungNhapLieu();
            cout << setfill(' ');
            HienThiTrangDocGia(Array, currentPage, totalPages, totalNode);
        } else if (key == 27) { // Phím ESC -> Thoát
            break;
        }
    }

    delete[] Array;
    cout << right;
}

void freeBST(TREE_DOCGIA &root){
    if(root == nullptr) return;
    freeBST(root->left);
    freeBST(root->right); 
    delete root;
    root = nullptr;
}

string formatDate(const Date& d){
    if(d.ngay == -1 && d.thang == -1 && d.nam == -1) return "-1/-1/-1";
    return to_string(d.ngay) + "/" + to_string(d.thang) + "/" + to_string(d.nam);
}

void write_DSMuonTra(ofstream &fout, PTRMT First){
    PTRMT p = First;
    while(p!=nullptr){
        MuonTra mt = p->data;
        fout << mt.maSach << " "
             << formatDate(mt.ngayMuon) << " "
             << formatDate(mt.ngayTra) << " "
             << mt.trangThai << endl;
        p = p->next;
    }
}

void write_Docgia(ofstream &fout, const TheDocGia &docgia){
    fout << docgia.maThe << endl;
    fout << docgia.ho << endl;
    fout << docgia.ten << endl;
    fout << docgia.gioitinh << endl;
    fout << docgia.trangThai << endl;

    write_DSMuonTra(fout, docgia.dsMuonTra);

    fout<<endl;
}

void save_File(TREE_DOCGIA root, const char* filename){
    ofstream fout(filename);
    if(!fout.is_open()){
        cout<<"Khong the mo file de ghi!"<<endl;
        return;
    }

    queue<TREE_DOCGIA> q;
    if(root != nullptr) q.push(root);

    while(!q.empty()){
        TREE_DOCGIA node = q.front(); q.pop();
        write_Docgia(fout, node->data);

        if(node->left) q.push(node->left);
        if(node->right) q.push(node->right);
    }

    fout.close();
}

Date parseDate(const string &str){
    Date d;
    if(str == "-1/-1/-1"){
        d = {-1, -1, -1};
        return d;
    }
    sscanf(str.c_str(), "%d/%d/%d", &d.ngay, &d.thang, &d.nam);
    return d;
}

PTRMT read_DSMuonTra(ifstream &fin){
    PTRMT First = nullptr, tail = nullptr;
    string line;
    while(getline(fin, line)){
        if(line.empty()) break;

        stringstream ss(line);
        MuonTra mt;
        string ngayMuonStr, ngayTraStr;
        ss >> mt.maSach >> ngayMuonStr >> ngayTraStr >> mt.trangThai;
        mt.ngayMuon = parseDate(ngayMuonStr);
        mt.ngayTra = parseDate(ngayTraStr);

        PTRMT node = new NodeMT;
        node->data = mt;
        node->next = nullptr;
        if(First == nullptr) First = tail = node;
        else {
            tail->next = node;
            tail = node;
        }
    }
    return First;
}

bool read_Docgia(ifstream &fin, TheDocGia &docgia){
    string line;

    if (!getline(fin, line)) return false;  docgia.maThe = stoi(line);
    if (!getline(fin, docgia.ho)) return false;
    if (!getline(fin, docgia.ten)) return false;
    if (!getline(fin, docgia.gioitinh)) return false;
    if (!getline(fin, line)) return false; docgia.trangThai = stoi(line);

    docgia.dsMuonTra = read_DSMuonTra(fin);

    return true;
}

void read_File(TREE_DOCGIA &root, const char* filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout<<"Khong mo duoc fil danh sach doc gia!"<<endl;
        return;
    }

    while(!fin.eof()){
        TheDocGia docgia;
        if(read_Docgia(fin, docgia)) InsertNode(root, docgia);
    }
    fin.close();
}

int countNodeDocGia(TREE_DOCGIA root){
    if(root == nullptr) return 0;
    return 1 + countNodeDocGia(root->left) + countNodeDocGia(root->right);
}

void collectDocGia(TREE_DOCGIA root, TheDocGia dsDocGia[], int& index){
    if(root == nullptr) return;
    collectDocGia(root->left, dsDocGia, index);
    dsDocGia[index++] = root->data;
    collectDocGia(root->right, dsDocGia, index);
}

void InsertTreeToArray(TREE_DOCGIA root, TheDocGia** Array, int& index){
    if(root == nullptr) return;
    InsertTreeToArray(root->left, Array, index);
    Array[index] = &(root->data);
    index++;
    InsertTreeToArray(root->right, Array, index);
}

bool SortDocGiaByName(const TheDocGia* a, const TheDocGia* b){
    if(a->ten != b->ten) return a->ten < b->ten;
    return a->ho < b->ho;
}

void QuickSortDocGia(TheDocGia* A[], int left, int right){
    int i = left;
    int j = right;

    TheDocGia* mid = A[(left + right) / 2];
    while(i<=j){
        while(SortDocGiaByName(A[i], mid)) i++; //A[i] < mid
        while(SortDocGiaByName(mid, A[j])) j--; //A[j] > mid

        if(i<=j){
            swap(A[i], A[j]);
            i++;
            j--;
        }
    }

    if(left < j) QuickSortDocGia(A,left,j);
    if(i < right) QuickSortDocGia(A, i, right);
}

void HienThiDanhSachDocGia(TheDocGia** Array, int page, int totalPages, int totalNode) {
    const int ITEMS_PER_PAGE = 15;
    
    for(int i = 0; i < ITEMS_PER_PAGE + 2; i++){
        gotoxy(4, 6 + i);
        clreol();
        cout<<string(80, ' ');
    }

    int startIndex = page * ITEMS_PER_PAGE;
    int endIndex = min(startIndex + ITEMS_PER_PAGE, totalNode);
    
    for (int i = startIndex; i < endIndex; ++i) {
        int row = 6 + (i - startIndex);
        gotoxy(4, row); cout << char(179);
        cout << " " << left << setw(5) << i + 1 << char(179);
        cout << " " << left << setw(9) << Array[i]->maThe << char(179);
        string hoTen = Array[i]->ho + " " + Array[i]->ten;
        cout << " " << left << setw(29) << hoTen << char(179);
        cout << " " << left << setw(11) << Array[i]->gioitinh << char(179);
        string trangThai = (Array[i]->trangThai == 1) ? "Hoat dong" : "Bi khoa";
        cout << " " << left << setw(14) << trangThai << char(179);
    }
    
    int soDongDaIn = endIndex - startIndex;
    int viTriChanBangY = 6 + soDongDaIn;
    gotoxy(4, viTriChanBangY);
    cout << char(192) << string(6, char(196)) << char(193) << string(10, char(196)) << char(193)
         << string(30, char(196)) << char(193) << string(12, char(196)) << char(193)
         << string(15, char(196)) << char(217);

    gotoxy(35, 7 + ITEMS_PER_PAGE); clreol();
    gotoxy(35, 7 + ITEMS_PER_PAGE); cout << "Trang " << page + 1 << " / " << totalPages;
}

void XuLyInDanhSachDocGia(TREE_DOCGIA &root, bool sortByName) {
    if (root == nullptr) {
        ThongBao("Danh sach doc gia trong!");
        return;
    }
    int totalNode = countNodeDocGia(root);
    TheDocGia** Array = new TheDocGia*[totalNode];
    int index = 0;
    InsertTreeToArray(root, Array, index);
    if (sortByName) { 
        QuickSortDocGia(Array, 0, totalNode -1); 
    }

    clrscr();
    SetColor(14);
    CreateBoxDouble(40, 1, "   DANH SACH DOC GIA   ", 25);
    cout << setfill(' ');
    SetColor(7);

    // Vẽ phần đầu bảng (Header)
    gotoxy(4, 3);
    cout << char(218) << string(6, char(196)) << char(194) << string(10, char(196)) << char(194)
         << string(30, char(196)) << char(194) << string(12, char(196)) << char(194)
         << string(15, char(196)) << char(191);
    
    gotoxy(4, 4);
    cout << char(179) << " " << left << setw(5) << "STT" << char(179) << " " << left << setw(9) << "Ma The"
         << char(179) << " " << left << setw(29) << "Ho va Ten" << char(179) << " " << left << setw(11) << "Gioi Tinh"
         << char(179) << " " << left << setw(14) << "Trang Thai" << char(179);
    
    gotoxy(4, 5); // Dòng kẻ ngay dưới header
    cout << char(195) << string(6, char(196)) << char(197) << string(10, char(196)) << char(197)
         << string(30, char(196)) << char(197) << string(12, char(196)) << char(197)
         << string(15, char(196)) << char(180);

    
    const int ITEMS_PER_PAGE = 15;
    int currentPage = 0;
    int totalPages = (totalNode - 1) / ITEMS_PER_PAGE + 1;
    HienThiDanhSachDocGia(Array, currentPage, totalPages, totalNode);

    SetColor(8);
    gotoxy(20, 8 + ITEMS_PER_PAGE); cout << "Nhan phim mui ten [<-] [->] de chuyen trang, [ESC] de thoat.";
    SetColor(7);
    
    while(true){
        ShowCur(false);
        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 75 && currentPage > 0) { // Trái
                currentPage--;
                HienThiDanhSachDocGia(Array, currentPage, totalPages, totalNode);
            } else if (key == 77 && currentPage < totalPages - 1) { // Phải
                currentPage++;
                HienThiDanhSachDocGia(Array, currentPage, totalPages, totalNode);
            }
        } else if (key == 27) { // ESC
            
            break;
        }
    }
    cout<< right;
    delete[] Array;
}

TREE_DOCGIA Search(TREE_DOCGIA root, int maThe){
    TREE_DOCGIA p;
    p = root;
    while(p != nullptr && p->data.maThe != maThe){
        if(maThe < p->data.maThe) p = p->left;
        else p = p->right;
    }
    return p;
}

TREE_DOCGIA findMin(TREE_DOCGIA root){
    while(root->left != nullptr) root = root->left;
    return root;
}

bool check_MuonSach(PTRMT ds){
    while(ds!= nullptr){
        if(ds->data.trangThai == 0 || ds->data.trangThai == 2) return true;
        ds = ds->next;
    }
    return false;
}

TREE_DOCGIA Delete_DocGia(TREE_DOCGIA root, int maThe){
    if(root == nullptr) return nullptr;
    if(maThe < root->data.maThe) root->left = Delete_DocGia(root->left, maThe);
    else if(maThe > root->data.maThe) root->right = Delete_DocGia(root->right, maThe);
    else{
        if(check_MuonSach(root->data.dsMuonTra)){
            ThongBao("Doc gia dang muon sach, khong the xoa!");
            return root;
        }
        if(root->left == nullptr){
            TREE_DOCGIA tmp = root->right;
            delete root;
            return tmp;
        } else if(root->right == nullptr){
            TREE_DOCGIA tmp = root->left;
            delete root;
            return tmp;
        } else {
            TREE_DOCGIA minNode = findMin(root->right);
            PTRMT dsMT = root->data.dsMuonTra;
            root->data = minNode->data;
            minNode->data.dsMuonTra = dsMT;
            root->right = Delete_DocGia(root->right, minNode->data.maThe);
        }
    }
    return root;
}

void XoaDocGia(TREE_DOCGIA &root) {
    // dung de phan trang
    TheDocGia* tempDocGiaArray = nullptr;
    TheDocGia** displayArray = nullptr;

    clrscr();
    SetColor(14);
    CreateBoxDouble(25, 1, "   XOA DOC GIA   ", 25);
    cout << setfill(' ');
    SetColor(7);

    // Vẽ header của bảng (chỉ vẽ một lần)
    gotoxy(4, 3);
    cout << char(218) << string(6, char(196)) << char(194) << string(10, char(196)) << char(194)
         << string(30, char(196)) << char(194) << string(12, char(196)) << char(194)
         << string(15, char(196)) << char(191);

    gotoxy(4, 4);
    cout << char(179) << " " << left << setw(5) << "STT" << char(179) << " " << left << setw(9) << "Ma The"
         << char(179) << " " << left << setw(29) << "Ho va Ten" << char(179) << " " << left << setw(11) << "Gioi Tinh"
         << char(179) << " " << left << setw(14) << "Trang Thai" << char(179);

    gotoxy(4, 5);
    cout << char(195) << string(6, char(196)) << char(197) << string(10, char(196)) << char(197)
         << string(30, char(196)) << char(197) << string(12, char(196)) << char(197)
         << string(15, char(196)) << char(180);

    const int ITEMS_PER_PAGE = 15;
    int currentPage = 0;
    int totalNode = 0;
    int totalPages = 1;

    // Vị trí con trỏ nhập liệu
    int inputX = 90 + string("Nhap ma the can xoa: ").length();
    int inputY = 6;
    string currentMaTheInput = ""; // Chuỗi lưu trữ mã thẻ đang được nhập
    while (true) {
        totalNode = countNodeDocGia(root);
        if (tempDocGiaArray != nullptr) {
            delete[] tempDocGiaArray;
            tempDocGiaArray = nullptr;
        }
        if (displayArray != nullptr) {
            delete[] displayArray;
            displayArray = nullptr;
        }

        if (totalNode == 0) {
            for (int i = 6; i <= 8 + ITEMS_PER_PAGE + 5; ++i) {
                gotoxy(1, i);
                clreol();
            }
            SetColor(7);
            ThongBao("Danh sach doc gia rong. Nhan ESC de thoat.");
            while (_getch() != 27) {}
            break;
        }

        tempDocGiaArray = new TheDocGia[totalNode];
        int tempIndex = 0;
        collectDocGia(root, tempDocGiaArray, tempIndex);

        displayArray = new TheDocGia*[totalNode];
        for (int i = 0; i < totalNode; ++i) {
            displayArray[i] = &tempDocGiaArray[i];
        }

        totalPages = (totalNode > 0) ? (totalNode - 1) / ITEMS_PER_PAGE + 1 : 1;
        if (currentPage >= totalPages) {
            currentPage = (totalPages > 0) ? totalPages - 1 : 0;
        }
        if (currentPage < 0) currentPage = 0;

        HienThiDanhSachDocGia(displayArray, currentPage, totalPages, totalNode);

        SetColor(8);
        gotoxy(20, 8 + ITEMS_PER_PAGE); cout << "Nhan phim mui ten [<-] [->] de chuyen trang, [ESC] de thoat.";
        SetColor(7);
        gotoxy(90, 6); cout << "Nhap ma the can xoa: " << currentMaTheInput;
        ShowCur(true);
        gotoxy(inputX + currentMaTheInput.length(), inputY);

        int key;
        bool actionTaken = false;

        while (!actionTaken) {
            key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 75 && currentPage > 0) { // left
                    currentPage--;
                    HienThiDanhSachDocGia(displayArray, currentPage, totalPages, totalNode);
                    SetColor(8);
                    gotoxy(20, 8 + ITEMS_PER_PAGE); cout << "Nhan phim mui ten [<-] [->] de chuyen trang, [ESC] de thoat.";
                    SetColor(7);
                    gotoxy(90, 6); cout << "Nhap ma the can xoa: " << currentMaTheInput;
                    ShowCur(true);
                    gotoxy(inputX + currentMaTheInput.length(), inputY);
                } else if (key == 77 && currentPage < totalPages -1) { // right
                    currentPage++;
                    HienThiDanhSachDocGia(displayArray, currentPage, totalPages, totalNode);
                    SetColor(8);
                    gotoxy(20, 8 + ITEMS_PER_PAGE); cout << "Nhan phim mui ten [<-] [->] de chuyen trang, [ESC] de thoat.";
                    SetColor(7);
                    gotoxy(90, 6); cout << "Nhap ma the can xoa: " << currentMaTheInput;
                    ShowCur(true);
                    gotoxy(inputX + currentMaTheInput.length(), inputY);
                }
            } else if (key == 27) { // Phím ESC
                ThongBao("DA HUY THAO TAC XOA DOC GIA");
                ShowCur(false);
                if (tempDocGiaArray != nullptr) delete[] tempDocGiaArray;
                if (displayArray != nullptr) delete[] displayArray;
                cout<<right;
                return;
            } else if (key == '\r' || key == '\n') { // Phím Enter
                actionTaken = true;
            } else if (key == '\b') { // Phím Backspace
                if (!currentMaTheInput.empty()) {
                    currentMaTheInput.pop_back();
                    gotoxy(inputX + currentMaTheInput.length(), inputY);
                    cout << " ";
                    gotoxy(inputX + currentMaTheInput.length(), inputY);
                }
            } else if (isdigit(key) && currentMaTheInput.length() < 10) {
                currentMaTheInput += (char)key;
                cout << (char)key;
            }
        }

        int maTheToDelete = -1;
        if (currentMaTheInput.empty()) {
            ThongBao("Vui long nhap ma the can xoa.");
            continue;
        }

        try {
            maTheToDelete = stoi(currentMaTheInput);
        } catch (const exception& e) {
            ThongBao("Ma the khong hop le. Vui long nhap so.");
            currentMaTheInput = "";
            continue;
        }

        TREE_DOCGIA p = Search(root, maTheToDelete);
        if (p == nullptr) {
            SetColor(12);
            ThongBao("Khong tim thay doc gia co ma the nay!");
            SetColor(7);
            currentMaTheInput = "";
            for(int i = 6; i <= 17 + ITEMS_PER_PAGE + 1; ++i) {
                gotoxy(90, i);
                clreol();
            }
            continue;
        }

        if(check_MuonSach(p->data.dsMuonTra)){
            SetColor(12);
            ThongBao("Doc gia dang muon sach, khong the xoa!");
            SetColor(7);
            currentMaTheInput = "";
            continue;
        }

        for(int i = 6; i <= 17 + ITEMS_PER_PAGE + 1; ++i) {
            gotoxy(90, i);
            clreol();
        }
        gotoxy(90, 9);  cout << "Thong tin doc gia:";
        gotoxy(90, 10); cout << "Ma the     : " << p->data.maThe;
        gotoxy(90, 11); cout << "Ho va ten  : " << p->data.ho << " " << p->data.ten;
        gotoxy(90, 12); cout << "Gioi tinh  : " << p->data.gioitinh;
        gotoxy(90, 13); cout << "Trang thai : " << (p->data.trangThai == 1 ? "Hoat dong" : "Bi khoa");
        SetColor(10);
        gotoxy(90, 15); cout << "Ban co chac chan muon xoa doc gia nay? (y/n): ";
        SetColor(7);

        ShowCur(true);
        char confirmChar = GetYesNoInput(90 + string("Ban co chac chan muon xoa doc gia nay? (y/n): ").length(), 15);

        if (confirmChar == 'y') {
            root = Delete_DocGia(root, maTheToDelete);

            save_File(root, "txt\\DanhSachDocGia.txt");

            SetColor(10);
            ThongBao("XOA DOC GIA THANH CONG!");
            SetColor(7);
            currentMaTheInput = "";
        } else {
            SetColor(10);
            ThongBao("DA HUY THAO TAC XOA DOC GIA");
            SetColor(7);
            currentMaTheInput = "";
        }

        gotoxy(90, 6); clreol();
    }

    ShowCur(false);
    if (tempDocGiaArray != nullptr) delete[] tempDocGiaArray;
    if (displayArray != nullptr) delete[] displayArray;
    cout << right;
}

void SuaDocGia(TREE_DOCGIA &root){
    TheDocGia* tempDocGiaArray = nullptr;
    TheDocGia** displayArray = nullptr;

    const int ITEMS_PER_PAGE = 15;
    int currentPage = 0;
    int totalNode = 0;
    int totalPages = 1;

    // Vị trí con trỏ nhập liệu
    int inputX = 90 + string("Nhap ma the can sua: ").length();
    int inputY = 6;
    string currentMaTheInput = ""; // Chuỗi lưu trữ mã thẻ đang được nhập
    while (true) {
        clrscr();
        SetColor(14);
        CreateBoxDouble(35, 1, "   SUA DOC GIA   ", 17);
        cout << setfill(' ');
        SetColor(7);

        gotoxy(4, 3);
        cout << char(218) << string(6, char(196)) << char(194) << string(10, char(196)) << char(194)
            << string(30, char(196)) << char(194) << string(12, char(196)) << char(194)
            << string(15, char(196)) << char(191);

        gotoxy(4, 4);
        cout << char(179) << " " << left << setw(5) << "STT" << char(179) << " " << left << setw(9) << "Ma The"
            << char(179) << " " << left << setw(29) << "Ho va Ten" << char(179) << " " << left << setw(11) << "Gioi Tinh"
            << char(179) << " " << left << setw(14) << "Trang Thai" << char(179);

        gotoxy(4, 5);
        cout << char(195) << string(6, char(196)) << char(197) << string(10, char(196)) << char(197)
            << string(30, char(196)) << char(197) << string(12, char(196)) << char(197)
            << string(15, char(196)) << char(180);

        totalNode = countNodeDocGia(root);
        if (tempDocGiaArray != nullptr) {
            delete[] tempDocGiaArray;
            tempDocGiaArray = nullptr;
        }
        if (displayArray != nullptr) {
            delete[] displayArray;
            displayArray = nullptr;
        }

        if (totalNode == 0) {
            ThongBao("Danh sach doc gia rong. Nhan ESC de thoat.");
            while (_getch() != 27) {}
            break;
        }

        tempDocGiaArray = new TheDocGia[totalNode];
        int tempIndex = 0;
        collectDocGia(root, tempDocGiaArray, tempIndex);

        displayArray = new TheDocGia*[totalNode];
        for (int i = 0; i < totalNode; ++i) displayArray[i] = &tempDocGiaArray[i];

        totalPages = (totalNode > 0) ? (totalNode - 1) / ITEMS_PER_PAGE + 1 : 1;
        if (currentPage >= totalPages) {
            currentPage = (totalPages > 0) ? totalPages - 1 : 0;
        }
        if (currentPage < 0) currentPage = 0;

        HienThiDanhSachDocGia(displayArray, currentPage, totalPages, totalNode);

        SetColor(8);
        gotoxy(15, 8 + ITEMS_PER_PAGE); cout << "Nhan phim mui ten [<-] [->] de chuyen trang, [ESC] de thoat.";
        SetColor(7);
        gotoxy(90, 6); cout << "Nhap ma the can sua: " << currentMaTheInput;
        ShowCur(true);
        gotoxy(inputX + currentMaTheInput.length(), inputY);

        int key;
        bool actionTaken = false;

        while (!actionTaken) {
            key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 75 && currentPage > 0) { // left
                    currentPage--;
                    HienThiDanhSachDocGia(displayArray, currentPage, totalPages, totalNode);
                    SetColor(8);
                    gotoxy(15, 8 + ITEMS_PER_PAGE); cout << "Nhan phim mui ten [<-] [->] de chuyen trang, [ESC] de thoat.";
                    SetColor(7);
                    gotoxy(90, 6); cout << "Nhap ma the can sua: " << currentMaTheInput;
                    ShowCur(true);
                    gotoxy(inputX + currentMaTheInput.length(), inputY);
                } else if (key == 77 && currentPage < totalPages -1) { // right
                    currentPage++;
                    HienThiDanhSachDocGia(displayArray, currentPage, totalPages, totalNode);
                    SetColor(8);
                    gotoxy(15, 8 + ITEMS_PER_PAGE); cout << "Nhan phim mui ten [<-] [->] de chuyen trang, [ESC] de thoat.";
                    SetColor(7);
                    gotoxy(90, 6); cout << "Nhap ma the can sua: " << currentMaTheInput;
                    ShowCur(true);
                    gotoxy(inputX + currentMaTheInput.length(), inputY);
                }
            } else if (key == 27) { // Phím ESC
                ThongBao("DA HUY THAO TAC SUA DOC GIA");
                ShowCur(false);
                if (tempDocGiaArray != nullptr) delete[] tempDocGiaArray;
                if (displayArray != nullptr) delete[] displayArray;
                cout<<right;
                return;
            } else if (key == '\r' || key == '\n') { // Phím Enter
                actionTaken = true;
            } else if (key == '\b') { // Phím Backspace
                if (!currentMaTheInput.empty()) {
                    currentMaTheInput.pop_back();
                    gotoxy(inputX + currentMaTheInput.length(), inputY);
                    cout << " ";
                    gotoxy(inputX + currentMaTheInput.length(), inputY);
                }
            } else if (isdigit(key) && currentMaTheInput.length() < 10) {
                currentMaTheInput += (char)key;
                cout << (char)key;
            }
        }

        int maTheToEdit = -1;
        if (currentMaTheInput.empty()) {
            ThongBao("Vui long nhap ma the can sua.");
            continue;
        }

        try {
            maTheToEdit = stoi(currentMaTheInput);
        } catch (const exception& e) {
            ThongBao("Ma the khong hop le. Vui long nhap so.");
            currentMaTheInput = "";
            continue;
        }

        TREE_DOCGIA p = Search(root, maTheToEdit);
        if(p == nullptr){
            SetColor(12);
            ThongBao("Khong tim thay doc gia co ma the nay!");
            SetColor(7);
            currentMaTheInput = "";
            continue;

        }
        ShowCur(false);
        string tmpHo = p->data.ho;
        string tmpTen = p->data.ten;
        string tmpGioiTinh = p->data.gioitinh;
        int tmpTrangThai = p->data.trangThai;
        enum Field {HO, TEN, GIOITINH, TRANGTHAI};
        int currentField = HO;
        int formX = 90, formY = 10;

        bool isEditing = true;
        while(isEditing){
            
            HienThiFormSua(formX, formY, tmpHo, tmpTen, tmpGioiTinh, tmpTrangThai, currentField);
            key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
                switch (key) {
                    case 72: // Up
                        currentField = (currentField > HO) ? currentField - 1 : TRANGTHAI;
                        break;
                    case 80: // Down
                        currentField = (currentField < TRANGTHAI) ? currentField + 1 : HO;
                        break;
                    case 60: // F2 Key
                    {
                        ShowCur(true);
                        gotoxy(formX, formY + 15);
                        SetColor(14); // Yellow
                        cout << "Ban co chac chan muon luu (Y/N): ";
                        SetColor(7);
                        char confirmChar = GetYesNoInput(formX + string("Ban co chac chan muon luu (Y/N): ").length(), formY + 15);
                        
                        if (confirmChar == 'y') {
                            p->data.ho = chuanHoaTen(tmpHo);
                            p->data.ten = chuanHoaTen(tmpTen);
                            p->data.gioitinh = chuanHoaTen(tmpGioiTinh);
                            p->data.trangThai = tmpTrangThai;
                            save_File(root, "txt\\DanhSachDocGia.txt");
                            ThongBao("CAP NHAT DOC GIA THANH CONG!");
                        } else {
                            ThongBao("DA HUY THAO TAC LUU THAY DOI");
                        }
                        isEditing = false;
                    }
                    break;
                }
            } else if (key == 27) { // ESC
                ThongBao("DA HUY THAO TAC SUA DOC GIA");
                isEditing = false;
            } else if (key == 13) { // Enter
                ShowCur(true);
                string resultStr;
                switch (currentField) {
                    case HO: {
                        string prompt = "Ho         : ";
                        int editY = formY + 1;
                        gotoxy(formX + 2, editY); cout << prompt; clreol();
                        resultStr = inputName(formX + 2 + prompt.length(), editY, 20, true);
                        if (resultStr != INPUT_CANCELLED && !resultStr.empty()) tmpHo = resultStr;
                        break;
                    }
                    case TEN: {
                        string prompt = "Ten        : ";
                        int editY = formY + 3;
                        gotoxy(formX + 2, editY); cout << prompt; clreol();
                        resultStr = inputName(formX + 2 + prompt.length(), editY, 10, false);
                        if (resultStr != INPUT_CANCELLED && !resultStr.empty()) tmpTen = resultStr;
                        break;
                    }
                    case GIOITINH: {
                        string prompt = "Gioi tinh  : ";
                        int editY = formY + 5;
                        while(true){
                            gotoxy(formX + 2, editY); cout << prompt; clreol();
                            resultStr = inputName(formX + 2 + prompt.length(), editY, 3, false);
                            if (resultStr == INPUT_CANCELLED || resultStr.empty()) break;
                            if (isValidGender(resultStr)) {
                                tmpGioiTinh = chuanHoaTen(resultStr);
                                break;
                            } else {
                                ThongBao("Gioi tinh khong hop le! (Nhap 'Nam' hoac 'Nu')");
                            }
                        }
                        break;
                    }
                    case TRANGTHAI:
                        tmpTrangThai = 1 - tmpTrangThai;
                        break;
                }
                ShowCur(false);
            }
        }
        
        for (int i = formY - 2; i < formY + 17; ++i) {
            gotoxy(formX, i); clreol();
        }
        currentMaTheInput = "";
        gotoxy(90, 6); clreol();
    }
    ShowCur(false);
    if (tempDocGiaArray != nullptr) delete[] tempDocGiaArray;
    if (displayArray != nullptr) delete[] displayArray;
    cout << right;
}

void HienThiFormSua(int x, int y, const string &ho, const string &ten, const string &gioitinh, int trangthai, int currentField){
    enum Field {HO, TEN, GIOITINH, TRANGTHAI};
    const int FORM_WIDTH = 45;

    for(int i = y - 2; i < y + 16; ++i) {
        gotoxy(x, i);
        clreol();
    }

    CreateBoxDouble(x + 10, y - 2, "   CHINH SUA THONG TIN   ", 25);
    gotoxy(x+2, y+1); SetColor(currentField == HO ? 11 : 7);
    cout << "Ho         : " << ho;

    gotoxy(x + 2, y + 3); SetColor(currentField == TEN ? 11 : 7);
    cout << "Ten        : " << ten;

    gotoxy(x + 2, y + 5);
    SetColor(currentField == GIOITINH ? 11 : 7);
    cout << "Gioi tinh  : " << gioitinh;

    gotoxy(x + 2, y + 7);
    SetColor(currentField == TRANGTHAI ? 11 : 7);
    cout << "Trang thai : " << (trangthai == 1 ? "Hoat dong" : "Bi khoa");

    SetColor(8);
    gotoxy(x + 1, y + 9);
    cout << string(FORM_WIDTH - 2, char(196));

    gotoxy(x + 3, y + 10); cout << "Mui ten   : Di chuyen len/xuong";
    gotoxy(x + 3, y + 11); cout << "Enter     : Sua truong duoc chon";
    gotoxy(x + 3, y + 12); cout << "F2        : Luu tat ca thay doi";
    gotoxy(x + 3, y + 13); cout << "ESC       : Huy bo";
    SetColor(7);
}

string Search_NameBook(DS_DauSach &ds, int maSach){
    for(int i =0; i< ds.soluong; i++){
        PTRDMS p = ds.nodes[i].dms;
        while(p != nullptr){
            if(p->data.maSach == maSach) return ds.nodes[i].tenSach;
            p=p->next;
        }
    }
    return "Khong tim thay sach";
}

int countNodeMuon(PTRMT First){
    int count = 0;
    PTRMT p = First;
    while(p!=nullptr){
        if(p->data.trangThai == 0 || p->data.trangThai == 2) count++;
        p = p->next;
    }
    return count;
}

void collectMuon(PTRMT First, MuonTra dsMuon[], int &index){
    PTRMT p =First;
    while(p!=nullptr){
        if(p->data.trangThai == 0 || p->data.trangThai == 2) dsMuon[index++] = p->data;
        p = p->next;
    }
}

void draw_GiaoDienXemDanhSachMuon(){
    clrscr();
    SetColor(14);
    CreateBoxDouble(50, 1, "   XEM DANH SACH MUON TRA   ", 30);
    cout<< setfill(' ');
    SetColor(7);

    gotoxy(4, 3);
    cout << char(218) << string(6, char(196)) << char(194) << string(10, char(196)) << char(194)
         << string(30, char(196)) << char(194) << string(12, char(196)) << char(194)
         << string(15, char(196)) << char(191);
    
    gotoxy(4, 4);
    cout << char(179) << " " << left << setw(5) << "STT" << char(179) << " " << left << setw(9) << "Ma The"
         << char(179) << " " << left << setw(29) << "Ho va Ten" << char(179) << " " << left << setw(11) << "Gioi Tinh"
         << char(179) << " " << left << setw(14) << "Trang Thai" << char(179);
    
    gotoxy(4, 5);
    cout << char(195) << string(6, char(196)) << char(197) << string(10, char(196)) << char(197)
         << string(30, char(196)) << char(197) << string(12, char(196)) << char(197)
         << string(15, char(196)) << char(180);

    gotoxy(84, 3); cout << "TIM KIEM VA CHI TIET MUON TRA";
    SetColor(8);
    gotoxy(2, 25); cout << "[<-] [->]: Chuyen trang";
    gotoxy(45, 25); cout << "[ESC]: Thoat";
    SetColor(7);
}

void XoaKhuVucMuonTra(int startX, int startY){
    for (int i = 0; i < 20; ++i) {
        gotoxy(startX, startY + i);
        cout << string(70, ' ');
    }
}

void HienThiDanhSachMuon(MuonTra** Array, DS_DauSach &ds, int page, int totalPages, int totalNode, int startX, int startY) {
    SetColor(7);
    const int ITEMS_PER_PAGE_MUON = 5; 
    
    for(int i = 0; i < ITEMS_PER_PAGE_MUON + 5; ++i) {
        gotoxy(startX, startY + i);
        cout << string(75, ' ');
    }

    gotoxy(startX, startY);
    cout << char(218) << string(5, char(196)) << char(194)
         << string(8, char(196)) << char(194)
         << string(25, char(196)) << char(194)
         << string(12, char(196)) << char(194)
         << string(12, char(196)) << char(191);

    gotoxy(startX, startY + 1);
    cout << char(179) << " " << left << setw(4) << "STT" << char(179)
         << " " << left << setw(7) << "MaSach" << char(179)
         << " " << left << setw(24) << "Ten Sach" << char(179)
         << " " << left << setw(11) << "Ngay Muon" << char(179)
         << " " << left << setw(11) << "Trang Thai" << char(179);

    gotoxy(startX, startY + 2);
    cout << char(195) << string(5, char(196)) << char(197)
         << string(8, char(196)) << char(197)
         << string(25, char(196)) << char(197)
         << string(12, char(196)) << char(197)
         << string(12, char(196)) << char(180);

    int startIndex = page * ITEMS_PER_PAGE_MUON;
    int endIndex = min(startIndex + ITEMS_PER_PAGE_MUON, totalNode);

    for (int i = startIndex; i < endIndex; ++i) {
        MuonTra* current = Array[i];
        int row = startY + 3 + (i - startIndex);
        gotoxy(startX, row);

        cout << char(179) << " " << left << setw(4) << i + 1 << char(179)
             << " " << left << setw(7) << current->maSach << char(179);

        string tenSach = Search_NameBook(ds, current->maSach);
        if (tenSach.length() > 24) tenSach = tenSach.substr(0, 24);
        cout << " " << left << setw(24) << tenSach << char(179);

        string ngayMuon = to_string(current->ngayMuon.ngay) + "/" +
                          to_string(current->ngayMuon.thang) + "/" +
                          to_string(current->ngayMuon.nam);
        cout << " " << left << setw(11) << ngayMuon << char(179);

        string trangThai = (current->trangThai == 0) ? "Dang muon" : "Mat";
        cout << " " << left << setw(11) << trangThai << char(179);
    }

    for (int i = endIndex - startIndex; i < ITEMS_PER_PAGE_MUON; ++i) {
        int row = startY + 3 + i;
        gotoxy(startX, row);
        cout << char(179) << string(5, ' ') << char(179)
             << string(8, ' ') << char(179)
             << string(25, ' ') << char(179)
             << string(12, ' ') << char(179)
             << string(12, ' ') << char(179);
    }

    int fixedFooterY = startY + 3 + ITEMS_PER_PAGE_MUON;
    gotoxy(startX, fixedFooterY);
    cout << char(192) << string(5, char(196)) << char(193)
         << string(8, char(196)) << char(193)
         << string(25, char(196)) << char(193)
         << string(12, char(196)) << char(193)
         << string(12, char(196)) << char(217);

    int pageInfoX = startX + (62 / 2) - (string("Trang X / Y").length() / 2);
    int pageInfoY = fixedFooterY + 1;

    gotoxy(pageInfoX, pageInfoY);
    clreol(); 
    cout << "Trang " << page + 1 << " / " << totalPages;
}

void print_DsDangMuon(TREE_DOCGIA root, DS_DauSach &ds) {
    TheDocGia* tempDocGiaArray = nullptr;
    TheDocGia** displayDGArray = nullptr;

    MuonTra* tmpMuonArray = nullptr;
    MuonTra** displayMuonArray = nullptr;

    TREE_DOCGIA p;

    draw_GiaoDienXemDanhSachMuon();

    const int ITEMS_PER_PAGE_DG = 15;
    int currentPage_DG = 0;
    int totalNode_DG = 0;
    int totalPages_DG = 1;

    const int ITEMS_PER_PAGE_MUON = 5;
    int currentMuonPage = 0;
    int totalMuonNode = 0;
    int totalMuonPages = 1;

    int inputX = 84 + string("Nhap ma the doc gia: ").length();
    int inputY = 5;
    string currentMaTheInput = "";

    while(true){
        totalNode_DG = countNodeDocGia(root);
        if(tempDocGiaArray != nullptr){
            delete[] tempDocGiaArray;
            tempDocGiaArray = nullptr;
        }
        if(displayDGArray != nullptr){
            delete[] displayDGArray;
            displayDGArray = nullptr;
        }
        if(totalNode_DG == 0){
            ThongBao("Danh sach doc gia rong!");
            ShowCur(false);
            cout<<right;
            return;
        }

        tempDocGiaArray = new TheDocGia[totalNode_DG];
        int tmpIndex = 0;
        collectDocGia(root, tempDocGiaArray, tmpIndex);

        displayDGArray = new TheDocGia*[totalNode_DG];
        for(int i = 0; i < totalNode_DG; ++i) {
            displayDGArray[i] = &tempDocGiaArray[i];
        }

        totalPages_DG = (totalNode_DG > 0) ? (totalNode_DG - 1) / ITEMS_PER_PAGE_DG + 1 : 1;
        if (currentPage_DG >= totalPages_DG) {
            currentPage_DG = (totalPages_DG > 0) ? totalPages_DG - 1 : 0;
        }
        if (currentPage_DG < 0) currentPage_DG = 0;

        HienThiDanhSachDocGia(displayDGArray, currentPage_DG, totalPages_DG, totalNode_DG);

        gotoxy(84, 5); cout<<"Nhap ma the doc gia: " << currentMaTheInput;
        ShowCur(true);
        gotoxy(inputX + currentMaTheInput.length(), inputY);

        int key;
        bool actionTaken = false;

        while(!actionTaken){
            key = _getch();
            if(key == 0 || key == 224){
                key = _getch();
                if(key == 75 && currentPage_DG > 0){
                    currentPage_DG--;
                    HienThiDanhSachDocGia(displayDGArray, currentPage_DG, totalPages_DG, totalNode_DG);
                    gotoxy(84, 5); cout << "Nhap ma the doc gia: " << currentMaTheInput;
                    ShowCur(true);
                    gotoxy(inputX + currentMaTheInput.length(), inputY);
                } else if (key == 77 && currentPage_DG < totalPages_DG - 1){
                    currentPage_DG++;
                    HienThiDanhSachDocGia(displayDGArray, currentPage_DG, totalPages_DG, totalNode_DG);
                    gotoxy(84, 5); cout << "Nhap ma the doc gia: " << currentMaTheInput;
                    ShowCur(true);
                    gotoxy(inputX + currentMaTheInput.length(), inputY);
                }
            } else if(key == 27){
                ThongBao("DA HUY THAO TAC XEM SACH MUON");
                ShowCur(false);
                if(tempDocGiaArray != nullptr) delete[] tempDocGiaArray;
                if(displayDGArray != nullptr) delete[] displayDGArray;
                cout<<right;
                return;
            } else if (key == '\r' || key == '\n') { // Phím Enter
                actionTaken = true;
            } else if (key == '\b') { // Phím Backspace
                if (!currentMaTheInput.empty()) {
                    currentMaTheInput.pop_back();
                    gotoxy(inputX + currentMaTheInput.length(), inputY);
                    cout << " ";
                    gotoxy(inputX + currentMaTheInput.length(), inputY);
                }
            } else if (isdigit(key) && currentMaTheInput.length() < 10) {
                currentMaTheInput += (char)key;
                cout << (char)key;
            }
        }

        int maTheToView = -1;
    
        if(currentMaTheInput.empty()){
            ThongBao("Vui long nhap ma the doc gia.");
            continue;
        }
    
        try{
            maTheToView = stoi(currentMaTheInput);
        } catch(const exception& e){
            ThongBao("Ma the khong hop le. Vui long nhap so.");
            currentMaTheInput = "";
            continue;
        }

        p = Search(root, maTheToView);
        if(p == nullptr){
            SetColor(12);
            ThongBao("Khong tim thay doc gia co ma the nay!");
            SetColor(7);
            currentMaTheInput = "";
            for(int i = 5; i < 17 + ITEMS_PER_PAGE_DG + 1; ++i){
                gotoxy(84, i); clreol();
            }
            continue;
        }

        ShowCur(false);
        gotoxy(84, 7); cout << "Ma The: " << p->data.maThe;
        gotoxy(84, 8); cout << "Ho Ten: " << p->data.ho << " " << p->data.ten;
        gotoxy(84, 9); cout << "Trang Thai The: " << (p->data.trangThai == 0 ? "Bi Khoa" : "Hoat dong");
        
        totalMuonNode = countNodeMuon(p->data.dsMuonTra);

        if (tmpMuonArray != nullptr) {
            delete[] tmpMuonArray;
            tmpMuonArray = nullptr;
        }
        if (displayMuonArray != nullptr) {
            delete[] displayMuonArray;
            displayMuonArray = nullptr;
        }
        
        if(totalMuonNode == 0){
            ThongBao("Doc gia nay khong co sach nao dang muon.");
            XoaKhuVucMuonTra(84, 7);
            currentMaTheInput = "";
            gotoxy(84, 5); clreol();
            continue;
        }

        tmpMuonArray = new MuonTra[totalMuonNode];
        int tmpMuonIndex = 0;
        collectMuon(p->data.dsMuonTra, tmpMuonArray, tmpMuonIndex);

        displayMuonArray = new MuonTra*[totalMuonNode];
        for(int i =0; i<totalMuonNode; ++i){
            displayMuonArray[i] = &tmpMuonArray[i];
        }

        totalMuonPages = (totalMuonNode > 0) ? (totalMuonNode - 1) / ITEMS_PER_PAGE_MUON + 1 : 1;
        if (currentMuonPage >= totalMuonPages) {
            currentMuonPage = (totalMuonPages > 0) ? totalMuonPages - 1 : 0;
        }
        if(currentMuonPage < 0) currentMuonPage = 0;

        bool isViewing = true;
        while(isViewing){
            HienThiDanhSachMuon(displayMuonArray, ds, currentMuonPage, totalMuonPages, totalMuonNode, 84, 11);
            key = _getch();
            if(key == 0 || key == 224){
                key = _getch();
                if(key == 75 && currentMuonPage > 0){
                    currentMuonPage--;
                } else if (key == 77 && currentMuonPage < totalMuonPages - 1){
                    currentMuonPage++;
                }
            } else if(key == 27){
                ThongBao("QUAY VE TRANG DANH SACH DOC GIA");
                isViewing = false;
            }
        }
        if (tmpMuonArray != nullptr) {
            delete[] tmpMuonArray;
            tmpMuonArray = nullptr;
        }
        if (displayMuonArray != nullptr) {
            delete[] displayMuonArray;
            displayMuonArray = nullptr;
        }
        XoaKhuVucMuonTra(84, 7);
        currentMaTheInput = "";
        gotoxy(84, 5); clreol();
    }
    ShowCur(false);
    if (tempDocGiaArray != nullptr) delete[] tempDocGiaArray;
    if (displayDGArray != nullptr) delete[] displayDGArray;
    cout << right;
}