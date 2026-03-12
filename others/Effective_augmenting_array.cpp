#include <bits/stdc++.h>
using namespace std;

const int N = 1e5;
int n_max = 1e3;
int n[N] = {0, n_max};
int k = 0;

int search(int x, int l, int r){
    if(r - l <= 1){
        return l;
    }
    int mid = (r + l)/2;
    if(x <= n[mid]){
        return search(x, l, mid);
    }
    return search(x, mid, r);
}

void add(int x){
    int i = search(x, 0, k+1);
    if(i < k){
        n[i + 1] = x;
    }else{
        n[k + 1] = x;
        k++;
        n[k + 1] = n_max;
    }
}