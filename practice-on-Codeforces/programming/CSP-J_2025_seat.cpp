#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, m, r = 1, s, cur;
    cin >> n >> m >> s;
    int k = n*m - 1;

    while(k--){
        cin >> cur;
        if(cur > s){
            r += 1;
        }
    }
    int i = (r - 1) / (2*n), j = r % (2*n);
    if(j <= n){
        cout << j << ' ' << 2*i + 1;
    }else{
        cout << 2 * n + 1 - j << ' ' << 2*i + 2;
    }


    return 0;
}