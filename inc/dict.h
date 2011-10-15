#ifndef _DICT_H
#define _DICT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "vec.h"

#define BUCKET_INDEX_BITS 16

typedef struct kari_dict_entry {
    uint32_t hash;
    char* key;
    void* value;
} kari_dict_entry_t;

typedef struct kari_dict {
    kari_vec_t* keys;
    kari_vec_t* buckets[1 << BUCKET_INDEX_BITS];
} kari_dict_t;

kari_dict_t* new_kari_dict();

bool kari_dict_set(kari_dict_t* dict, char* key, void* value);
bool kari_dict_exists(kari_dict_t* dict, char* key);
void kari_dict_remove(kari_dict_t* dict, char* key);
kari_dict_entry_t* kari_dict_find(kari_dict_t* dict, char* key);
void* kari_dict_find_value(kari_dict_t* dict, char* key);

#endif