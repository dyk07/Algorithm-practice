#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Node {
    char val;
    int pri, size;
    bool rev;
    Node *l, *r;
    Node(char v) : val(v), pri(rng()), size(1), rev(false), l(nullptr), r(nullptr) {}
};

int getSize(Node* x) {
    return x ? x->size : 0;
}

void update(Node* x) {
    if (x) x->size = getSize(x->l) + 1 + getSize(x->r);
}

void pushdown(Node* x) {
    if (x && x->rev) {
        x->rev = false;
        swap(x->l, x->r);
        if (x->l) x->l->rev ^= 1;
        if (x->r) x->r->rev ^= 1;
    }
}

// 将 root 分成 left 和 right，left 包含前 k 个节点
void split(Node* root, int k, Node*& left, Node*& right) {
    if (!root) {
        left = right = nullptr;
        return;
    }
    pushdown(root);
    if (getSize(root->l) < k) {
        split(root->r, k - getSize(root->l) - 1, root->r, right);
        left = root;
    } else {
        split(root->l, k, left, root->l);
        right = root;
    }
    update(root);
}

Node* merge(Node* left, Node* right) {
    if (!left) return right;
    if (!right) return left;
    pushdown(left);
    pushdown(right);
    if (left->pri < right->pri) {
        left->r = merge(left->r, right);
        update(left);
        return left;
    } else {
        right->l = merge(left, right->l);
        update(right);
        return right;
    }
}

// 在位置 pos 插入字符 val（0 ≤ pos ≤ 当前长度）
void insert(Node*& root, int pos, char val) {
    Node *left, *right;
    split(root, pos, left, right);
    root = merge(merge(left, new Node(val)), right);
}

// 删除位置 pos 的字符（0 ≤ pos < 长度）
void erase(Node*& root, int pos) {
    Node *left, *mid, *right;
    split(root, pos, left, mid);
    split(mid, 1, mid, right);
    delete mid;
    root = merge(left, right);
}

// 翻转区间 [l, r)（0 ≤ l < r ≤ 长度）
void reverse(Node*& root, int l, int r) {
    Node *left, *mid, *right;
    split(root, l, left, mid);
    split(mid, r - l, mid, right);
    if (mid) mid->rev ^= 1;
    root = merge(merge(left, mid), right);
}

// 输出整个文本（中序遍历）
void inorder(Node* root) {
    if (!root) return;
    pushdown(root);
    inorder(root->l);
    cout << root->val;
    inorder(root->r);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string init;
    getline(cin, init);
    Node* root = nullptr;
    for (char c : init) {
        insert(root, getSize(root), c);
    }

    int n;
    cin >> n;
    cin.ignore(); // 忽略换行

    int posL = 0, posR = getSize(root); // 左光标在第一个字符左，右光标在最后一个字符右

    while (n--) {
        string line;
        getline(cin, line);
        istringstream iss(line);
        string op;
        iss >> op;

        if (op == "<") {
            string cur;
            iss >> cur;
            if (cur == "L") {
                if (posL == 0) cout << "F\n";
                else { posL--; cout << "T\n"; }
            } else { // "R"
                if (posR == 0) cout << "F\n";
                else { posR--; cout << "T\n"; }
            }
        } 
        else if (op == ">") {
            string cur;
            iss >> cur;
            int len = getSize(root);
            if (cur == "L") {
                if (posL == len) cout << "F\n";
                else { posL++; cout << "T\n"; }
            } else { // "R"
                if (posR == len) cout << "F\n";
                else { posR++; cout << "T\n"; }
            }
        }
        else if (op == "I") {
            string cur;
            char c;
            iss >> cur >> c;
            int len = getSize(root);
            if (cur == "L") {
                int p = posL;
                insert(root, p, c);
                // 更新右光标：如果右光标在插入点右侧（严格），则右移
                if (posR > p) posR++;
                cout << "T\n";
            } else { // "R"
                int p = posR;
                insert(root, p, c);
                if (posL > p) posL++;
                cout << "T\n";
            }
        }
        else if (op == "D") {
            string cur;
            iss >> cur;
            int len = getSize(root);
            if (cur == "L") {
                if (posL == len) {
                    cout << "F\n";
                } else {
                    int p = posL;
                    erase(root, p);
                    if (posR > p) posR--;
                    cout << "T\n";
                }
            } else { // "R"
                if (posR == len) {
                    cout << "F\n";
                } else {
                    int p = posR;
                    erase(root, p);
                    if (posL > p) posL--;
                    cout << "T\n";
                }
            }
        }
        else if (op == "R") {
            if (posL < posR) {
                reverse(root, posL, posR);
                cout << "T\n";
            } else {
                cout << "F\n";
            }
        }
        else if (op == "S") {
            inorder(root);
            cout << "\n";
        }
    }

    return 0;
}