#include <cstdio>
#include <algorithm>
using namespace std;

const int N = 300005;
int Hl[N], Hr[N], Q[N], Q_[N], n, m, h = 0, q = 0;
bool t[N];

struct nd {
    int l;
    int r;
    nd* lc;
    nd* rc;
    int cnt;
};

nd* _rt;

nd* build_seg_tree(int l, int r) {
    if (l >= r) {
        return nullptr;
    }

    nd* rt = new nd{l, r, nullptr, nullptr, 0};

    if (r - l > 1) {
        int mid = (l + r) / 2;
        nd* lc = build_seg_tree(l, mid);
        nd* rc = build_seg_tree(mid, r);
        rt->lc = lc;
        rt->rc = rc;
    }

    return rt;
}

void flip(nd* rt, int l, int r) {
    if (!rt || r < Q_[rt->l] || Q_[rt->r - 1] < l) return;

    if (l <= Q_[rt->l] && Q_[rt->r - 1] <= r) {
        rt->cnt++;
        return;
    }

    flip(rt->lc, l, r);
    flip(rt->rc, l, r);
}

int query(nd* rt, int x) {
    int mid = (rt->l + rt->r)/2;
    if (rt->lc) {
        if (x >= Q_[mid]) {
            return rt->cnt + query(rt->rc, x);
        } 
        return rt->cnt + query(rt->lc, x);
    }
    return rt->cnt;
}

int main() {
    scanf("%d%d", &n, &m);
    char c;
    for (int i = 0; i < m; i++) {
        scanf(" %c", &c);
        if (c == 'H') {
            t[i] = true;
            scanf("%d%d", &Hl[h], &Hr[h]);
            h++;
        } else {
            t[i] = false;
            scanf("%d", &Q[q]);
            Q_[q] = Q[q];
            q++;
        }
    }

    sort(Q_, Q_ + q);
    _rt = build_seg_tree(0, q);

    int a = 0, b = 0;
    for (int i = 0; i < m; i++) {
        if (t[i]) {
            flip(_rt, Hl[a], Hr[a]);
            a++;
        } else {
            printf("%d\n", query(_rt, Q[b]));
            b++;
        }
    }

    return 0;
}