#ifndef _VEC_H
#define _VEC_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct kari_vec {
    void** entries;
    size_t count;
    size_t capacity;
} kari_vec_t;

kari_vec_t* new_kari_vec();
void kari_vec_push(kari_vec_t* v, void* obj);
void* kari_vec_pop(kari_vec_t* v);
void kari_vec_remove(kari_vec_t* v, size_t offset);

#endif