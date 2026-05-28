#include<algorithm>
#include<cstdio>
#include<cstdlib>
int LIMIT_INSERTION = 10;
inline int myrand(int lo, int hi){
    return lo + rand()%(hi-lo);
}
void insertionSort(int array[],int L, int R){
    for(int i=L+1;i<R;++i){
        for(int j=i;j>L;--j){
            if(array[j]<array[j-1]){
                std::swap(array[j],array[j-1]);
            }else{
                break;
            }
        }
    }
}

int partition(int array[], int lo, int hi){ //LGU 
    std::swap(array[lo], array[myrand(lo,hi)]);
    int pivot = array[lo];
    int mi = lo;
    for(int k = lo + 1; k < hi; ++k){
        if(array[k]<pivot){
            std::swap(array[++mi], array[k]);
        }
    }
    std::swap(array[lo], array[mi]);
    return mi;
}
void classic_quicksort(int array[ ], int lo, int hi){
    if(hi - lo <= 1) return;
    if(hi - lo <= LIMIT_INSERTION){
        insertionSort(array, lo, hi);
        return;
    }
    int mi = partition(array, lo, hi);
    classic_quicksort(array, lo, mi); 
    classic_quicksort(array, mi+1, hi);
}