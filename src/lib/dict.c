#include "kari.h"
#include "kari_stdlib.h"
#include <gc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

/* dset */
struct dset_state {
    kari_dict_val_t* dict;
    char* key;
};

K_FN(_dset_3)
{
    struct dset_state* st = (struct dset_state*)state;
    kari_dict_set(st->dict->items, st->key, argument);
    return argument;
}

K_FN(_dset_2)
{
    struct dset_state* st = (struct dset_state*)state;
    KASSERT(argument->type == KARI_STRING, "Expected string");
    st->key = ((kari_string_t*)argument)->str;
    return (kari_value_t*)kari_create_native_function(K_REF(_dset_3), st);
}

K_FN(dset)
{
    struct dset_state* st = (struct dset_state*)GC_MALLOC(sizeof(struct dset_state));
    KASSERT(argument->type == KARI_DICT, "Expected dictionary");
    st->dict = (kari_dict_val_t*)argument;
    return (kari_value_t*)kari_create_native_function(K_REF(_dset_2), st);
}