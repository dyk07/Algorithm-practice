#include <cstdio>
#include <list>
using namespace std;

const int T = 7200005;
char s[T];
list<char> l;
auto le = l.begin(), ri = l.end();
bool lyy = 0, ryy = 0;


void yy(bool a, bool b){
    if(a){
        if(lyy ^ b) le--;
        else le++;
    }else{
        if(ryy ^ b) ri--;
        else ri++;
    }
}


char mov(char a, char b){
    if(a == '<'){
        if(b == 'L'){
            if(le == l.begin()){
                return 'F';
            }else{
                le--;
                return 'T';
            }
        }else{
            if(ri == l.begin()){
                return 'F';
            }else{
                ri--;
                return 'T';
            }
        }
    }else{
        if(b == 'L'){
            if(le == l.end()){
                return 'F';
            }else{
                le++;
                return 'T';
            }
        }else{
            if(ri == l.end()){
                return 'F';
            }else{
                ri++;
                return 'T';
            }
        }
    }
}


char ins(char b, char c){
    if(b == 'L'){
        l.insert(le, c);
    }else{
        l.insert(ri, c);
    }
    return 'T';
}


char del(char b){
    if(le == ri){
        if(le == l.end()){
            return 'F';
        }else{
            ri = le = l.erase(le);
            return 'T';
        }
    }

    if(b == 'L'){
        if(le == l.end()){
            return 'F';
        }else{
            le = l.erase(le);
            return 'T';
        }
    }else{
        if(ri == l.end()){
            return 'F';
        }else{
            ri = l.erase(ri);
            return 'T';
        }
    }
}


char rev(){
    auto i = le;
    if(i == l.end()) return 'F';
    i++;
    while(i != ri && i != l.end()) i++;
    if(i == l.end() && ri != l.end()) return 'F';

    
    auto k = le; k--;
    auto p = ri; p++;
    while(le != k){
        l.insert(k, *le);
        k--;
        le = l.erase(le);
    }
    return 'T';
}


int main(){
    scanf("%s", s);
    for(int i = 0; s[i] != '\0'; i++){
        l.push_back(s[i]);
    }
    le = l.begin(), ri = l.end();

    
 
    int N;
    scanf(" %d", &N);
    
    char a, b, c;
    for(int i = 0; i < N; i++){
        scanf(" %c", &a);
        if(a == '<' or a == '>'){
            scanf(" %c", &b);
            printf("%c\n", mov(a, b));
        }else if(a == 'I'){
            scanf(" %c", &b);
            scanf(" %c", &c);
            printf("%c\n", ins(b, c));
        }else if(a == 'D'){
            scanf(" %c", &b);
            printf("%c\n", del(b));
        }else if(a == 'R'){
            printf("%c\n", rev());
        }else if(a == 'S'){
            for(auto j = l.begin(); j != l.end(); j++){
                printf("%c", *j);
            }
            printf("\n");
        }
    }


    return 0;
}