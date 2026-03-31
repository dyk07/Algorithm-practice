#include <cstdio>

const int nn = 1e6 + 5;

int par[nn] = {0}, chnm[nn] = {0}, fch[nn] = {0}, el[nn] = {0}, yo[nn] = {0};
int q[nn];

int loc() {
    int l, k;
    scanf("%d", &l);
    int tmp = 1;
    for (int i = 0; i < l; i++) {
        scanf("%d", &k);
        if (chnm[tmp] > k) {
            tmp = fch[tmp];
            while (k--) {
                tmp = yo[tmp];
            }
        } else {
            while (i < l - 1) {
                scanf("%d", &k);
                i++;
            }
            return tmp;
        }
    }
    return tmp;
}

void rem(int s) {
    int p = par[s];
    chnm[p]--;
    int prev = el[s];
    int next = yo[s];
    if (prev == 0) {
        fch[p] = next;
        if (next != 0) el[next] = 0;
    } else {
        yo[prev] = next;
        if (next != 0) el[next] = prev;
    }
}

void ins(int s, int d, int k) {
    par[s] = d;
    chnm[d]++;
    if (k == 0) {
        yo[s] = fch[d];
        el[s] = 0;
        fch[d] = s;
        if (yo[s] != 0) el[yo[s]] = s;
    } else {
        int t = fch[d];
        for (int i = 1; i < k; i++) {
            t = yo[t];
        }
        int next = yo[t];
        yo[t] = s;
        el[s] = t;
        yo[s] = next;
        if (next != 0) el[next] = s;
    }
}

int main() {
    int N, M;
    scanf("%d%d", &N, &M);

    for (int i = 1; i <= N; i++) {
        int k, t, r;
        scanf("%d", &k);
        chnm[i] = k;
        if (k--) {
            scanf("%d", &t);
            fch[i] = t;
            par[t] = i;
            while (k--) {
                scanf("%d", &r);
                par[r] = i;
                yo[t] = r;
                el[r] = t;
                t = r;
            }
        }
    }

    int s, d, k;
    while (M--) {
        s = loc();
        rem(s);
        d = loc();
        scanf("%d", &k);
        ins(s, d, k);
    }

    q[0] = 1;
    int cr = 0, cn = 1;
    int cnt = 0;
    while (cr < cn && cnt < N) {
        int t = q[cr++];
        printf("%d\n", t);
        cnt++;
        int c = fch[t];
        while (c != 0) {
            q[cn++] = c;
            c = yo[c];
        }
    }

    return 0;
}