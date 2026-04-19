#include <bits/stdc++.h>
using namespace std;

struct col{
    int num;
    int next;
};

struct bar{
    int pos;
    int hi;
    int chi;
    vector<bar*> tmpch = {};
    vector<bar*> ch = {};
    bar* par;
};

const int N = 5e5 + 5;
int tmp[N], add[N] = {0};
col a[N];
bar h[N];
bar extr[N];
int ex = 0;

void cal(int l, bar* br){
    int r = br->pos + 1;
    int ceil = br->hi;
    int val = br->chi;
    if(r - l <= 1){
        return;
    }

    int hei = ceil - val;

    int sl = l;
    for(bar* child : br->ch){
        cal(sl, child);
        sl = child->pos + 1;
    }

    int cnt = 0, cntmax = 0;
    int ii = l;
    while(ii < r){
        if(a[ii].num <= ceil && ii > 0){
            a[ii - 1].next = a[ii].next;
        }
        if(a[ii].num > val){
            if(a[ii].num >= ceil){
                cntmax += 1;
                add[ii] -= hei;
            }else{
                tmp[cnt] = a[ii].num - val;
                add[ii] -= tmp[cnt];
                cnt++;
            }
        }
        ii = a[ii].next;
    }
    if(cnt > 0){
        sort(tmp, tmp + cnt);
    }
    for(int i = 0; i < cntmax; i++){
        add[r - 1 - i] += hei;
    }
    for(int i = 0; i < cnt; i++){
        add[r - cntmax - cnt + i] += tmp[i];
    }
    
}

void tryadd(bar* x, bar* y){
    if(x->hi < y->hi){
        y->tmpch.push_back(x);
        x->par = y;
    }else{
        tryadd(x, y->par);
    }
}

void optm(bar* x){
    int l = x->tmpch.size();
    if(l == 0){
        x->chi = 0;
        return;
    }
    x->chi = x->tmpch[l - 1]->hi;
    int cur = l - 1;
    while(cur >= 0){
        if(x->tmpch[cur]->hi == x->chi){
            x->ch.push_back(x->tmpch[cur]);
            optm(x->tmpch[cur]);
        }else{
            break;
        }
        cur--;
    }
    if(cur >= 0){
        extr[ex].pos = x->pos;
        extr[ex].hi = x->chi;
        for(int i = 0; i <= cur; i++){
            extr[ex].tmpch.push_back(x->tmpch[i]);
        }
        x->ch.push_back(&extr[ex]);
        optm(&extr[ex]);
        ex++;
    }
    x->tmpch.clear();
}

int main(){

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);


    int n;
    cin >> n;
    for(int i = 0; i < n; i++){
        cin >> a[i].num;
        a[i].next = i + 1;
    }
    for(int i = 0; i < n - 1; i++){
        h[i].pos = i;
        cin >> h[i].hi;
    }
    h[n - 1].pos = n - 1;
    h[n - 1].hi = n;

    for(int i = n - 2; i >= 0; i--){
        tryadd(&h[i], &h[i + 1]);
    }

    optm(&h[n - 1]);

    cal(0, &h[n - 1]);

    for(int i = 0; i < n; i++){
        cout << a[i].num + add[i] << ' ';
    }


    return 0;
}