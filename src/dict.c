#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <gc.h>
#include "dict.h"
#include "string.h"

static const unsigned int hash_seed = 0xdeadbeef;

static uint32_t murmur(char* key, size_t len)
{    
    /* MurmurHash2, by Austin Appleby (public domain) */
    
    /* 'm' and 'r' are mixing constants generated offline.
	 * They're not really 'magic', they just happen to work well. */
    const unsigned int m = 0x5bd1e995;
    const int r = 24;
    
    /* Initialize the hash to a 'random' value */
    unsigned int h = hash_seed ^ len;
        
    unsigned int k;
    
    /* Mix 4 bytes at a time into the hash */
    while(len >= 4) {
        k = *(unsigned int*)key;
        
        k *= m;
        k ^= k >> r;
        k *= m;
        
        h *= m;
        h ^= k;
        
        key += 4;
        len -= 4;
    }
    
    /* Handle the last few bytes of the input array */
    switch(len) {
        case 3: h ^= key[2] << 16;
        case 2: h ^= key[1] << 8;
        case 1: h ^= key[0];
                h *= m;
    }
    
    /* Do a few final mixes of the hash to ensure the last few
	 * bytes are well-incorporated. */

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

kari_dict_t* new_kari_dict()
{
    kari_dict_t* d = (kari_dict_t*)GC_MALLOC(sizeof(kari_dict_t));
    d->keys = new_kari_vec();
    return d;
}

bool kari_dict_set(kari_dict_t* dict, char* key, void* value)
{
    /*
    bool is_new = false;
    char* k = key;
    kari_dict_branch_t* branch = dict->branch;
    while(*k) {
        if(branch->branches[(int)*k] == NULL) {
            branch->branches[(int)*k] = (kari_dict_branch_t*)GC_MALLOC(sizeof(kari_dict_branch_t));
            dict->bytes_used += sizeof(kari_dict_branch_t);
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
    */
    kari_dict_entry_t* new_entry = NULL;
    size_t i;
    
    unsigned int hash = murmur(key, strlen(key));
    kari_vec_t* bucket = dict->buckets[hash & ((1 << BUCKET_INDEX_BITS) - 1)];
    if(bucket == NULL) {
        bucket = dict->buckets[hash & ((1 << BUCKET_INDEX_BITS) - 1)] = new_kari_vec();
        new_entry = (kari_dict_entry_t*)GC_MALLOC(sizeof(kari_dict_entry_t));
        new_entry->hash = hash;
        new_entry->key = key;
        new_entry->value = value;
        kari_vec_push(dict->keys, key);
        kari_vec_push(bucket, new_entry);
        return true;
    } else {
        for(i = 0; i < bucket->count; i++) {
            if(((kari_dict_entry_t*)bucket->entries[i])->hash == hash) {
                new_entry = (kari_dict_entry_t*)bucket->entries[i];
                new_entry->value = value;
                return false;
            }
        }
        new_entry = (kari_dict_entry_t*)GC_MALLOC(sizeof(kari_dict_entry_t));
        new_entry->hash = hash;
        new_entry->key = key;
        new_entry->value = value;
        kari_vec_push(dict->keys, key);
        kari_vec_push(bucket, new_entry);
        return true;
    }
}

bool kari_dict_exists(kari_dict_t* dict, char* key)
{
    kari_dict_entry_t* e = kari_dict_find(dict, key);
    if(e == NULL || e->value == NULL) {
        return false;
    } else {
        return true;
    }
}

void kari_dict_remove(kari_dict_t* dict, char* key)
{
    /*
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
    */
    size_t i;
    unsigned int hash = murmur(key, strlen(key));
    kari_vec_t* bucket = dict->buckets[hash & ((1 << BUCKET_INDEX_BITS) - 1)];
    if(bucket != NULL) {
        if(bucket->count == 1 && ((kari_dict_entry_t*)bucket->entries[0])->hash == hash) {
            dict->buckets[hash & ((1 << BUCKET_INDEX_BITS) - 1)] = NULL;
            return;
        }
        for(i = 0; i < bucket->count; i++) {
            if(((kari_dict_entry_t*)bucket->entries[i])->hash == hash) {
                kari_vec_remove(bucket, i);
                return;
            }
        }
        return;
    }
}

kari_dict_entry_t* kari_dict_find(kari_dict_t* dict, char* key)
{
    /*
    kari_dict_branch_t* branch = dict->branch;
    while(*key) {
        if(branch->branches[(int)*key] == NULL) {
            return NULL;
        }
        branch = branch->branches[(int)*key];
        key++;
    }
    return branch;
    */
    size_t i;
    unsigned int hash = murmur(key, strlen(key));
    kari_vec_t* bucket = dict->buckets[hash & ((1 << BUCKET_INDEX_BITS) - 1)];
    if(bucket == NULL) {
        return NULL;
    } else {
        for(i = 0; i < bucket->count; i++) {
            if(((kari_dict_entry_t*)bucket->entries[i])->hash == hash) {
                return (kari_dict_entry_t*)bucket->entries[i];
            }
        }
        return NULL;
    }
}

void* kari_dict_find_value(kari_dict_t* dict, char* key)
{
    kari_dict_entry_t* e = kari_dict_find(dict, key);
    if(e == NULL) {
        return NULL;
    } else {
        return e->value;
    }
}
