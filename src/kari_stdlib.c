#include "kari.h"
#include "kari_stdlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#define KASSERT(cond, msg) if(!(cond)) { *err = (msg); return NULL; }

/* add */
K_FN(_add_2)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(((kari_number_t*)argument)->number + ((kari_number_t*)state)->number);
}
K_FN(add)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_native_function(K_REF(_add_2), argument);
}

/* sub */
K_FN(_sub_2)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(((kari_number_t*)state)->number - ((kari_number_t*)argument)->number);
}
K_FN(sub)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_native_function(K_REF(_sub_2), argument);
}

/* mul */
K_FN(_mul_2)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(((kari_number_t*)argument)->number * ((kari_number_t*)state)->number);
}
K_FN(mul)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_native_function(K_REF(_mul_2), argument);
}

/* div */
K_FN(_div_2)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(((kari_number_t*)state)->number / ((kari_number_t*)argument)->number);
}
K_FN(div)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_native_function(K_REF(_div_2), argument);
}

/* mod */
K_FN(_mod_2)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(fmod(((kari_number_t*)state)->number, ((kari_number_t*)argument)->number));
}
K_FN(mod)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_native_function(K_REF(_mod_2), argument);
}

/* odd */
K_FN(odd)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return kari_boolean(fabs((fmod(((kari_number_t*)argument)->number, 2)) - 1) < DBL_EPSILON);
}

/* even */
K_FN(even)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return kari_boolean(fabs(fmod(((kari_number_t*)argument)->number, 2)) < DBL_EPSILON);
}

/* floor */
K_FN(floor)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(floor(((kari_number_t*)argument)->number));
}

/* ceil */
K_FN(ceil)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(ceil(((kari_number_t*)argument)->number));
}

/* sin */
K_FN(sin)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(sin(((kari_number_t*)argument)->number));
}

/* cos */
K_FN(cos)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(cos(((kari_number_t*)argument)->number));
}

/* tan */
K_FN(tan)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(tan(((kari_number_t*)argument)->number));
}

/* asin */
K_FN(asin)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(asin(((kari_number_t*)argument)->number));
}

/* acos */
K_FN(acos)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(acos(((kari_number_t*)argument)->number));
}

/* atan */
K_FN(atan)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(atan(((kari_number_t*)argument)->number));
}

/* exp */
K_FN(exp)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(exp(((kari_number_t*)argument)->number));
}

/* quit */
K_FN(quit)
{
    if(argument->type != KARI_NUMBER) {
        exit(0);
    }
    exit((int)((kari_number_t*)argument)->number);
}

/* inspect */
K_FN(inspect)
{
    return (kari_value_t*)kari_create_string(kari_inspect(argument));
}

/* if */
K_FN(_if_3)
{
    KASSERT(argument->type == KARI_FUNCTION || argument->type == KARI_NATIVE_FUNCTION, "Expected function");
    if(((kari_value_t*)state)->type == KARI_FALSE) {
        return kari_call(argument, kari_nil(), err);
    } else {
        /* if the condition was true, we'll get a function to call here,
           not a boolean in the state var */
        return kari_call(state, kari_nil(), err);
    }
}
K_FN(_if_2)
{
    KASSERT(argument->type == KARI_FUNCTION || argument->type == KARI_NATIVE_FUNCTION, "Expected function");
    /* we don't call the true function right now even if the bool was true,
       however if the bool was true, we'll replace the state passed to the
       next function with the true function for it to call. */
    return (kari_value_t*)kari_create_native_function(K_REF(_if_3),
        (((kari_value_t*)state)->type == KARI_TRUE) ? argument : state);
}
K_FN(if)
{
    KASSERT(argument->type == KARI_TRUE || argument->type == KARI_FALSE, "Expected true/false value");
    return (kari_value_t*)kari_create_native_function(K_REF(_if_2), argument);
}