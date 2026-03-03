#include <cstdio>
using namespace std;


int f(int n){
    return (4 + n - 1998 + (n - 1997)/4 - (n - 1901)/100 + (n - 1601)/400)%7;
}

bool g(int n){
    if(n%400 == 0){
        return true;
    }
    if((n%4 == 0) & (n%100 != 0)){
        return true;
    }
    return false;
}


int main(){
    int n, a, c = 0;
    scanf("%d", &n);

    a = f(n);
    for(int i = 1; i < 13; i++){
        if(a == 0){
            c++;
        }
        if(i == 2){
            if(g(n)){
                a = (a + 1)%7;
            }
        }else if(i == 4 || i == 6 || i == 9 || i == 11){
            a = (a + 2)%7;
        }else{
            a = (a + 3)%7;
        }
    }

    printf("%d", c);
    


    return 0;
}