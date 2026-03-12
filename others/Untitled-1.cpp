#include <bits/stdc++.h>
using namespace std;

const int N = 8000010;
int ne[2][N];
bool f[N];

int i, j;
bool fi, fj;

void left_cursor_mv_right() {
    fi ^= f[i];
    i = ne[fi][i];
}

void rev() {
    f[i] ^= 1, f[j] ^= 1;
    
}

int main() {


    return 0;
}