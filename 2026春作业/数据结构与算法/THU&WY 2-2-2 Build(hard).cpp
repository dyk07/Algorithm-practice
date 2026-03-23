#include <cstdio>
#include <queue>


const int nn = 1e6 + 5;


struct nd {
    int par;
    int chnm = 0;
    int fch = 0;
    int el = 0;
    int yo = 0;
    int ht = 0;
    int sz = 1;
};
nd n[nn];


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
            return tmp;
        }
    }
    return tmp;
}


void rem(int s) {
    int p = n[s].par;
    n[p].chnm--;
    n[p].sz -= n[s].sz;
    int prev = n[s].el;
    int next = n[s].yo;
    if (prev == 0) {
        n[p].fch = next;
        if (next != 0) n[next].el = 0;
    } else {
        n[prev].yo = next;
        if (next != 0) n[next].el = prev;
    }
}


void ins(int s, int d, int k) {
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
}


void mov(){
    int s, d, k;
    s = loc();
    rem(s);
    d = loc();
    scanf("%d", &k);
    ins(s, d, k);
}


void quer(){
    int s = loc();
    return n[s].sz;
}


void queh(){
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

    int tp, s, d, k;
    while (M--) {
        scanf("%d", &tp);
        switch (tp) {
            case 0: mov(); break;
            case 1: printf("%d", queh()); break;
            case 2: printf("%d", ques()); break;
        }
    }



    return 0;
}