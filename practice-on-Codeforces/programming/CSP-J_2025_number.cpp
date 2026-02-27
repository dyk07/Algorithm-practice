#include <bits/stdc++.h>
using namespace std;

int main(){
    string a;
    cin >> a;
    
    for(char c = '9'; c >= '0'; c--){
        int t = count( a.begin(), a.end(), c);
        while(t--){
            cout << c;
        }
    }



    return 0;
}