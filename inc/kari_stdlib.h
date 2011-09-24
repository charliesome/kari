#ifndef _KARI_STDLIB
#define _KARI_STDLIB

#include "kari.h"

#ifndef K_FN
    #define K_REF(name) kari_stdlib_##name
    #define K_FN(name) kari_value_t* K_REF(name)(kari_context_t* context, void* state, kari_value_t* argument, char** err)
    #define KASSERT(cond, msg) if(!(cond)) { *err = (msg); return NULL; }
    #define K_ALIAS(name) return K_REF(name)(context, state, argument, err)
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
K_FN(asin);
K_FN(acos);
K_FN(atan);
K_FN(exp);

/* system */
K_FN(inspect);
K_FN(quit);
K_FN(str);
K_FN(put);
K_FN(try);
K_FN(require);

/* control flow */
K_FN(if);
K_FN(while);
K_FN(for);

/* comparison */
K_FN(eq);
K_FN(lt);
K_FN(gt);
K_FN(lte);
K_FN(gte);

/* string */
K_FN(len);
K_FN(chr);
K_FN(ord);
K_FN(split);
K_FN(concat);
K_FN(index_of);
K_FN(interpolate);

/* array */
K_FN(push);
K_FN(pop);
K_FN(first);
K_FN(last);
K_FN(ind);
K_FN(remove);
K_FN(each);
K_FN(map);
K_FN(foldl);
K_FN(reduce); /* alias of foldl */

/* dict */
K_FN(dset);

#endif