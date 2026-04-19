#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;
int p[N];
int u[N];
int v[N];
bool ncon[N];

int main(){

    int tt;
    cin >> tt;
    while(tt--){

        int n, cnt = 0;
        cin >> n;
        for(int i = 0; i < n; i++){
            cin >> p[i];
            ncon[i] = 1;
        }
        ncon[0] = 0;

        int min_value = p[0];
        bool nw = 1;
        while(nw){
            nw = 0;
            for(int i = 1; i < n; i++){
                if(min_value > p[i]){
                    if(!ncon[i]){
                        min_value = p[i];
                    }
                }else{
                    if(ncon[i]){
                        u[cnt] = min_value;
                        v[cnt] = p[i];
                        cnt++;
                        ncon[i] = 0;
                        nw = 1;
                    }
                }
            }

            int max_value = 0;
            for(int i = n - 1; i >= 0; i--){
                if(p[i] > max_value){
                    if(!ncon[i]){
                        max_value = p[i];
                    }
                }else{
                    if(ncon[i]){
                        u[cnt] = max_value;
                        v[cnt] = p[i];
                        cnt++;
                        ncon[i] = 0;
                        nw = 1;
                    }
                }

            }
        }
        if(cnt != n - 1){
            cout << "No" << endl;
        }else{
            cout << "Yes" << endl;
            for(int i = 0; i < cnt; i++){
                cout << u[i] << ' ' << v[i] << endl;
            }
        }


    }

    return 0;
}