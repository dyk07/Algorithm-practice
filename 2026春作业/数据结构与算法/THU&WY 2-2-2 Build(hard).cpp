#include <cstdio>
#include <algorithm>
#include <queue>
#include <stack>


const int nn = 1e6 + 5;
std::queue<int> Q;
std::stack<int> S;


struct nd {
    int par;
    int chnm = 0;
    int fch = 0;
    int el = 0;
    int yo = 0;
    int ht = 0;
    int sz = 1;
    int htl = 0;
};
nd n[nn];

/*
void init_sz(int x) {
    if (n[x].chnm == 0) {
        n[x].sz = 1;
        return;
    }
    int sz = 1;
    int id = n[x].fch;
    while (id != 0) {
        init_sz(id);
        sz += n[id].sz;
        id = n[id].yo;
    }
    n[x].sz = sz;
}


void init_ht(int x) {
    if (n[x].chnm == 0) {
        n[x].ht = 0;
        return;
    }
    int ht = 0;
    int id = n[x].fch;
    while (id != 0) {
        init_ht(id);
        ht = std::max(n[id].ht, ht);
        id = n[id].yo;
    }
    n[x].ht = ht + 1;
}


void init_htl(int x) {
    if (n[x].chnm) {
        init_htl(n[x].fch);
    }

    if (!n[x].yo) {
        n[x].htl = n[x].ht;
        return;
    }

    init_htl(n[x].yo);
    n[x].htl = std::max(n[x].ht, n[n[x].yo].htl);
}*/


void init() {
    Q.push(1);
    while (!Q.empty()) {
        int id = Q.front();
        S.push(id);
        Q.pop();
        id = n[id].fch;
        while (id) {
            Q.push(id);
            id = n[id].yo;
        }
    }

    while (!S.empty()) {
        int x = S.top();
        S.pop();
        n[x].sz = 1;
        int id = n[x].fch;
        n[x].ht = (id) ? n[id].htl + 1 : 0;
        while (id) {
            n[x].sz += n[id].sz;
            id = n[id].yo;
        }
        n[x].htl = (n[x].yo) ? std::max(n[x].ht, n[n[x].yo].htl) : n[x].ht;
    }
}


int loc(){
    int l, k;
    scanf("%d", &l);
    int tmp = 1;
    for (int i = 0; i < l; i++) {
        scanf("%d", &k);
        if (n[tmp].chnm > k) {
            tmp = n[tmp].fch;
            while (k--) {
                tmp = n[tmp].yo;
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


void updt_sz_abv(int p, int sz) {
    while (p != 1 && p != 0){
        n[p].sz += sz;
        p = n[p].par;
    }
}


void updt_ht_abv(int s) {
    while (s != 1) {
        while (1) {
            n[s].htl = std::max(n[s].ht, n[n[s].yo].htl);
            if (n[s].el == 0) {
                break;
            }
            s = n[s].el;
        }
        n[n[s].par].ht = n[s].htl + 1;
        s = n[s].par;
    }
}


void rem(int s) {
    if(s == 1) return;
    int p = n[s].par;
    n[p].chnm--;
    updt_sz_abv(p, -n[s].sz);
    int prev = n[s].el;
    int next = n[s].yo;
    if (prev == 0) {
        n[p].fch = next;
        if (next != 0) n[next].el = 0;
    } else {
        n[prev].yo = next;
        if (next != 0) n[next].el = prev;
    }
    if (prev) {
        updt_ht_abv(prev);
    } else {
        if(next){
            n[p].ht = n[next].htl + 1;
        } else {
            n[p].ht = 0;
        }
        updt_ht_abv(p);
    }
    
}


void ins(int s, int d, int k) {
    updt_sz_abv(d, n[s].sz);
    n[s].par = d;
    n[d].chnm++;
    if (k == 0) {
        n[s].yo = n[d].fch;
        n[s].el = 0;
        n[d].fch = s;
        if (n[s].yo != 0) n[n[s].yo].el = s;
    } else {
        int t = n[d].fch;
        for (int i = 1; i < k; i++) {
            t = n[t].yo;
        }
        int next = n[t].yo;
        n[t].yo = s;
        n[s].el = t;
        n[s].yo = next;
        if (next != 0) n[next].el = s;
    }
    updt_ht_abv(s);
}


void mov(){
    int s, d, k;
    s = loc();
    rem(s);
    d = loc();
    scanf("%d", &k);
    ins(s, d, k);
}


int ques(){
    int s = loc();
    return n[s].sz;
}


int queh(){
    int s = loc();
    return n[s].ht;
}


int main(){
    int N, M;
    scanf("%d%d", &N, &M);

    for (int i = 1; i < N + 1; i++) {
        int k, t, r;
        scanf("%d", &k);
        n[i].chnm = k;
        if(k--){
            scanf("%d", &t);
            n[i].fch = t;
            n[t].par = i;
            while(k--){
                scanf("%d", &r);
                n[r].par = i;
                n[t].yo = r;
                n[r].el = t;
                t = r;
            }
        }
    }
    init();


    int tp;
    while (M--) {
        scanf("%d", &tp);
        switch (tp) {
            case 0: mov(); break;
            case 1: printf("%d\n", queh()); break;
            case 2: printf("%d\n", ques());
        }
    }



    return 0;
}