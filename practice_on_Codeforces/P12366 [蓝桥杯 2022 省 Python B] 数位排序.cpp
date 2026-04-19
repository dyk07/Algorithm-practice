#include <bits/stdc++.h>
using namespace std;


int f(int x){
    if(x < 10){
        return x;
    }
    return x%10 + f(x/10);
}


bool cmp(int a, int b){
    return f(a) < f(b) || (f(a) == f(b) && a < b);
}


const int N = 1e6 + 5;
int a[N];

int main(){
    int n, m;
    cin >> n >> m;
    for(int i = 1; i <= n; i++){
        a[i] = i;
    }
    sort(a + 1, a + n + 1, cmp);

    cout << a[m];


    return 0;
}