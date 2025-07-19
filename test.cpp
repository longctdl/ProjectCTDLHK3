#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <fstream>

using namespace std;


struct List {
    int n;
    int* nodes;
};

void Swap(int& a, int& b) {
    int t = a;
    a = b;
    b = t;
}

void TaoDayNN_2(List& ds, int N, int max) {
    ds.n = N;
    ds.nodes = new int[max];

    for (int i = 0; i < max; i++) {
        ds.nodes[i] = i + 1;
    }

    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        int r = i + rand() % (max - i);
        swap(ds.nodes[i], ds.nodes[r]);
    }
}

void GhiMaTheNgauNhienVaoFile(const char* filename, int soLuong) {
    List ds;
    TaoDayNN_2(ds, soLuong, 99999);

    ofstream fout(filename);
    for (int i = 0; i < soLuong; i++) {
        fout << ds.nodes[i] << endl;
    }
    fout.close();

    delete[] ds.nodes;
}

int main() {
    GhiMaTheNgauNhienVaoFile("txt\\MaTheDocGia.txt", 10000);
    cout << "Da ghi 10000 ma the doc gia ngau nhien!" << endl;
    return 0;
}
