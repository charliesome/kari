#include "kari.h"
#include "kari_stdlib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

/* if */
K_FN(_if_3)
{
    KASSERT(K_IS_CALLABLE(K_TYPE_OF(argument)), "Expected function");
    if(((kari_value_t*)state)->type == KARI_FALSE) {
        return kari_call(argument, kari_nil(), err);
    } else {
        /* if the condition was true, we'll get a function to call here,
           not a boolean in the state var */
        return kari_call((kari_value_t*)state, kari_nil(), err);
    }
}
K_FN(_if_2)
{
    KASSERT(K_IS_CALLABLE(K_TYPE_OF(argument)), "Expected function");
    /* we don't call the true function right now even if the bool was true,
       however if the bool was true, we'll replace the state passed to the
       next function with the true function for it to call. */
    return (kari_value_t*)kari_create_native_function(context, K_REF(_if_3),
        (((kari_value_t*)state)->type == KARI_TRUE) ? argument : state);
}
K_FN(if)
{
    KASSERT(K_IS_BOOLEAN(K_TYPE_OF(argument)), "Expected true/false value");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_if_2), argument);
}

/* while */
K_FN(_while_2)
{
    kari_value_t* tmp;
    KASSERT(K_IS_CALLABLE(K_TYPE_OF(argument)), "Expected function");
    while(true) {
        tmp = kari_call((kari_value_t*)state, kari_nil(), err);
        if(tmp == NULL) {
            return NULL;
        }
        if(tmp->type != KARI_TRUE) {
            break;
        }
        tmp = kari_call(argument, kari_nil(), err);
        if(tmp == NULL) {
            return NULL;
        }
    }
    return kari_nil();
}

K_FN(while)
{
    KASSERT(K_IS_CALLABLE(K_TYPE_OF(argument)), "Expected function");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_while_2), argument);
}

/* for */
struct for_state {
    int from;
    int to;
};

K_FN(_for_3)
{
    struct for_state* st = (struct for_state*)state;
    int from = st->from, to = st->to + 1;
    KASSERT(K_IS_CALLABLE(K_TYPE_OF(argument)), "Expected function");
    for(; from < to; from++) {
        if(!kari_call(argument, (kari_value_t*)kari_create_number(from), err)) {
            return NULL;
        }
    }
    return kari_nil();
}

K_FN(_for_2)
{
    struct for_state* st = (struct for_state*)state;
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    st->to = (int)K_GET_NUMBER(argument);
    return (kari_value_t*)kari_create_native_function(context, K_REF(_for_3), st);
}

K_FN(for)
{
    struct for_state* st = (struct for_state*)GC_MALLOC(sizeof(struct for_state));
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    st->from = (int)K_GET_NUMBER(argument);
    return (kari_value_t*)kari_create_native_function(context, K_REF(_for_2), st);
}

