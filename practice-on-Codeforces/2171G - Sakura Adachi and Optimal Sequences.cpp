#include <bits/stdc++.h>
using namespace std;


const int N = 1e6 + 3;
int a[N], b[N], add[N];
int fac[N], inv[N];


int main(){

    fac[0] = 1;
    for(int i = 1; i < N; i++){
        fac[i] = ((long long)fac[i - 1]*i)%N;
    }

    inv[1] = 1;
    inv[N - 1] = N - 1;
    inv[2] = (N + 1)/2;
    for(int i = 3; i < N - 1; i++){
        inv[i] = ((long long)(N/i)*(N - inv[N%i]))%N;
    }


    int tt;
    cin >> tt;
    while(tt--){
        int n;
        cin >> n;
        int db = 50;
        for(int i = 0; i < n; i++){
            cin >> a[i];
        }
        for(int i = 0; i < n; i++){
            cin >> b[i];
            int cur = 0, tem = b[i]/2;
            while(tem >= a[i]){
                tem /= 2;
                cur++;
            }
            db = min(db, cur);
        }

        long long x = db;
        long long cc;
        long long cnt = 1;
        while(db--){
            int cc = 0;
            for(int i = 0; i < n; i++){
                int tmp = b[i] - 2*(b[i]/2);
                b[i] /= 2;
                cc += tmp;
            }
            x += cc;
            cnt = ((long long)cnt*fac[cc])%N;
        }
        
        cc = 0;
        for(int i = 0; i < n; i++){
            add[i] = b[i] - a[i];
            cc += add[i]; 
        }
        x += cc;
        if(cc >= N){
            cnt = 0;
        }else{
            cnt = ((long long)cnt*fac[cc])%N;
            for(int i = 0; i < n; i++){
                cnt = ((long long)inv[fac[add[i]]]*cnt)%N;
            }
        }
        cout << x << ' ' << cnt << endl;

    }



    return 0;
}