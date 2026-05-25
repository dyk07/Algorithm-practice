#include <cstdio>

int n, crt = 0, nxt = 0;

struct p {
    int id;
    int pr;
    int st;
    int d;
};

const int N = 1e6 + 5;
p ps[N];
int sz = 0;

bool pr(p a, p b) {
    return a.pr > b.pr || (a.pr == b.pr && a.st < b.st) || (a.pr == b.pr && a.st == b.st && a.id < b.id);
}

void add(p tmp) {
    ps[sz++] = tmp;
    int i = sz - 1;
    while (i > 0) {
        int f = (i - 1) / 2;
        if (pr(ps[i], ps[f])) {
            p tmp = ps[i];
            ps[i] = ps[f];
            ps[f] = tmp;
            i = f;
        } else {
            break;
        }
    }
}

void rem() {
    ps[0] = ps[--sz];
    int i = 0;
    while (true) {
        int l = i * 2 + 1, r = i * 2 + 2, t = i;
        if (l < sz && pr(ps[l], ps[t])) {
            t = l;
        }
        if (r < sz && pr(ps[r], ps[t])) {
            t = r;
        }
        if (t != i) {
            p tmp = ps[i];
            ps[i] = ps[t];
            ps[t] = tmp;
            i = t;
        } else {
            break;
        }
    }
}

void exc() {
    while (sz > 0 && nxt - crt >= ps[0].d) {
        crt += ps[0].d;
        printf("%d %d\n", ps[0].id, crt);
        rem();
    }
    if (sz > 0) {
        ps[0].d -= nxt - crt;
    }
    crt = nxt;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        p tmp;
        scanf("%d%d%d%d", &tmp.id, &tmp.pr, &tmp.st, &tmp.d);
        nxt = tmp.st;
        exc();
        add(tmp);
    }
    nxt = 2147483647;
    exc();

    return 0;
}