#include <cstdio>
#include <cstdlib>

int main() {
    // 自动创建文件夹，防止路径不存在报错
    system("if not exist input mkdir input");
    system("if not exist output mkdir output");

    while (1) {
        // Windows 下直接写文件名即可，不要加 ./
        // 建议带上 .exe 后缀更稳妥
        system("rand_input.exe > input\\rand.in");
        system("my_stdset.exe < input\\rand.in > output\\my_stdset.out");
        system("my_skiplist.exe < input\\rand.in > output\\my_skiplist.out");
        system("my_bst.exe < input\\rand.in > output\\my_bst.out");
        system("my_splay_single.exe < input\\rand.in > output\\my_splay_single.out");
        system("my_splay_double.exe < input\\rand.in > output\\my_splay_double.out");

        // 使用 Windows 自带的 fc (File Compare) 替代 diff
        // > nul 是为了不让对比的差异文字刷屏
        if (system("fc output\\my_stdset.out output\\my_skiplist.out > nul") != 0) {
            printf("my_skiplist: different output!\n");
            break;
        }
        if (system("fc output\\my_stdset.out output\\my_bst.out > nul") != 0) {
            printf("my_bst: different output!\n");
            break;
        }
        if (system("fc output\\my_stdset.out output\\my_splay_single.out > nul") != 0) {
            printf("my_splay_single: different output!\n");
            break;
        }
        if (system("fc output\\my_stdset.out output\\my_splay_double.out > nul") != 0) {
            printf("my_splay_double: different output!\n");
            break;
        }
        puts("OK!");
    }
    return 0;
}