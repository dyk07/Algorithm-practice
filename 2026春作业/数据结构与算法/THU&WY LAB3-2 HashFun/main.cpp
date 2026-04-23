#include <cstdio>
#include <cstdlib>
#include "hashtable.h"

int main(int argc, char** argv){
    /* 代码框架一共有两种运行方式，假设程序名为 `hashtable`：
     *
     * 1. 运行 `./hashtable <hashing> <collision>` 其中 <hashing> 和 <collision> 均为 0-2 的数字，
     *    则程序会根据相应的命令行参数确定哈希策略和冲突处理策略（见下面的 switch-case 语句）。
     *
     * 2. 运行 `./hashtable`，则程序会从标准输入的第一行读取两个数作为哈希策略和冲突处理策略。
     *
     * 你也可以修改下面的代码来支持你自己喜欢的输入方式。
     */
    int hashing_id = 0, collision_id = 0;
    hashing_strategy* hashing_type = NULL;
    collision_strategy* collision_type = NULL;
    if(argc == 3){
        // 从命令行的第一、第二个参数读取策略编号
        // 回忆：为什么不是从 0 编号？
        hashing_id = atoi(argv[1]);
        collision_id = atoi(argv[2]);
    }else if(argc == 1){
#ifndef _OJ_
        // 在本地执行且没有指定命令行选项时，默认从标准输入中读取策略选项。
        // 先读取策略编号
        scanf("%d%d", &hashing_id, &collision_id);
        // 随后按照题目中给定的输入格式读取输入数据
        // 可以通过重定向从文件中输入，如：
        // freopen("1.in", "r", stdin);
#else
        // 在 OJ 评测时，你可以通过修改这两个变量来指定使用的策略。
        // 在本地执行时，这一段代码将不会被编译。而在 OJ 上，上面的代码不会被编译。
        // 你可以在编译选项中加入 `-D_OJ_` 来在本地使用这种运行方式。
        hashing_id = 2, collision_id = 2;
#endif
    }else{
        printf("Invalid arguments!\n");
        return 1;
    }

    switch(hashing_id){
    case 0:
        hashing_type = new naive_hashing();
        break;
    case 1:
        hashing_type = new bad_hashing();
        break;
    case 2:
        hashing_type = new good_hashing();
        break;
    default:
        printf("Invalid hashing type!");
        return 1;
    }
    
    switch(collision_id){
    case 0:
        collision_type = new linear_probe();
        break;
    case 1:
        collision_type = new quadratic_probe();
        break;
    case 2:
        collision_type = new overflow_area();
        break;
    default:
        printf("Invalid collision strategy!");
        return 1;
    }

    int type;
    char buffer[1000];int data;
    // TODO: select your table size
    hashtable table(10000,hashing_type,collision_type);
    while(true){
        scanf("%d", &type);
        if(type == 0){
            scanf("%s", buffer);scanf("%d",&data);
            table.insert(hash_entry(buffer, data));
        }else if(type == 1){
            scanf("%s",buffer);
            printf("%d\n", table.query(buffer));
        }else break;
    }
    return 0;
}
