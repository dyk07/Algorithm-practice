#include <cstdio>
#include <algorithm>
using namespace std;

const int N = 2e5 + 5;
int a[N], b[N];

bool above(int a, int b, int x, int y){
    return (long long)a*y + (long long)b*x - (long long)a*b >= 0;
}

int s(int x, int y, int lo, int hi){
    if(!above(a[lo], b[lo], x, y)) return lo;
    while(hi - lo > 1){
        int mid = ((long long)lo + hi)/2;
        if(above(a[mid], b[mid], x, y)) lo = mid;
        else hi = mid;
    }
    return lo + 1;
}


int main(){
    int n, m, x, y;
    scanf("%d%d", &n, &m);
    for(int i = 0; i < n; i++){
        scanf("%d%d", &a[i], &b[i]);
    }
    sort(a, a + n);
    sort(b, b + n);
    for(int i = 0; i < m; i++){
        scanf("%d%d", &x, &y);
        printf("%d \n", s(x, y, 0, n));
    }




    return 0;
}