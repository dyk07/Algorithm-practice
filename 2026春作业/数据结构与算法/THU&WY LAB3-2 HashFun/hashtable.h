#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <cstring>
#include <cstdio>
const int TABLE_SIZE = 499883; // = 124970 * 4 + 3
struct hash_entry{
    char* my_string;
    int my_data; 
    hash_entry(){
        my_string = NULL;my_data = 0;
    }
    hash_entry(char* str, int data):my_data(data){
        my_string = new char[strlen(str)+1];
        strcpy(my_string,str);
    }
};
struct hashing_strategy{
    virtual int operator()(char* str, int N)=0;
};
struct naive_hashing: public hashing_strategy{
    int operator()(char* str, int N) override;
};
struct bad_hashing: public hashing_strategy{
    int operator()(char* str, int N) override;
};
struct good_hashing: public hashing_strategy{
    int operator()(char* str, int N) override;
private:
    // TODO: 如果需要，可以定义一些成员函数/变量
};
struct collision_strategy {
    virtual void init()=0;// pure virtual function
    virtual int operator()(hash_entry* Table, int table_size, int last_choice)=0;
    virtual int bucket_size(int table_size) {
        // 对于线性试探和双向平方试探，其桶大小就是整个哈希表的大小
        return table_size;
    }
};
struct linear_probe: public collision_strategy{
    void init() override;
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};
struct quadratic_probe: public collision_strategy{
    void init() override;
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
private:
    int probe_step = 0;// 双向平方试探的步数
};
struct overflow_area: public collision_strategy{
    void init() override;
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
    // 对于公共溢出区策略，桶区域只是整个哈希表数组的一部分
    int bucket_size(int table_size) override;
private:
    int main_bucket_size = 0;   // 主桶区域大小
    int probe_step = 0;         // 当前试探步数（偏移量）
};
struct hashtable {
    hash_entry* Table;
    int table_size;
    int bucket_size;
    hashing_strategy* my_hashing; // 如果改为hashing_strategy my_hashing, 即不是用指针作为hashtable的成员, 而是让hashing_strategy结构体直接作为hashtable的成员. 会发生什么bug?
    collision_strategy* my_collision;
    hashtable(int size, hashing_strategy* hashing, collision_strategy* collision)
        :table_size(size),my_hashing(hashing),my_collision(collision)
    {
        Table = new hash_entry[table_size];
        bucket_size = collision->bucket_size(table_size);
    }
    bool insert(hash_entry entry){ 
       int last_choice = (*my_hashing)(entry.my_string,bucket_size);
       my_collision->init();
       int tries = 0;
       while(Table[last_choice].my_string!=NULL){ // loop infinitely? return false when no more space?
           if (++tries >= table_size * 2) {
               return false; // 哈希表已满
           }
           last_choice = (*my_collision)(Table, table_size, last_choice);
       }
       Table[last_choice] = entry;
       return true;
    }
    int query(char* query_string){
        int last_choice = (*my_hashing)(query_string,bucket_size);
        my_collision->init();
        int tries = 0;
        while(Table[last_choice].my_string!=NULL && 
            strcmp(Table[last_choice].my_string, query_string)!=0){ // 未处理的情况: 哈希表已满?
            if (++tries >= table_size * 2) {
                return -1; // 哈希表已满或未找到
            }
            last_choice = (*my_collision)(Table, table_size, last_choice);
        }
        if(Table[last_choice].my_string == NULL){
            return -1;
        }
        else {
            return Table[last_choice].my_data;
        }
    }
};
#endif // HASHTABLE_H
