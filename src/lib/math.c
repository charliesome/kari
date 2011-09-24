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
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_number(((kari_number_t*)argument)->number + ((kari_number_t*)state)->number);
}
K_FN(add)
{
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_add_2), argument);
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
    return (kari_value_t*)kari_create_native_function(context, K_REF(_sub_2), argument);
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
    return (kari_value_t*)kari_create_native_function(context, K_REF(_mul_2), argument);
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
    return (kari_value_t*)kari_create_native_function(context, K_REF(_div_2), argument);
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
    return (kari_value_t*)kari_create_native_function(context, K_REF(_mod_2), argument);
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