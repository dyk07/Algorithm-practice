#include <bits/stdc++.h>
using namespace std;

struct pt{
    long long num;
    int l;
    int r;
};

const int N = 2e5 + 5;
pt p[N];

int main(){
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;
        cin >> p[0].num;
        for(int i = 1; i < n - 1; i++){
            cin >> p[i].num; 
            p[i].l = i - 1;
            p[i].r = i + 1;
        }
        cin >> p[n - 1].num;
        p[0].l = n - 1;
        p[0].r = 1;
        p[n - 1].l = n - 2;
        p[n - 1].r = 0;

        long long cost = 0;
        int rmd = n;

        int crt = 0;
        while(rmd > 2){
            while(p[crt].num <= min(p[p[crt].l].num, p[p[crt].r].num)){
                rmd--;
                cost += min(p[p[crt].l].num, p[p[crt].r].num);
                p[p[crt].l].r = p[crt].r;
                p[p[crt].r].l = p[crt].l;
                crt = p[crt].l;
                if(rmd == 2){
                    break;
                }
            }
            crt = p[crt].r;
        }
        cost += max(p[crt].num, p[p[crt].r].num);
        
        cout << cost << endl;

    }



    return 0;
}