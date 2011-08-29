#include "kari.h"
#include "kari_stdlib.h"
#include <gc.h>
#include <string.h>
#include <stdio.h>

kari_context_t* kari_create_std_context()
{    
    kari_context_t* ctx = (kari_context_t*)GC_MALLOC(sizeof(kari_context_t));
    ctx->variables = new_kari_dict();
    ctx->parent = NULL;
    
    kari_load_stdlib(ctx);
    return ctx;
}

#define EXPOSE(fn) kari_dict_set(context->variables, #fn, kari_create_native_function(kari_stdlib_##fn, NULL))
void kari_load_stdlib(kari_context_t* context)
{    
    kari_dict_set(context->variables, "true", kari_true());
    kari_dict_set(context->variables, "false", kari_false());
    kari_dict_set(context->variables, "nil", kari_nil());
    
    #undef K_FN
    #undef _KARI_STDLIB
    #define K_FN(fn) EXPOSE(fn)
    #include "kari_stdlib.h"
}

kari_value_t* kari_eval(kari_context_t* ctx, char* source, char** err)
{
    kari_token_t** tokens;
    size_t tokens_count;
    *err = NULL;
    tokens_count = kari_parse(source, &tokens, err);
    if(*err) {
        return NULL;
    }
    return kari_execute(ctx, tokens, tokens_count, err);
}

kari_value_t* kari_call(kari_value_t* function, kari_value_t* argument, char** err)
{
    kari_context_t* ctx;
    switch(function->type) {
        case KARI_FUNCTION:
            ctx = (kari_context_t*)GC_MALLOC(sizeof(kari_context_t));
            ctx->variables = new_kari_dict();
            ctx->parent = ((kari_function_t*)function)->parent;
            
            kari_dict_set(ctx->variables, ((kari_function_t*)function)->argument, argument);
            return kari_execute(ctx, ((kari_function_t*)function)->tokens, ((kari_function_t*)function)->token_count, err);
        case KARI_NATIVE_FUNCTION:
            *err = NULL;
            return ((kari_native_function_t*)function)->call(((kari_native_function_t*)function)->state, argument, err);
        default:
            *err = (char*)GC_MALLOC(36 + strlen(kari_string_for_value_type_t(function->type)));
            sprintf(*err, "Value of type %s called as function", kari_string_for_value_type_t(function->type));
            return NULL;
    }
}