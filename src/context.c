#include "kari.h"
#include "kari_stdlib.h"
#include <gc.h>
#include <string.h>
#include <stdio.h>

kari_context_t* kari_create_std_context()
{    
    kari_context_t* ctx = (kari_context_t*)GC_MALLOC(sizeof(kari_context_t));
    ctx->variables = st_init_numtable();
    ctx->parent = NULL;
    
    kari_load_stdlib(ctx);
    return ctx;
}

#define EXPOSE(fn) st_insert(context->variables, kari_identifier_uniqid(#fn), (st_data_t)kari_create_native_function(context, kari_stdlib_##fn, NULL))
void kari_load_stdlib(kari_context_t* context)
{
    st_insert(context->variables, kari_identifier_uniqid("true"),    (st_data_t)kari_true());
    st_insert(context->variables, kari_identifier_uniqid("false"),   (st_data_t)kari_false());
    st_insert(context->variables, kari_identifier_uniqid("nil"),     (st_data_t)kari_nil());
    st_insert(context->variables, kari_identifier_uniqid("$inc"),    (st_data_t)kari_create_array());
    
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
            ctx->variables = st_init_numtable();
            ctx->parent = ((kari_function_t*)function)->parent;
            
            st_insert(ctx->variables, kari_identifier_uniqid(((kari_function_t*)function)->argument), (st_data_t)argument);
            return kari_execute(ctx, ((kari_function_t*)function)->tokens, ((kari_function_t*)function)->token_count, err);
        case KARI_NATIVE_FUNCTION:
            *err = NULL;
            return ((kari_native_function_t*)function)->call(((kari_native_function_t*)function)->context, ((kari_native_function_t*)function)->state, argument, err);
        default:
            *err = (char*)GC_MALLOC(36 + strlen(kari_string_for_value_type_t(function->type)));
            sprintf(*err, "Value of type %s called as function", kari_string_for_value_type_t(function->type));
            return NULL;
    }
}