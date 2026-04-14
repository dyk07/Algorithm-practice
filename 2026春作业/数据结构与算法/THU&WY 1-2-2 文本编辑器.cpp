/*#include <cstdio>
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
}*/




//Gemini Version
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

// =============== Fast I/O 模板 ===============
namespace IO {
    const int BUF_SIZE = 1 << 23;
    char buf[BUF_SIZE], *p1 = buf, *p2 = buf;
    inline char gc() {
        return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, BUF_SIZE, stdin), p1 == p2) ? EOF : *p1++;
    }
    inline char next_char() {
        char c = gc();
        // 忽略空格、换行等不可见字符
        while (c <= 32) {
            if (c == EOF) return EOF;
            c = gc();
        }
        return c;
    }
    // 读取初始字符串
    inline void read_str(char* s, int &len) {
        char c = gc();
        while (c <= 32) c = gc(); 
        len = 0;
        // 题意：ascii在 33~126 之间为可见字符
        while (c >= 33 && c <= 126) {
            s[++len] = c;
            c = gc();
        }
    }
    char outbuf[BUF_SIZE];
    int outlen = 0;
    inline void pc(char c) {
        outbuf[outlen++] = c;
        if (outlen == BUF_SIZE) { fwrite(outbuf, 1, outlen, stdout); outlen = 0; }
    }
    inline void flush() {
        if (outlen) fwrite(outbuf, 1, outlen, stdout);
    }
}

// =============== FHQ-Treap ===============
const int MAXN = 7500005; // 足够容纳同时存在的最大字符数量

// 使用平行数组代替结构体，提升缓存命中率，节省内存
int ls[MAXN], rs[MAXN], sz[MAXN];
unsigned int rnd[MAXN];
char val[MAXN];
bool rev[MAXN];

int trash[MAXN], trash_top; // 内存垃圾桶
int root, tot;

// 伪随机数生成器（比 rand() 快很多）
unsigned int seed = 19260817;
inline unsigned int fast_rand() {
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

// 分配新节点
inline int new_node(char c) {
    int u = trash_top ? trash[trash_top--] : ++tot;
    ls[u] = rs[u] = rev[u] = 0;
    sz[u] = 1;
    val[u] = c;
    rnd[u] = fast_rand();
    return u;
}

// 废弃节点回收
inline void del_node(int u) {
    trash[++trash_top] = u;
}

inline void pushup(int u) {
    sz[u] = sz[ls[u]] + sz[rs[u]] + 1;
}

inline void pushdown(int u) {
    if (rev[u]) {
        swap(ls[u], rs[u]);
        if (ls[u]) rev[ls[u]] ^= 1;
        if (rs[u]) rev[rs[u]] ^= 1;
        rev[u] = 0; // 标记下传后清空
    }
}

// 按照大小分裂：前 k 个元素分到 x，剩下的分到 y
void split(int u, int k, int &x, int &y) {
    if (!u) { x = y = 0; return; }
    pushdown(u);
    if (sz[ls[u]] < k) {
        x = u;
        split(rs[u], k - sz[ls[u]] - 1, rs[u], y);
    } else {
        y = u;
        split(ls[u], k, x, ls[u]);
    }
    pushup(u);
}

// 合并：要求 x 中的元素在相对位置上全部处于 y 的左侧
int merge(int x, int y) {
    if (!x || !y) return x | y;
    pushdown(x); pushdown(y);
    if (rnd[x] < rnd[y]) {
        rs[x] = merge(rs[x], y);
        pushup(x);
        return x;
    } else {
        ls[y] = merge(x, ls[y]);
        pushup(y);
        return y;
    }
}

// 笛卡尔树建树：O(N) 级别将初始文本构建为平衡树
char init_str[3200005];
int stk[MAXN];

int build_cartesian(int n) {
    int top = 0;
    for (int i = 1; i <= n; i++) {
        int u = new_node(init_str[i]);
        int last = 0;
        // 维护小根堆性质
        while (top && rnd[stk[top]] > rnd[u]) {
            pushup(stk[top]);
            last = stk[top--];
        }
        if (top) rs[stk[top]] = u;
        ls[u] = last;
        stk[++top] = u;
    }
    while (top) pushup(stk[top--]);
    return stk[1];
}

// 中序遍历输出文本
void print_tree(int u) {
    if (!u) return;
    pushdown(u);
    print_tree(ls[u]);
    IO::pc(val[u]);
    print_tree(rs[u]);
}

// =============== 主函数与核心逻辑 ===============
int main() {
    int init_len;
    IO::read_str(init_str, init_len);
    
    // 初始化平衡树
    root = build_cartesian(init_len);
    
    // 初始化光标相对位置：posL 在最左侧，posR 在最右侧
    int posL = 0, posR = init_len;
    
    int N = 0;
    char c = IO::next_char();
    while (c >= '0' && c <= '9') {
        N = N * 10 + c - '0';
        c = IO::next_char();
    }
    
    // 如果刚刚读取数字多读了一个字符，把它放进去，但在当前的逻辑中由于操作符是字母，
    // 我们手动在下面解析
    
    for (int i = 0; i < N; i++) {
        char op1;
        if (i == 0 && (c < '0' || c > '9')) op1 = c; 
        else op1 = IO::next_char();
        
        if (op1 == '<') {
            char op2 = IO::next_char();
            if (op2 == 'L') {
                if (posL > 0) { posL--; IO::pc('T'); IO::pc('\n'); }
                else { IO::pc('F'); IO::pc('\n'); }
            } else {
                if (posR > 0) { posR--; IO::pc('T'); IO::pc('\n'); }
                else { IO::pc('F'); IO::pc('\n'); }
            }
        } else if (op1 == '>') {
            char op2 = IO::next_char();
            if (op2 == 'L') {
                if (posL < sz[root]) { posL++; IO::pc('T'); IO::pc('\n'); }
                else { IO::pc('F'); IO::pc('\n'); }
            } else {
                if (posR < sz[root]) { posR++; IO::pc('T'); IO::pc('\n'); }
                else { IO::pc('F'); IO::pc('\n'); }
            }
        } else if (op1 == 'I') {
            char op2 = IO::next_char();
            char ch = IO::next_char();
            int x, y;
            if (op2 == 'L') {
                split(root, posL, x, y);
                root = merge(merge(x, new_node(ch)), y);
                int old = posL; posL++;
                // 联动：如果右光标在插入处或插入处右边，必须跟着右移以保持相对位置不变
                if (posR >= old) posR++; 
            } else {
                split(root, posR, x, y);
                root = merge(merge(x, new_node(ch)), y);
                int old = posR; posR++;
                // 同理联动左光标
                if (posL >= old) posL++;
            }
            IO::pc('T'); IO::pc('\n');
        } else if (op1 == 'D') {
            char op2 = IO::next_char();
            if (op2 == 'L') {
                if (posL == sz[root]) { IO::pc('F'); IO::pc('\n'); }
                else {
                    int x, y, y1, y2;
                    split(root, posL, x, y);
                    split(y, 1, y1, y2); // y1 就是被删除的那一个字符
                    del_node(y1);        // 扔进垃圾桶复用
                    root = merge(x, y2);
                    if (posR > posL) posR--;
                    IO::pc('T'); IO::pc('\n');
                }
            } else {
                if (posR == sz[root]) { IO::pc('F'); IO::pc('\n'); }
                else {
                    int x, y, y1, y2;
                    split(root, posR, x, y);
                    split(y, 1, y1, y2);
                    del_node(y1);
                    root = merge(x, y2);
                    if (posL > posR) posL--;
                    IO::pc('T'); IO::pc('\n');
                }
            }
        } else if (op1 == 'R') {
            if (posL >= posR) { IO::pc('F'); IO::pc('\n'); }
            else {
                int x, y, y1, y2;
                split(root, posL, x, y);
                split(y, posR - posL, y1, y2); // y1 提取出两个光标之间的所有字符
                
                rev[y1] ^= 1; // 关键：O(1) 打上翻转懒标记
                
                root = merge(x, merge(y1, y2));
                IO::pc('T'); IO::pc('\n');
            }
        } else if (op1 == 'S') {
            print_tree(root);
            IO::pc('\n');
        }
    }
    
    IO::flush(); // 清除并输出缓冲区剩余字符
    return 0;
}