#include <cstdio>


const int N = 2e5 + 10;
int n, q, m;
int l[N], r[N], v[N], pt[2*N];


struct nd {
    int l, r;
    nd* lc;
    nd* rc;
    int val = -1;
    nd(int l, int r) : l(l), r(r), lc(nullptr), rc(nullptr) {}
};


void sort_pt(int i, int j) {  
    if (i < j) {
        int mid = (i + j) / 2;
        sort_pt(i, mid);
        sort_pt(mid + 1, j);
        int* tmp = new int[j - i + 1];
        int p1 = i, p2 = mid + 1, k = 0;
        while (p1 <= mid && p2 <= j) {
            if (pt[p1] < pt[p2]) tmp[k++] = pt[p1++];
            else tmp[k++] = pt[p2++];
        }
        while (p1 <= mid) tmp[k++] = pt[p1++];
        while (p2 <= j) tmp[k++] = pt[p2++];
        for (int t = 0; t < k; t++) pt[i + t] = tmp[t];
        delete[] tmp;
    }
}


int rem_dup() {
    int cnt = 0;
    for (int i = 1; i <= 2*n; i++) {
        if (i == 1 || pt[i] != pt[i-1]) {
            pt[++cnt] = pt[i];
        }
    }
    return cnt;
}


void buildSegTree(nd* node) {
    if (node->l + 2 > node->r) {
        return;
    }
    int mid = (node->l + node->r) / 2;
    node->lc = new nd(node->l, mid);
    node->rc = new nd(mid, node->r);
    buildSegTree(node->lc);
    buildSegTree(node->rc);
}


void add_val(int i, nd* node) {
    int mid = (node->l + node->r) / 2;
    if (l[i] <= pt[node->l] && pt[node->r] <= r[i]) {
        if (node->val < v[i]) {
            node->val = v[i];
        }
    } else {
        if (node->lc && l[i] < pt[mid]) add_val(i, node->lc);
        if (node->rc && r[i] > pt[mid]) add_val(i, node->rc);
    }
}


int que(int x, nd* n) {
    int ans = n->val;
    int mid = (n->l + n->r) / 2;
    if (x < pt[mid] && n->lc) {
        int tmp = que(x, n->lc);
        if (tmp > ans) ans = tmp;
    } else if (x >= pt[mid] && n->rc) {
        int tmp = que(x, n->rc);
        if (tmp > ans) ans = tmp;
    }
    return ans;
}


int main() {
    scanf("%d%d%d", &n, &q, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d%d%d", &l[i], &r[i], &v[i]);
        l[i]++;
        pt[2*i-1] = l[i];
        pt[2*i] = r[i];
    }

    sort_pt(1, 2*n);
    int nn = rem_dup();

    nd root(1, nn);
    buildSegTree(&root);
    for (int i = 1; i <= n; i++) {
        add_val(i, &root);
    }

    int x;
    while (q--) {
        scanf("%d", &x);
        if (x < pt[1] || x >= pt[nn]) {
            printf("-1\n");
        } else {
            printf("%d\n", que(x, &root));
        }
    }

    return 0;
}