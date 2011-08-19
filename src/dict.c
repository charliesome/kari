#include <stdlib.h>
#include <string.h>
#include <gc.h>
#include "dict.h"
#include "string.h"

kari_dict_t* new_kari_dict()
{
    kari_dict_t* d = (kari_dict_t*)GC_MALLOC(sizeof(kari_dict_t));
    d->branch = (kari_dict_branch_t*)GC_MALLOC(sizeof(kari_dict_branch_t));
    return d;
}

bool kari_dict_set(kari_dict_t* dict, char* key, void* value)
{
    kari_dict_branch_t* branch = dict->branch;
    while(*key) {
        if(branch->branches[(int)*key] == NULL) {
            branch->branches[(int)*key] = (kari_dict_branch_t*)GC_MALLOC(sizeof(kari_dict_branch_t));
        }
        branch = branch->branches[(int)*key];
        key++;
    }
    branch->value = value;
    return true;
}

bool kari_dict_exists(kari_dict_t* dict, char* key)
{
    kari_dict_branch_t* e = kari_dict_find(dict, key);
    if(e == NULL) {
        return false;
    } else {
        return true;
    }
}

void kari_dict_remove(kari_dict_t* dict, char* key)
{
    kari_dict_branch_t* b = kari_dict_find(dict, key);
    b->value = NULL;
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
