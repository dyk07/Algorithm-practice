#include <cstdio>
#include <list>
#include <iostream>
using namespace std;


struct a{
    int val;
    int cnt;
};


list<int> q;
list<a> y[2];
int d = 0;


void recz(){
    y[1-d].clear();
    if( !y[d].empty() ){
        auto it = q.begin();
        int tt = y[d].front().cnt - 1;
        while( tt-- ){
            int c = 1;
            while( !y[1-d].empty() && (*it >= y[1-d].back().val) ){
                c += y[1-d].back().cnt;
                y[1-d].pop_back();
            }
            a tmp = {*it, c};
            y[1-d].push_back(tmp);
            if( tt > 0 ) it++;
        }
    }
}


void dequeue(){
    q.pop_front();
    if ( --y[d].front().cnt < 1 ){
        y[d].pop_front();
        recz();
    } else {
        if( --y[1-d].front().cnt < 1 ){
            y[1-d].pop_front();
        }
    }
}


int m(){
    if(q.size() < 1){
        return -1;
    }
    return y[d].front().val;
}


void enqueue(int x){
    q.push_back(x);
    if(!y[d].empty() && x >= m()){
        d = 1 - d;
        a tmp = {x, q.size()};
        y[d].clear();
        y[d].push_back(tmp);
        return;
    }
    int c = 1;
    while ( !y[d].empty() && (x >= y[d].back().val) ){
        c += y[d].back().cnt;
        y[d].pop_back();
    }
    a tmp = {x, c};
    y[d].push_back(tmp);
    if(y[d].size() == 1){
        recz();
    }
}


int secm(){
    if(q.size() < 2){
        return -1;
    }
    
    int tmp = y[1-d].empty() ? -1 : y[1-d].front().val;
    if(y[d].size() < 2){
        return tmp;
    }

    auto it = next(y[d].begin());
    return max(it->val, tmp);
}


int main(){

    int T;
    int xx, yy;
    scanf("%d", &T);
    while( T-- ){
        scanf("%d", &xx);
        if(xx == 2){
            dequeue();
        }else{
            scanf("%d", &yy);
            enqueue(yy);
        }
        printf("%d %d\n", m(), secm());
    }



    return 0;
}