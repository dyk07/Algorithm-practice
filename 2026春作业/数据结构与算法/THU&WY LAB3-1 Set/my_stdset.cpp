#include <set>
#include <stdio.h>

std::set<int> my_set;
int N;

void insert(int key) {
    my_set.insert(key);
}

bool erase(int key) {
    return my_set.erase(key);
}

int lower_bound(int key) {
    const std::set<int>::iterator it = my_set.lower_bound(key);
    if(it == my_set.end()) {
        return -1;
    }
    else {
        return *it;
    }
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
