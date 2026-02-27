#include<bits/stdc++.h>
using namespace std;


int main(){

    int n;
    cin >> n;
    int p[n];
    for(int i = 0; i < n; i++){
        cin >> p[i];
    }

    int a[n], b[n];
    a[0] = p[0];
    for(int i = 1; i < n; i++){
        a[i] = gcd(a[i-1], p[i]);
    }
    b[n-1] = p[n-1];
    for(int i = n-2; i >= 0; i--){
        b[i] = gcd(b[i+1], p[i]);
    }
    
    long long s = 0;
    for(int i = 0; i < n - 1; i++){
        s += min(a[i], b[i]);
    }

    cout << s;


    return 0;
}