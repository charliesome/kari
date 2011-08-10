#ifndef _DICT_H
#define _DICT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct kari_dict_entry {
    uint32_t hash;
    void* key;
    void* value;
} kari_dict_entry_t;

typedef struct kari_dict {
    uint32_t(*hash_fn)(void*);
    kari_dict_entry_t* entries;
    size_t entries_count;
    size_t entries_capacity;
} kari_dict_t;

uint32_t kari_dict_string_hash(void*);

kari_dict_t* new_kari_dict(uint32_t(*hash_fn)(void*));
bool kari_dict_add(kari_dict_t* dict, void* key, void* value);
bool kari_dict_exists(kari_dict_t* dict, void* key);
void kari_dict_remove(kari_dict_t* dict, void* key);
kari_dict_entry_t* kari_dict_find(kari_dict_t* dict, void* key);
void* kari_dict_find_value(kari_dict_t* dict, void* key);

#endif