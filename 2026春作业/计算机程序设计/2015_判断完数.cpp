#include <cstdio>
using namespace std;


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
    int n;
    scanf("%d", &n);

    if(g(n) == 2*n){
        printf("Y");
    }else{
        printf("N");
    }


    return 0;
}