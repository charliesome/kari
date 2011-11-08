#include "kari.h"
#include "kari_stdlib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>


/* eq */
K_FN(_eq_2)
{
    kari_value_t* oth = (kari_value_t*)state;
    if(K_TYPE_OF(argument) != oth->type) {
        return kari_false();
    }
    switch(K_TYPE_OF(argument)) {
        case KARI_NIL:
        case KARI_TRUE:
        case KARI_FALSE:
            return kari_true();
        case KARI_NUMBER:
            return kari_boolean(fabs(K_GET_NUMBER(argument) - K_GET_NUMBER(oth)) < DBL_EPSILON);
        case KARI_STRING:
            if(((kari_string_t*)argument)->len != ((kari_string_t*)oth)->len) {
                return kari_false();
            }
            return kari_boolean(memcmp(((kari_string_t*)argument)->str, ((kari_string_t*)oth)->str, ((kari_string_t*)argument)->len) == 0);
        case KARI_FUNCTION:
            return kari_boolean(memcmp(argument, oth, sizeof(kari_function_t)) == 0);
        case KARI_NATIVE_FUNCTION:
            return kari_boolean(memcmp(argument, oth, sizeof(kari_native_function_t)) == 0);
        case KARI_ARRAY:
            /* @TODO do linear comparison of two arrays */
            break;
        case KARI_DATA:
            return kari_boolean(((kari_data_t*)oth)->ptr == ((kari_data_t*)argument)->ptr);
        case KARI_DICT:
            /* @TODO */
            break;
    }
    return kari_false();
}

K_FN(eq)
{
    return (kari_value_t*)kari_create_native_function(context, K_REF(_eq_2), argument);
}

/* lt */
K_FN(_lt_2)
{
    kari_value_t* oth = (kari_value_t*)state;
    if(K_TYPE_OF(argument) != oth->type) {
        return kari_false();
    }
    switch(K_TYPE_OF(argument)) {
        case KARI_NUMBER:
            return kari_boolean((K_GET_NUMBER(argument) - K_GET_NUMBER(oth)) > DBL_EPSILON);
        default:
            return kari_false();
    }
}

K_FN(lt)
{
    return (kari_value_t*)kari_create_native_function(context, K_REF(_lt_2), argument);
}

/* lte */
K_FN(_lte_2)
{
    kari_value_t* oth = (kari_value_t*)state;
    if(K_TYPE_OF(argument) != oth->type) {
        return kari_false();
    }
    switch(K_TYPE_OF(argument)) {
        case KARI_NUMBER:
            return kari_boolean((K_GET_NUMBER(argument) - K_GET_NUMBER(oth)) > -DBL_EPSILON);
        default:
            return kari_false();
    }
}

K_FN(lte)
{
    return (kari_value_t*)kari_create_native_function(context, K_REF(_lte_2), argument);
}

/* gt */
K_FN(_gt_2)
{
    kari_value_t* oth = (kari_value_t*)state;
    if(K_TYPE_OF(argument) != oth->type) {
        return kari_false();
    }
    switch(K_TYPE_OF(argument)) {
        case KARI_NUMBER:
            return kari_boolean((K_GET_NUMBER(oth) - K_GET_NUMBER(argument)) > DBL_EPSILON);
        default:
            return kari_false();
    }
}

K_FN(gt)
{
    return (kari_value_t*)kari_create_native_function(context, K_REF(_gt_2), argument);
}


/* gte */
K_FN(_gte_2)
{
    kari_value_t* oth = (kari_value_t*)state;
    if(K_TYPE_OF(argument) != oth->type) {
        return kari_false();
    }
    switch(K_TYPE_OF(argument)) {
        case KARI_NUMBER:
            return kari_boolean((K_GET_NUMBER(oth) - K_GET_NUMBER(argument)) > -DBL_EPSILON);
        default:
            return kari_false();
    }
}

K_FN(gte)
{
    return (kari_value_t*)kari_create_native_function(context, K_REF(_gte_2), argument);
}