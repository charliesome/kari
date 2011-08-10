#include "kari.h"
#include "kari_stdlib.h"
#include <gc.h>
#include <string.h>
#include <stdio.h>

kari_context_t* kari_create_std_context()
{
    kari_context_t* ctx = GC_MALLOC(sizeof(kari_context_t));
    ctx->variables = new_kari_dict(kari_dict_string_hash);
    kari_load_stdlib(ctx);
    return ctx;
}

#define EXPOSE(fn) kari_dict_add(context->variables, #fn, kari_create_native_function(kari_stdlib_##fn, NULL))
void kari_load_stdlib(kari_context_t* context)
{
    #undef K_FN
    #undef _KARI_STDLIB
    #define K_FN(fn) EXPOSE(fn)
    #include "kari_stdlib.h"
}

kari_value_t* kari_call(kari_value_t* function, kari_value_t* argument, char** err)
{
    kari_context_t* ctx;
    switch(function->type) {
        case KARI_FUNCTION:
            ctx = GC_MALLOC(sizeof(kari_context_t));
            ctx->variables = new_kari_dict(kari_dict_string_hash);
            kari_dict_add(ctx->variables, ((kari_function_t*)function)->argument, argument);
            return kari_execute(ctx, ((kari_function_t*)function)->tokens, ((kari_function_t*)function)->token_count, err);
        case KARI_NATIVE_FUNCTION:
            *err = NULL;
            return ((kari_native_function_t*)function)->call(((kari_native_function_t*)function)->state, argument, err);
        default:
            *err = GC_MALLOC(36 + strlen(kari_string_for_value_type_t(function->type)));
            sprintf(*err, "Value of type %s called as function", kari_string_for_value_type_t(function->type));
            return NULL;
    }
}