#include <bits/stdc++.h>
#include <iomanip>
using namespace std;

struct bus{
        int s;
        int t;
        float tm;
        bool operator<(const bus& other) const {
            return s < other.s;
        }
    };

struct per{
    int id;
    int s;
    float tm;
};


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int n, m;
    int l, x, y;
    cin >> n >> m >> l >> x >> y;

    float xx = (float)1/x;
    float yy = (float)1/y;
    
    bus a[n + 1];
    per p[m];
    
    for(int i = 0; i < n; i++){
        cin >> a[i].s >> a[i].t;
         a[i].tm = (a[i].t - a[i].s)*xx + (float)(l - a[i].t)*yy;
    }
    a[n] = {l, l, 0};
    

    for(int i = 0; i < m; i++){
        p[i].id = i;
        cin >> p[i].s;
        p[i].tm = (float)(l - p[i].s)*yy;
    }

    
    sort(a, a+n+1);
    for(int i = 1; i < n; i ++){
        a[i].tm = min(a[i].tm, a[i-1].tm);
    }

    

    sort(p, p+m, [](per x, per y){ return x.s < y.s; });
    int ii = 0, jj = 0;
    while(jj < m and p[jj].s < a[0].s){
        jj++;
    }
    for(int ii = 0; ii < n; ii++){
        while(jj < m && p[jj].s < a[ii+1].s){
            p[jj].tm = min(a[ii].tm, p[jj].tm);
            jj++;
        }
    }
    
    sort(p, p+m, [](per x, per y){ return x.id < y.id; });

    cout.precision(10);
    for(int i = 0; i < m; i++){
        cout << fixed << p[i].tm << ' ';
    }   







    return 0;
}