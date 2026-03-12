#include <bits/stdc++.h>
using namespace std;

const long long N = 1e12;


int main(){
    int t;
    cin >> t;
    while(t--){
        long long x, y, k;
        cin >> x >> y >> k;

        if(y == 1){
            cout << -1 << endl;
            continue;
        }

        long long ans = k;
        long long i = 0;
        while(i < x){
            long long d = (ans - 1)/(y - 1);
            if(d == 0){
                break;
            }
            
            long long di = ((y - 1)*(d + 1) - ans)/d + 1;
            if(i + di > x){
                ans += d*(x - i);
                i = x;
            }else{
                ans += d*di;
                i += di;
            }
           
            if(ans > N){
                ans = -1;
                break;
            }
        }

        cout << ans << endl;



    }
    

    return 0;
}