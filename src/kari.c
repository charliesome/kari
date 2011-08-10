#include <gc.h>
#include <string.h>
#include <stdio.h>
#include "kari.h"
#include "kari_stdlib.h"

static kari_value_t k_nil;
kari_value_t* kari_nil()
{
    k_nil.type = KARI_NIL;
    return &k_nil;
}

char* kari_string_for_value_type_t(kari_value_type_t t)
{
    switch(t) {
        case KARI_NIL: return "NIL";
        case KARI_STRING: return "STRING";
        case KARI_NUMBER: return "NUMBER";
        case KARI_NATIVE_FUNCTION: return "NATIVE_FUNCTION";
        case KARI_FUNCTION: return "FUNCTION";
        default: return "(unknown type)";
    }
}

char* kari_inspect(kari_value_t* value)
{
    size_t s_len, s_cap, i;
    char* s;
    
    switch(value->type) {
        case KARI_NIL:
            return "(nil)";
        case KARI_STRING:
            s_len = 0;
            s_cap = 16;
            s = GC_MALLOC(s_cap);
            s[s_len++] = '"';
            for(i = 0; i < ((kari_string_t*)value)->len; i++) {
                if(s_len + 2 >= s_cap) {
                    s_cap *= 2;
                    s = GC_REALLOC(s, s_cap);
                }
                if(((kari_string_t*)value)->str[i] == '"') {
                    s[s_len++] = '\\';
                }
                s[s_len++] = ((kari_string_t*)value)->str[i];
            }
            if(s_len + 2 >= s_cap) {
                s = GC_REALLOC(s, s_cap + 2);
            }
            s[s_len++] = '"';
            s[s_len++] = 0;
            return s;
        case KARI_NUMBER:
            s = GC_MALLOC(32);
            sprintf(s, "%f", ((kari_number_t*)value)->number);
            return s;
        case KARI_NATIVE_FUNCTION:
            s = GC_MALLOC(64);
            sprintf(s, "(native-function:%p)", (void*)(size_t)((kari_native_function_t*)value)->call);
            return s;
        case KARI_FUNCTION:
            return "(function)";
        default:
            return "(unknown type)";
    }
}