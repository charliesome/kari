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