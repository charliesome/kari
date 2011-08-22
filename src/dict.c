#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <gc.h>
#include "dict.h"
#include "string.h"

kari_dict_t* new_kari_dict()
{
    kari_dict_t* d = (kari_dict_t*)GC_MALLOC(sizeof(kari_dict_t));
    d->keys = new_kari_vec();
    d->branch = (kari_dict_branch_t*)GC_MALLOC(sizeof(kari_dict_branch_t));
    return d;
}

bool kari_dict_set(kari_dict_t* dict, char* key, void* value)
{
    bool is_new = false;
    char* k = key;
    kari_dict_branch_t* branch = dict->branch;
    while(*k) {
        if(branch->branches[(int)*k] == NULL) {
            branch->branches[(int)*k] = (kari_dict_branch_t*)GC_MALLOC(sizeof(kari_dict_branch_t));
            is_new = true;
        }
        branch = branch->branches[(int)*k];
        k++;
    }
    branch->value = value;
    if(is_new) {
        kari_vec_push(dict->keys, key);
    }
    return is_new;
}

bool kari_dict_exists(kari_dict_t* dict, char* key)
{
    kari_dict_branch_t* e = kari_dict_find(dict, key);
    if(e == NULL || e->value == NULL) {
        return false;
    } else {
        return true;
    }
}

void kari_dict_remove(kari_dict_t* dict, char* key)
{
    size_t i;
    kari_dict_branch_t* b = kari_dict_find(dict, key);
    if(b) {
        for(i = 0; i < dict->keys->count; i++) {
            if(strcmp(key, (char*)dict->keys->entries[i]) == 0) {
                kari_vec_remove(dict->keys, i);
                break;
            }
        }
        b->value = NULL;
    }
}

kari_dict_branch_t* kari_dict_find(kari_dict_t* dict, char* key)
{
    kari_dict_branch_t* branch = dict->branch;
    while(*key) {
        if(branch->branches[(int)*key] == NULL) {
            return NULL;
        }
        branch = branch->branches[(int)*key];
        key++;
    }
    return branch;
}

void* kari_dict_find_value(kari_dict_t* dict, char* key)
{
    kari_dict_branch_t* e = kari_dict_find(dict, key);
    if(e == NULL) {
        return NULL;
    } else {
        return e->value;
    }
}
