#include <iostream>
#include <fstream>
#include <queue>
using namespace std;

const int MAX_SIZE = 10000;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

typedef TreeNode* PTR;

struct Node{
    PTR data;
    Node* next;
};

typedef Node* NodeQ;

struct Queue{
    NodeQ front, rear;

    Queue(){
        front = rear = nullptr;
    }

    bool empty() const{
        return front == nullptr;
    }

    void push(PTR value){
        NodeQ newNode = new Node{value, nullptr};
        if(rear) rear->next = newNode;
        else front = newNode;
        rear = newNode;
    }

    PTR getFront() const {
        return empty() ? nullptr : front->data;
    }

    void pop(){
        if(empty()) return;
        NodeQ tmp = front;
        front = front->next;
        if(!front) rear = nullptr;
        delete tmp;
    }
};

PTR buildBalancedTree(int arr[], int left, int right) {
    if (left > right) return nullptr;
    int mid = (left + right) / 2;
    PTR root = new TreeNode(arr[mid]);
    root->left = buildBalancedTree(arr, left, mid - 1);
    root->right = buildBalancedTree(arr, mid + 1, right);
    return root;
}

int levelOrder(PTR root, int output[]) {
    if (!root) return 0;
    int index = 0;
    Queue q;
    q.push(root);
    while (!q.empty()) {
        PTR node = q.getFront(); q.pop();
        output[index++] = node->val;
        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
    return index;
}

void writeToFile(const char* filename, int data[], int n) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cerr << "Khong mo duoc file!\n";
        return;
    }
    for (int i = 0; i < n; i++) {
        fout << data[i] << "\n";
    }
    fout.close();
}

int main() {
    int a[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) {
        a[i] = i + 1;
    }

    PTR root = buildBalancedTree(a, 0, MAX_SIZE - 1);

    int result[MAX_SIZE];
    int size = levelOrder(root, result);

    writeToFile("txt\\MaTheDocGia.txt", result, size);

    cout << "Da ghi xong danh sach ma the!\n";
    return 0;
}
