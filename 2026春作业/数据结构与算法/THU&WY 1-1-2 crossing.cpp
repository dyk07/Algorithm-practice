#include <cstdio>
using namespace std;


const int N = 4e5 + 5;
int A[N];
int B[N];
int tmp[N];


long long merge(int lo, int mi, int hi, int k){
    long long sum = 0;
    int la = mi - lo, lb = hi - mi, a = 0, b = 0;
    for(int i = 0; i < la; i++){
        tmp[i] = B[lo + i];
    }
    while((a < la) && (b < lb)){
        if((k == 1 && tmp[a] <= B[mi + b]) || (k == 4 && (long long)A[tmp[a]]*B[mi + b] >= (long long)A[B[mi + b]]*tmp[a]) || (k == 2 && (long long)A[tmp[a]]*B[mi + b] <= (long long)A[B[mi + b]]*tmp[a])){
            B[lo++] = tmp[a++];
        }else{
            sum += la - a;
            B[lo++] = B[mi + (b++)];
        }
    }
    while(a < la){
        B[lo++] = tmp[a++];
    }
    return sum;
}


long long cal(int lo, int hi, int k){
    if(lo + 1 >= hi) return 0;
    int mi = (lo + hi)/2;
    long long t1 = cal(lo, mi, k), t2 = cal(mi, hi, k);
    long long t3 = merge(lo, mi, hi, k);
    return t1 + t2 + t3;
}


int main(){
    int n;
    scanf("%d", &n);
    for(int i = 1; i < n + 1; i++){
        scanf("%d", &A[i]);
        B[i] = A[i];
    }

    long long s1 = cal(1, n + 1, 1);

    long long s4 = cal(1, n + 1, 4);

    for(int i = 1; i < n + 1; i++){
        B[i] = i;
    }
    long long s2 = cal(1, n + 1, 2) - s1;

    printf("%lld %lld %lld", s1, s4, s2);

    return 0;
}