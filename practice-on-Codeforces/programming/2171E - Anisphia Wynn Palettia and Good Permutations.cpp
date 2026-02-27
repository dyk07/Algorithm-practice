#include <bits/stdc++.h>
using namespace std;


int main(){

    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;

        int i = 1;
        int j = 0;
        while(6*j + 5 <= n){
            cout << 6*j + 5 << ' ';
            cout << 2*i << ' ';
            i++;
            cout << 2*i << ' ';
            i++;
            j++;
        }
        j = 0;
        while(2*i + 2 <= n){
            cout << 6*j + 1 << ' ';
            cout << 2*i << ' ';
            i++;
            cout << 2*i << ' ';
            i++;
            j++;
        }
        if(2*i <= n){
            cout << 2*i << ' ';
        }
        int k = 0;
        while(6*j + 1 <= n && 6*k + 9 <= n){
            cout << 6*j + 1 << ' ';
            j++;
            cout << 6*k + 3 << ' ';
            k++;
            cout << 6*k + 3 << ' ';
            k++;
        }
        while(6*j + 1 <= n){
            cout << 6*j + 1 << ' ';
            j++;
        }
        while(6*k + 3 <= n){
            cout << 6*k + 3 << ' ';
            k++;
        }
        cout << endl;

    }



    return 0;
}