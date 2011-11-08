#include "kari.h"
#include "kari_stdlib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

/* add */
K_FN(_add_2)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(K_GET_NUMBER(argument) + K_GET_NUMBER(state));
}
K_FN(add)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_add_2), argument);
}

/* sub */
K_FN(_sub_2)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(K_GET_NUMBER(argument) - K_GET_NUMBER(state));
}
K_FN(sub)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_sub_2), argument);
}

/* mul */
K_FN(_mul_2)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(K_GET_NUMBER(argument) * K_GET_NUMBER(state));
}
K_FN(mul)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_mul_2), argument);
}

/* div */
K_FN(_div_2)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(K_GET_NUMBER(argument) / K_GET_NUMBER(state));
}
K_FN(div)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_div_2), argument);
}

/* mod */
K_FN(_mod_2)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(fmod(K_GET_NUMBER(state), K_GET_NUMBER(argument)));
}
K_FN(mod)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_mod_2), argument);
}

/* odd */
K_FN(odd)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return kari_boolean(fabs((fmod(K_GET_NUMBER(argument), 2)) - 1) < DBL_EPSILON);
}

/* even */
K_FN(even)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return kari_boolean(fabs(fmod(K_GET_NUMBER(argument), 2)) < DBL_EPSILON);
}

/* floor */
K_FN(floor)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(floor(K_GET_NUMBER(argument)));
}

/* ceil */
K_FN(ceil)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(ceil(K_GET_NUMBER(argument)));
}

/* sin */
K_FN(sin)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(sin(K_GET_NUMBER(argument)));
}

/* cos */
K_FN(cos)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(cos(K_GET_NUMBER(argument)));
}

/* tan */
K_FN(tan)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(tan(K_GET_NUMBER(argument)));
}

/* asin */
K_FN(asin)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(asin(K_GET_NUMBER(argument)));
}

/* acos */
K_FN(acos)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(acos(K_GET_NUMBER(argument)));
}

/* atan */
K_FN(atan)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(atan(K_GET_NUMBER(argument)));
}

/* exp */
K_FN(exp)
{
    KASSERT(K_TYPE_OF(argument) == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(exp(K_GET_NUMBER(argument)));
}