#include <bits/stdc++.h>
using namespace std;

const int N = 100005;
struct num{
    long long id;
    long long v;
    long long cnt;
};

num a[N];
int n, m, q;

int isg(char c){
    return c == '+' or c == '*';
}

int add(long long v){
    if(v > 1000000) return 1;
    int l = 0;
    int r = q-1;
    while(l <= r){
        int mid = l + (r-l)/2;
        if(v == a[mid].v){
            a[mid].cnt++;
            return 0;
        }
        if(v < a[mid].v){
            r = mid -1;
            continue;
        }
        l = mid + 1;
    }
    return 0;
}



bool vcmp(num x, num y){
    return x.v < y.v;
}
bool idcmp(num x, num y){
    return x.id < y.id;
}

int main(){
    cin >> n >> m >> q;
    char g[n][m];
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> g[i][j];
        }
    }
    for(int i = 0; i < q; i++){
        cin >> a[i].v;
        a[i].cnt = 0;
        a[i].id = i;
    }
    
    sort(a, a+q, vcmp);


    long long val, sum, pro;
    bool sgn = 0;
    for(int x = -1; x < 2; x++){
        for(int y = -1; y < 2; y++){
            if(x or y){
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        val = 0;
                        sum = 0;
                        pro = 1;
                        sgn = 1;
                        for(int k = 0; i + x*k >= 0 && i + x*k < n && j + y*k >= 0 && j + y*k < m; k++){
                            char crt = g[i + x*k][j + y*k];
                            if(sgn && isg(crt)){
                                break;
                            }
                            if(isg(crt)){
                                pro *= val;
                                val = 0;
                                sgn = 1;
                                if(crt == '+'){
                                    sum += pro;
                                    pro = 1;
                                }
                                continue;
                            }
                            int crn = crt - '0';
                            val = val * 10 + crn;
                            if(k){
                                int tmp = add(sum + pro * val);
                                if(tmp == 1) break;
                            }
                            sgn = 0;
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            int cc = g[i][j] - '0';
            add(cc);
        }
    }
    

    sort(a, a+q, idcmp);
    for(int i = 0; i < q; i++){
        cout << a[i].cnt << endl;
    }

    return 0;
}