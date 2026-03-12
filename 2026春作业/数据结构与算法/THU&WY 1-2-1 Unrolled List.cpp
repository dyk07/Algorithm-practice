#include <cstdio>
using namespace std;


struct node{
    int data[705];
    int cnt = 0;
    node* pre = nullptr;
    node* next = nullptr;
};


node head;
node* h = &head;


int find(node* n, int pos){
    if(pos >= 0 && pos < n->cnt){
        return n->data[pos];
    }
    if(n->next == nullptr){
        return -1;
    }
    return find(n->next, pos - n->cnt);
}


void insert(node* n, int pos, int m){
    if(pos > n->cnt || pos < 0){
        if(n->next == nullptr){
            insert(n, n->cnt, m);
            return;
        }
        insert(n->next, pos - n->cnt, m);
        return;
    }

    if(n->cnt < 700){
        for(int i = n->cnt - 1; i >= pos; i--){
            n->data[i + 1] = n->data[i];
        }
        n->data[pos] = m;
        n->cnt++;
        return;
    }
    node* nn = new node;
    nn->pre = n;
    if(n->next != nullptr){
        nn->next = n->next;
        nn->next->pre = nn;
    }
    n->next = nn;
    n->cnt = 350;
    n->next->cnt = 350;
    for(int i = 350; i < 700; i++){
        nn->data[i - 350] = n->data[i];
    }
    insert(n, pos, m);
    return;
}


void remove(node*n, int pos){
    if(pos < n->cnt && pos >= 0){
        for(int i = pos; i < n->cnt - 1; i++){
            n->data[i] = n->data[i + 1];
        }
        n->cnt--;
        if(n->cnt == 0){
            if(n == h){
                if(h->next != nullptr){
                    h = h->next;
                    h->pre = nullptr;
                }
            }else{
                if(n->next != nullptr){
                    n->pre->next = n->next;
                    n->next->pre = n->pre;
                }else{
                    n->pre->next = nullptr;
                }
            }
        }
        return;
    }
    if(n->next == nullptr){
        return;
    }
    remove(n->next, pos - n->cnt);
    return;
}


int main(){
    int n, a, b, c;
    scanf("%d", &n);
    while(n--){
        scanf("%d%d%d", &a, &b, &c);
        switch(a){
            case 1: insert(h, b, c); break;
            case 2: remove(h, b); break;
            case 3: printf("%d\n", find(h, b)); break;
        }
    }


    return 0;
}