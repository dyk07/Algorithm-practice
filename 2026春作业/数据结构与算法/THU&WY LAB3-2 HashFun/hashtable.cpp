#include "hashtable.h"
int naive_hashing::operator()(char* str, int N){
    if(str == NULL) return 0;
    else return (str[0]+N)%N;
}

int bad_hashing::operator()(char *str, int N){
    if (str == nullptr) return 0;
    int hash = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        hash ^= static_cast<int>(str[i]);
    }
    return hash % N;
}

int good_hashing::operator()(char *str, int N){
    if (str == nullptr) return 0;
    unsigned long long hash = 0;
    const unsigned long long base = 131;
    for (int i = 0; str[i] != '\0'; ++i) {
        hash = hash * base + static_cast<unsigned char>(str[i]);
    }
    return hash % N;
}

int linear_probe::operator()(hash_entry* Table, int table_size, int last_choice){
    return (last_choice + 1) % table_size;
}
void linear_probe::init(){
    return;// do nothing
}

int quadratic_probe::operator()(hash_entry *Table, int table_size, int last_choice){
    probe_step++;
    return (last_choice + probe_step * probe_step) % table_size;
}
void quadratic_probe::init(){
    probe_step = 0;
    return;
}

int overflow_area::operator()(hash_entry *Table, int table_size, int last_choice){
    probe_step++;
    int pos = (main_bucket_size + probe_step) % table_size; 
    if (pos >= table_size) {
        pos = table_size - 1;
    }
    return pos;
}
void overflow_area::init() {
    probe_step = 0;
}
int overflow_area::bucket_size(int table_size) {
    main_bucket_size = table_size * 4 / 5;
    return main_bucket_size;
}
