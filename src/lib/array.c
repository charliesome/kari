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
    KASSERT(argument->type == KARI_ARRAY, "Expected array");
    return (kari_value_t*)kari_create_native_function(K_REF(_push_2), argument);
}

/* pop */
K_FN(pop)
{
    KASSERT(argument->type == KARI_ARRAY, "Expected array");
    if(((kari_array_t*)argument)->items->count == 0) {
        return kari_nil();
    }
    return (kari_value_t*)kari_vec_pop(((kari_array_t*)argument)->items);
}

/* first */
K_FN(first)
{
    KASSERT(argument->type == KARI_ARRAY, "Expected array");
    if(((kari_array_t*)argument)->items->count == 0) {
        return kari_nil();
    }
    return (kari_value_t*)((kari_array_t*)argument)->items->entries[0];
}

/* last */
K_FN(last)
{
    KASSERT(argument->type == KARI_ARRAY, "Expected array");
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
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    ind = (long)((kari_number_t*)argument)->number;
    if(((kari_number_t*)argument)->number < 0) {
        ind = ary->items->count + (long)((kari_number_t*)argument)->number;
    } else {
        ind = (size_t)((kari_number_t*)argument)->number;
    }
    if(ind > ary->items->count) {
        return kari_nil();
    }
    return (kari_value_t*)ary->items->entries[ind];
}

K_FN(ind)
{
    KASSERT(argument->type == KARI_ARRAY, "Expected array");
    return (kari_value_t*)kari_create_native_function(K_REF(_ind_2), argument);
}

/* remove */
K_FN(_remove_2)
{
    size_t ind = 0;
    kari_array_t* ary = (kari_array_t*)state;
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    ind = (long)((kari_number_t*)argument)->number;
    if(((kari_number_t*)argument)->number < 0) {
        ind = ary->items->count + (long)((kari_number_t*)argument)->number;
    } else {
        ind = (size_t)((kari_number_t*)argument)->number;
    }
    if(ind > ary->items->count) {
        return kari_false();
    }
    kari_vec_remove(ary->items, ind);
    return kari_true();
}

K_FN(remove)
{
    KASSERT(argument->type == KARI_ARRAY, "Expected array");
    return (kari_value_t*)kari_create_native_function(K_REF(_ind_2), argument);
}