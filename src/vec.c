#include <stdlib.h>
#include <string.h>
#include <gc.h>
#include "vec.h"

kari_vec_t* new_kari_vec()
{
    kari_vec_t* v = GC_MALLOC(sizeof(kari_vec_t));
    v->entries = GC_MALLOC(4 * sizeof(void**));
    v->count = 0;
    v->capacity = 4;
    return v;
}

void kari_vec_push(kari_vec_t* v, void* obj)
{
    if(v->count == v->capacity) {
        v->capacity *= 2;
        v->entries = GC_REALLOC(v->entries, sizeof(void*) * v->capacity);
    }
    v->entries[v->count++] = obj;
}

void* kari_vec_pop(kari_vec_t* v)
{
    if(v->count == 0) {
        return NULL;
    }
    if(v->count == v->capacity / 2) {
        v->capacity /= 2;
        v->entries = GC_REALLOC(v->entries, sizeof(void*) * v->capacity);
    }
    return v->entries[--v->count];
}

void kari_vec_remove(kari_vec_t* v, size_t offset)
{
    if(offset >= v->count) {
        return;
    }
    if(offset + 1 < v->count) {
        memmove(&v->entries[offset], &v->entries[offset + 1], v->count - offset);
    }
    v->count--;
    if(v->count == v->capacity / 2) {
        v->capacity /= 2;
        v->entries = GC_REALLOC(v->entries, sizeof(void*) * v->capacity);
    }
}