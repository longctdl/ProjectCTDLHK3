#include "DocGia.h"
#include "UIConsole.h"
#include "hamXuLy.h"
#include <iostream>
#include <fstream>
#include <algorithm>

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