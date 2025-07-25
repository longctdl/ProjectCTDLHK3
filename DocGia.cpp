#include "DocGia.h"
#include "UIConsole.h"
#include "hamXuLy.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>

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
    int maThe = dsMaThe[0];
    for(int i = 0; i< n-1; i++){
        dsMaThe[i] = dsMaThe[i+1];
    }
    n--;
    // write_MaThe(dsMaThe, n);
    return maThe;
}

bool Enter_DocGia(TheDocGia &docgia) {
    ShowCur(true);
    clrscr();

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
        ho = inputName(INPUT_X, 7, 30, true);
        if(ho == INPUT_CANCELLED) return false;
        if (!ho.empty()) break;
        ThongBao("Ho khong duoc de trong. Vui long nhap lai!");
    }

    while (true) {
        ten = inputName(INPUT_X, 9, 10, false);
        if(ten == INPUT_CANCELLED) return false;
        if (!ten.empty()) break;
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

void collectDocGia(TREE_DOCGIA root, TheDocGia dsDocGia[], int& index){
    if(root == nullptr) return;
    collectDocGia(root->left, dsDocGia, index);
    dsDocGia[index++] = root->data;
    collectDocGia(root->right, dsDocGia, index);
}

TREE_DOCGIA buildTree(TheDocGia dsDocGia[], int left, int right){
    if(left > right) return nullptr;
    int mid = (left + right) / 2;
    TREE_DOCGIA root = new NodeTheDocGia{dsDocGia[mid], nullptr, nullptr};

    root->left = buildTree(dsDocGia, left, mid - 1);
    root->right = buildTree(dsDocGia, mid + 1, right);

    return root;
}

void freeBST(TREE_DOCGIA &root){
    if(root == nullptr) return;
    freeBST(root->left);
    freeBST(root->right); 
    delete root;
    root = nullptr;
}

void sortDocGiaByMaThe(TheDocGia dsDocGia[], int left, int right) {
    if(left >= right) return;
    int i = left;
    int j = right;

    int mid = dsDocGia[(left + right) / 2].maThe;
    while(i<=j){
        while(dsDocGia[i].maThe < mid) i++;
        while(dsDocGia[j].maThe > mid) j--;

        if(i<=j){
            swap(dsDocGia[i], dsDocGia[j]);
            i++;
            j--;
        }
    }
    if(left < j) sortDocGiaByMaThe(dsDocGia, left, j);
    if(i < right) sortDocGiaByMaThe(dsDocGia, i, right);
}

void ThemDocGia(TREE_DOCGIA &root){
    TheDocGia docgia;
    if(!Enter_DocGia(docgia)){
        ThongBao("DA HUY THAO TAC THEM DOC GIA");
        ShowCur(false);
        return;
    }

    int* dsMaThe = new int[MAX_MATHE];
    int n = read_MaThe(dsMaThe);
    int maThe = get_MaThe(dsMaThe, n);

    if(maThe == -1){
        ThongBao("DA HET MA THE, KHONG THE THEM DOC GIA MOI");
        delete[] dsMaThe;
        return;
    }

    docgia.maThe = maThe;

    int currentSize = countNodeDocGia(root);
    TheDocGia* dsDocGia = new TheDocGia[currentSize + 1];

    int index = 0;
    collectDocGia(root, dsDocGia, index);
    dsDocGia[index] = docgia;
    index++;

    sortDocGiaByMaThe(dsDocGia, 0, index -1);
    freeBST(root);
    root = buildTree(dsDocGia, 0, index -1);

    write_MaThe(dsMaThe, n);

    SetColor(10);
    ThongBao("THEM DOC GIA THANH CONG!");
    SetColor(7);

    delete[] dsMaThe;
    delete[] dsDocGia;
}

void write_DSDocGia(TREE_DOCGIA root, const char* filename){
    ofstream fout(filename);
    if(!fout.is_open()){
        ThongBao("Khong the mo file de ghi danh sach doc gia!");
        return;
    }

    int currentSize = countNodeDocGia(root);
    TheDocGia* dsDocGia = new TheDocGia[currentSize];

    int index = 0;
    collectDocGia(root, dsDocGia, index);

    for(int i = 0; i< index; i++){
        fout<< dsDocGia[i].maThe << "\n" << dsDocGia[i].ho << "\n"
            << dsDocGia[i].ten<< "\n" << dsDocGia[i].gioitinh << "\n"
            << dsDocGia[i].trangThai << "\n\n";
    }
    fout.close();
    delete[] dsDocGia;
}

void read_DSDocGia(TREE_DOCGIA &root, const char* filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        ThongBao("Khong the mo file danh sach doc gia!");
        return;
    }

    TheDocGia* dsDocGia = new TheDocGia[MAX_MATHE]; 

    int count = 0;
    string line;

    while (count < MAX_MATHE) { 
        TheDocGia docgia;

        if (!getline(fin, line)) break;
        if (line.empty()) continue;

        try {
            docgia.maThe = stoi(line);

            if (!getline(fin, docgia.ho)) break;
            if (!getline(fin, docgia.ten)) break;
            if (!getline(fin, docgia.gioitinh)) break;
            if (!getline(fin, line)) break;
            docgia.trangThai = stoi(line);
        } catch (const exception& e) {
            continue; 
        }
        
        docgia.dsMuonTra = nullptr;
        getline(fin, line);
        
        dsDocGia[count++] = docgia;
    }
    fin.close();

    sortDocGiaByMaThe(dsDocGia, 0, count - 1);
    freeBST(root);
    root = buildTree(dsDocGia, 0, count - 1);

    delete[] dsDocGia;
}

int countNodeDocGia(TREE_DOCGIA root){
    if(root == nullptr) return 0;
    return 1 + countNodeDocGia(root->left) + countNodeDocGia(root->right);
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

TREE_DOCGIA Delete_DocGia(TREE_DOCGIA root, int maThe){
    if(root == nullptr) return nullptr;
    if(maThe < root->data.maThe) root->left = Delete_DocGia(root->left, maThe);
    else if(maThe > root->data.maThe) root->right = Delete_DocGia(root->right, maThe);
    else{
        if(root->data.dsMuonTra != nullptr){
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
            root->data = minNode->data;
            root->right = Delete_DocGia(root->right, minNode->data.maThe);
        }
    }
    return root;
}

void XoaDocGia(TREE_DOCGIA &root) {
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

        // 5. Tìm kiếm độc giả và xác nhận xóa
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

        if(p->data.dsMuonTra != nullptr){
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
            int* dsMaThe = new int[MAX_MATHE];
            int n = read_MaThe(dsMaThe);

            if (n < MAX_MATHE) {
                dsMaThe[n++] = maTheToDelete;
                write_MaThe(dsMaThe, n);
            } else {
                ThongBao("Canh bao: Danh sach ma the hien dang day. Ma the da xoa khong the tai su dung.");
            }
            delete[] dsMaThe;

            write_DSDocGia(root);

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
                            write_DSDocGia(root);
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