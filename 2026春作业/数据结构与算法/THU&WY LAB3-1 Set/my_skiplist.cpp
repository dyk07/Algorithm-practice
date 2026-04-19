#include <stdio.h>
#include <stdlib.h>
#include "lib/skiplist/skiplist.h"

Skiplist<int, int> skiplist;
int N;

void insert(int key) {
    skiplist.put(key, key);
}

bool erase(int key) {
    return skiplist.remove(key);
}

int lower_bound(int key) {
    QNodePosi<Entry<int, int>> p = skiplist.lower_bound(key);
    if(p==NULL)return -1;
    return p->entry.value;
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
