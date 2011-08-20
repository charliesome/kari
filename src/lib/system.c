#include "kari.h"
#include "kari_stdlib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

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

/* str */
K_FN(str)
{
    return (kari_value_t*)kari_create_string(kari_str(argument));
}

/* put */
K_FN(put)
{
    printf("%s\n", kari_str(argument));
    return kari_nil();
}

/* try */
K_FN(try)
{    
    char* e = NULL;
    kari_value_t* val = NULL;
    KASSERT(K_IS_CALLABLE(argument->type), "Expected function");
    
    val = kari_call(argument, kari_nil(), &e);
    if(e) {
        return (kari_value_t*)kari_create_string(e);
    } else {
        return val;
    }
}