#include <cstdio>
using namespace std;


bool f(int n){
    if(n == 2){
        return true;
    }
    if(n%2 == 0){
        return false;
    }

    for(int i = 3; i*i <= n; i+=2){
        if(n%i == 0){
            return false;
        }
    }

    return true;
}

int main(){

    int n, s = 0, i = 2;
    scanf("%d", &n);
    while(n > 0){
        if(f(i)){
            s += i;
            n--;
        }
        i++;
    }
    printf("%d", s);


    return 0;
}