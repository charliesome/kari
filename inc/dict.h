#ifndef _DICT_H
#define _DICT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "vec.h"

typedef struct kari_dict_branch {
    void* value;
    struct kari_dict_branch* branches[256];
} kari_dict_branch_t;

typedef struct kari_dict {
    kari_vec_t* keys;
    kari_dict_branch_t* branch;
} kari_dict_t;

kari_dict_t* new_kari_dict();

bool kari_dict_set(kari_dict_t* dict, char* key, void* value);
bool kari_dict_exists(kari_dict_t* dict, char* key);
void kari_dict_remove(kari_dict_t* dict, char* key);
kari_dict_branch_t* kari_dict_find(kari_dict_t* dict, char* key);
void* kari_dict_find_value(kari_dict_t* dict, char* key);

#endif