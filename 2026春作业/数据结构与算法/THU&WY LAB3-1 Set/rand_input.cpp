#include <set>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

#define max(x, y) ((x) < (y) ? (y) : (x))

std::set<int> my_set;

void insert(int key) {
    my_set.insert(key);
}

bool erase(int key) {
    return my_set.erase(key);
}

int main(){
    srand(time(0));
    int N = 20;
    int M = 8;
    printf("%d\n", N);
    
    int type, key;
    while(N--) {
        type = rand() % 3;
        if(type == 0) {
            do {
                // generate element between [1, max(size + 1, M)] to ensure no duplication can exist
                key = rand() % max(my_set.size() + 1, M) + 1;
            } while(my_set.count(key));
            insert(key);
        }
        else if(type == 1) {
            key = rand() % M + 1;
            erase(key);
        }
        else {
            key = rand() % (M + 1) + 1;
        }
        printf("%d %d\n", type, key);
    }
    return 0;
}