#include "kari.h"
#include "kari_stdlib.h"
#include <gc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

/* push */
K_FN(_push_2)
{
    kari_vec_push(((kari_array_t*)state)->items, argument);
    return (kari_value_t*)state;
}
K_FN(push)
{
    KASSERT(K_TYPE_OF(argument) == KARI_ARRAY, "Expected array");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_push_2), argument);
}

/* pop */
K_FN(pop)
{
    KASSERT(K_TYPE_OF(argument) == KARI_ARRAY, "Expected array");
    if(((kari_array_t*)argument)->items->count == 0) {
        return kari_nil();
    }
    return (kari_value_t*)kari_vec_pop(((kari_array_t*)argument)->items);
}

/* first */
K_FN(first)
{
    KASSERT(K_TYPE_OF(argument) == KARI_ARRAY, "Expected array");
    if(((kari_array_t*)argument)->items->count == 0) {
        return kari_nil();
    }
    return (kari_value_t*)((kari_array_t*)argument)->items->entries[0];
}

/* last */
K_FN(last)
{
    KASSERT(K_TYPE_OF(argument) == KARI_ARRAY, "Expected array");
    if(((kari_array_t*)argument)->items->count == 0) {
        return kari_nil();
    }
    return (kari_value_t*)((kari_array_t*)argument)->items->entries[((kari_array_t*)argument)->items->count - 1];
}

/* ind */
K_FN(_ind_2)
{
    size_t ind = 0;
    kari_array_t* ary = (kari_array_t*)state;
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    ind = (long)K_GET_NUMBER(argument);
    if(K_GET_NUMBER(argument) < 0) {
        ind = ary->items->count + (long)K_GET_NUMBER(argument);
    } else {
        ind = (size_t)K_GET_NUMBER(argument);
    }
    if(ind >= ary->items->count) {
        return kari_nil();
    }
    return (kari_value_t*)ary->items->entries[ind];
}

K_FN(ind)
{
    KASSERT(K_TYPE_OF(argument) == KARI_ARRAY, "Expected array");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_ind_2), argument);
}

/* remove */
K_FN(_remove_2)
{
    size_t ind = 0;
    kari_array_t* ary = (kari_array_t*)state;
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    ind = (long)K_GET_NUMBER(argument);
    if(K_GET_NUMBER(argument) < 0) {
        ind = ary->items->count + (long)K_GET_NUMBER(argument);
    } else {
        ind = (size_t)K_GET_NUMBER(argument);
    }
    if(ind > ary->items->count) {
        return kari_false();
    }
    kari_vec_remove(ary->items, ind);
    return kari_true();
}

K_FN(remove)
{
    KASSERT(K_TYPE_OF(argument) == KARI_ARRAY, "Expected array");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_ind_2), argument);
}

/* each */
K_FN(_each_2)
{
    size_t i;
    kari_array_t* ary = (kari_array_t*)state;
    KASSERT(K_IS_CALLABLE(K_TYPE_OF(argument)), "Expected function");
    for(i = 0; i < ary->items->count; i++) {
        if(kari_call(argument, (kari_value_t*)ary->items->entries[i], err) == NULL) {
            return NULL;
        }
    }
    return (kari_value_t*)ary;
}
K_FN(each)
{
    KASSERT(K_TYPE_OF(argument) == KARI_ARRAY, "Expected array");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_each_2), argument);
}

/* map */
K_FN(_map_2)
{
    size_t i;
    kari_array_t* ary = (kari_array_t*)state;
    kari_vec_t* new = new_kari_vec();
    kari_value_t* tmp;
    KASSERT(K_IS_CALLABLE(K_TYPE_OF(argument)), "Expected function");
    
    for(i = 0; i < ary->items->count; i++) {
        if((tmp = kari_call(argument, (kari_value_t*)ary->items->entries[i], err)) == NULL) {
            return NULL;
        }
        kari_vec_push(new, tmp);
    }
    
    ary = (kari_array_t*)GC_MALLOC(sizeof(kari_array_t));
    ary->base.type = KARI_ARRAY;
    ary->items = new;
    return (kari_value_t*)ary;
}

K_FN(map)
{
    KASSERT(K_TYPE_OF(argument) == KARI_ARRAY, "Expected array");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_map_2), argument);
}

/* foldl */
K_FN(_foldl_2)
{
    size_t i;
    kari_array_t* ary = (kari_array_t*)state;
    kari_value_t* acc = ary->items->count > 0 ? (kari_value_t*)ary->items->entries[0] : kari_nil();
    kari_value_t* fun = (kari_value_t*)argument;
    KASSERT(K_IS_CALLABLE(K_TYPE_OF(argument)), "Expected function");
    
    for(i = 1; i < ary->items->count; i++) {
        fun = kari_call(argument, acc, err);
        if(fun == NULL) {
            return NULL;
        }
        acc = kari_call(fun, (kari_value_t*)ary->items->entries[i], err);
        if(acc == NULL) {
            return NULL;
        }
    }
    
    return acc;
}

K_FN(foldl)
{
    KASSERT(K_TYPE_OF(argument) == KARI_ARRAY, "Expected array");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_foldl_2), argument);
}

/* reduce */
K_FN(reduce)
{
    K_ALIAS(foldl);
}