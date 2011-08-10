#ifndef _KARI_STDLIB
#define _KARI_STDLIB

#include "kari.h"

typedef kari_value_t*(*kari_nfn_t)(void*,kari_value_t*,char**);
#define K_FN(name) kari_value_t* kari_stdlib_##name(void* state, kari_value_t* argument, char** err)

K_FN(hello);

#endif