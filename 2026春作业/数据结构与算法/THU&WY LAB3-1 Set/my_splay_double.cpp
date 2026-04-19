#include <stdio.h>
#include <stdlib.h>
#include "lib/splay/splay.h"

// 双旋 splay 实现
Splay<int, false> splay;
int N;

void insert(int key) {
    splay.insert(key);
}

bool erase(int key) {
    return splay.remove(key);
}

int lower_bound(int key) {
    BinNodePosi<int> p = splay.lower_bound(key);
    if(p == NULL) return -1;
    return p->data;
}

int main() {
    scanf("%d", &N);

    int type, key;
    while(N--) {
        scanf("%d%d", &type, &key);
        if(type == 0) {
            insert(key);
        }
        else if(type == 1) {
            erase(key);
        }
        else if(type == 2) {
            printf("%d\n", lower_bound(key));
        }
        else {
            printf("invalid type\n");
            return -1;
        }
    }
    return 0;
}
