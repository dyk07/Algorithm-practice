#include <bits/stdc++.h>
using namespace std;


const int N = 2e5 + 5;
int yy;
int n[N];
int kk = 0;

int search(int x, int l, int r){
    if(r - l <= 1){
        return l;
    }
    int mid = (r + l)/2;
    if(x < n[mid]){
        return search(x, l, mid);
    }
    return search(x, mid, r);
}

void add(int x, int new_yy){
    int i = search(x, 0, kk+1);
    if(i < kk){
        n[i + 1] = x;
    }else{
        n[kk + 1] = x;
        kk++;
        n[kk + 1] = new_yy + 1;
    }
}


int v[N] = {0};

struct ar{
    int id;
    int n;
    int m;
    int x;
    int y;
    int ans;
};



int main(){
    int t, yy = 0;
    cin >> t;
    ar a[t];
    for(int i = 0; i < t; i++){
        a[i].id = i;
        cin >> a[i].n >> a[i].m;
        a[i].x = a[i].n - 1;
        a[i].y = a[i].m - a[i].n;
        yy = max(yy, a[i].y);
    }
    n[1] = yy + 1;

    sort(a, a+t, [](ar a, ar b){ return a.x < b.x || a.x == b.x && a.y < b.y; });

    
    int ct = 0;
    stack<int> visited = {};
    for(int cr_x = 1; ct < t; cr_x++){
        
        long long cr = cr_x + 1;
        while(cr <= cr_x + yy + 1){
            for(int k = 1; k*cr <= cr_x + yy + 1; k++){
                if(cr == cr_x + 1){
                    visited.push(k*cr);
                }
                v[k*cr]++;
            }
            cr *= cr_x + 1;
        }

        while(!visited.empty()){
            int cur = visited.top();
            visited.pop();
            while(v[cur]--){
                add(cur - cr_x - 1, yy);
            }
            v[cur]++;
        }

        while(ct < t && a[ct].x == cr_x){
            a[ct].ans = search(a[ct].y, 0, kk+1);
            ct++;
        }

        
    }    


    sort(a, a+t, [](ar a, ar b){ return a.id < b.id; });
    for(int i = 0; i < t; i++){
        cout << a[i].ans << ' ';
    }




    return 0;
}