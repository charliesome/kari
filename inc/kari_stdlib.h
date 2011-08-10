#ifndef _KARI_STDLIB
#define _KARI_STDLIB

#include "kari.h"

#ifndef K_FN
    #define K_REF(name) kari_stdlib_##name
    #define K_FN(name) kari_value_t* K_REF(name)(void* state, kari_value_t* argument, char** err)
#endif

/* math */
K_FN(add);
K_FN(sub);
K_FN(mul);
K_FN(div);
K_FN(mod);
K_FN(odd);
K_FN(even);
K_FN(floor);
K_FN(ceil);
K_FN(sin);
K_FN(cos);
K_FN(tan);
/*K_FN(asin);*/
/*K_FN(acos);*/
/*K_FN(atan);*/
/*K_FN(exp);*/

/* system */
K_FN(quit);

#endif