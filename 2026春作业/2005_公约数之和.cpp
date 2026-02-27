#include <cstdio>
using namespace std;

int f(int x, int y){
    int m, n;
    if(x < y){
        m = x;
        n = y;
    }else{
        m = y;
        n = x;
    }
    if(m == 0){
        return n;
    }
    return f(m, n%m);
}

int g(int x){
    int s = 0;
    for(int i = 1; i*i <= x; i++){
        if(x%i == 0){
            if(i*i == x){
                s += i;
            }else{
                s += i + x/i;
            }
        }
    }
    return s;
}

int main(){

    int x, y;
    scanf("%d%d", &x, &y);
    printf("%d", g(f(x, y)));

    return 0;
}
