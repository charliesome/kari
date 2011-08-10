#include <stdlib.h>
#include <string.h>
#include <gc.h>
#include "dict.h"
#include "string.h"

uint32_t kari_dict_string_hash(void* str)
{
    char* s = (char*)str;
    uint32_t h = 0;
    while(*s) {
        h *= 17;
        h += *s++;
    }
    return h;
}

static int kari_dict_entries_cmp(const void* a, const void* b) {
    return ((const kari_dict_entry_t*)b)->hash - ((const kari_dict_entry_t*)a)->hash;
}

kari_dict_t* new_kari_dict(uint32_t(*hash_fn)(void*))
{
    kari_dict_t* d = (kari_dict_t*)GC_MALLOC(sizeof(kari_dict_t));
    d->entries_capacity = 4;
    d->entries = (kari_dict_entry_t*)GC_MALLOC(sizeof(kari_dict_entry_t) * d->entries_capacity);
    d->hash_fn = hash_fn;
    return d;
}

bool kari_dict_add(kari_dict_t* dict, void* key, void* value)
{
    if(dict->entries_count == dict->entries_capacity) {
        dict->entries_capacity *= 2;
        dict->entries = GC_REALLOC(dict->entries, dict->entries_capacity);
    }
    if(kari_dict_exists(dict, key)) {
        return false;
    }
    dict->entries[dict->entries_count].hash = dict->hash_fn(key);
    dict->entries[dict->entries_count].key = key;
    dict->entries[dict->entries_count].value = value;
    dict->entries_count++;
    qsort(dict->entries, dict->entries_count, sizeof(kari_dict_entry_t), kari_dict_entries_cmp);
    return true;
}

bool kari_dict_exists(kari_dict_t* dict, void* key)
{
    kari_dict_entry_t* e = kari_dict_find(dict, key);
    if(e == NULL) {
        return false;
    } else {
        return true;
    }
}

void kari_dict_remove(kari_dict_t* dict, void* key)
{
    kari_dict_entry_t* e = kari_dict_find(dict, key);
    if(e == NULL) {
        return;
    }
    memmove(e, e + 1, dict->entries_count - ((size_t)e - (size_t)dict->entries) - 1);
    dict->entries_count--;
    dict->entries[dict->entries_count].hash = 0;
    dict->entries[dict->entries_count].key = NULL;
    dict->entries[dict->entries_count].value = NULL;
}

kari_dict_entry_t* kari_dict_find(kari_dict_t* dict, void* key)
{
    kari_dict_entry_t search;
    search.hash = dict->hash_fn(key);
    return (kari_dict_entry_t*)bsearch(&search, dict->entries, dict->entries_count, sizeof(kari_dict_entry_t), kari_dict_entries_cmp);
}

void* kari_dict_find_value(kari_dict_t* dict, void* key)
{
    kari_dict_entry_t* e = kari_dict_find(dict, key);
    if(e == NULL) {
        return NULL;
    } else {
        return e->value;
    }
}
