#include <gc.h>
#include <string.h>
#include <stdio.h>
#include "kari.h"
#include "kari_stdlib.h"

static kari_value_type_t k_nil = KARI_NIL;
kari_value_t* kari_nil()
{
    return (kari_value_t*)&k_nil;
}

static kari_value_type_t k_true = KARI_TRUE;
kari_value_t* kari_true()
{
    return (kari_value_t*)&k_true;
}

static kari_value_type_t k_false = KARI_FALSE;
kari_value_t* kari_false()
{
    return (kari_value_t*)&k_false;
}

kari_value_t* kari_boolean(bool boolean)
{
    return (boolean ? kari_true : kari_false)();
}

char* kari_string_for_token_type_t(kari_token_type_t t)
{
    switch(t) {
        case KARI_TOK_NIL: return "nil";
        case KARI_TOK_STRING: return "string";
        case KARI_TOK_NUMBER: return "number";
        case KARI_TOK_FUNCTION: return "function";
        case KARI_TOK_ASSIGN_TO_IDENTIFIER: return "assign to identifier";
        case KARI_TOK_ARRAY: return "array";
        default: return "(unknown type)";
    }
}

char* kari_string_for_value_type_t(kari_value_type_t t)
{
    switch(t) {
        case KARI_NIL: return "NIL";
        case KARI_STRING: return "STRING";
        case KARI_NUMBER: return "NUMBER";
        case KARI_NATIVE_FUNCTION: return "NATIVE_FUNCTION";
        case KARI_FUNCTION: return "FUNCTION";
        case KARI_TRUE: return "TRUE";
        case KARI_FALSE: return "FALSE";
        case KARI_ARRAY: return "ARRAY";
        default: return "(unknown type)";
    }
}

char* kari_inspect(kari_value_t* value)
{
    size_t s_len, s_cap, i, tmp_size;
    char *s, *tmp_s;
    
    switch(value->type) {
        case KARI_NIL:
            return "(nil)";
        case KARI_STRING:
            s_len = 0;
            s_cap = 16;
            s = (char*)GC_MALLOC(s_cap);
            s[s_len++] = '"';
            for(i = 0; i < ((kari_string_t*)value)->len; i++) {
                if(s_len + 2 >= s_cap) {
                    s_cap *= 2;
                    s = (char*)GC_REALLOC(s, s_cap);
                }
                if(((kari_string_t*)value)->str[i] == '"' || ((kari_string_t*)value)->str[i] == '\\') {
                    s[s_len++] = '\\';
                }
                s[s_len++] = ((kari_string_t*)value)->str[i];
            }
            if(s_len + 2 >= s_cap) {
                s = (char*)GC_REALLOC(s, s_cap + 2);
            }
            s[s_len++] = '"';
            s[s_len++] = 0;
            return s;
        case KARI_NUMBER:
            s = (char*)GC_MALLOC(32);
            sprintf(s, "%f", ((kari_number_t*)value)->number);
            for(i = strlen(s) - 1; s[i] == '0' || s[i] == '.'; i--) {
                if(s[i] == '.') {
                    s[i] = 0;
                    break;
                } else {
                    s[i] = 0;
                }
            }
            if(s[0] == 0) {
                s[0] = '0';
            }
            return s;
        case KARI_NATIVE_FUNCTION:
            s = (char*)GC_MALLOC(64);
            sprintf(s, "(native-function:%p)", (void*)(size_t)((kari_native_function_t*)value)->call);
            return s;
        case KARI_FUNCTION:
            return "(function)";
        case KARI_TRUE:
            return "(true)";
        case KARI_FALSE:
            return "(false)";
        case KARI_ARRAY:
            s_len = 0;
            s_cap = 16;
            s = (char*)GC_MALLOC(s_cap);
            s[s_len++] = '[';
            for(i = 0; i < ((kari_array_t*)value)->items->count; i++) {
                if(i != 0) {
                    if(s_len + 2 > s_cap) {
                        s_cap *= 2;
                        s = (char*)GC_REALLOC(s, s_cap);
                    }
                    s[s_len++] = ',';
                    s[s_len++] = ' ';
                }
                if(((kari_value_t*)((kari_array_t*)value)->items->entries[i])->type == KARI_ARRAY) {
                    tmp_s = "(array)";
                } else if(((kari_value_t*)((kari_array_t*)value)->items->entries[i])->type == KARI_ARRAY) {
                    tmp_s = "(dict)";
                } else {
                    tmp_s = kari_inspect((kari_value_t*)((kari_array_t*)value)->items->entries[i]);
                }
                tmp_size = strlen(tmp_s);
                while(s_len + tmp_size > s_cap) {
                    s_cap *= 2;
                    s = (char*)GC_REALLOC(s, s_cap);
                }
                memcpy(s + s_len, tmp_s, tmp_size);
                s_len += tmp_size;
            }
            if(s_len + 2 > s_cap) {
                s_cap *= 2;
                s = (char*)GC_REALLOC(s, s_cap);
            }
            s[s_len++] = ']';
            s[s_len++] = 0;
            return s;
        case KARI_DICT:
            s_len = 0;
            s_cap = 16;
            s = (char*)GC_MALLOC(s_cap);
            s[s_len++] = '{';
            s[s_len++] = ' ';
            for(i = 0; i < ((kari_dict_val_t*)value)->items->keys->count; i++) {
                if(i != 0) {
                    if(s_len + 5 > s_cap) {
                        s_cap *= 2;
                        s = (char*)GC_REALLOC(s, s_cap);
                    }
                    s[s_len++] = ',';
                    s[s_len++] = ' ';
                }
                tmp_size = strlen((char*)((kari_dict_val_t*)value)->items->keys->entries[i]);
                while(s_len + tmp_size + 5 > s_cap) {
                    s_cap *= 2;
                    s = (char*)GC_REALLOC(s, s_cap);
                }
                memcpy(s + s_len, (char*)((kari_dict_val_t*)value)->items->keys->entries[i], tmp_size);
                s_len += tmp_size;
                s[s_len++] = ':';
                s[s_len++] = ' ';
                if(((kari_value_t*)kari_dict_find_value(((kari_dict_val_t*)value)->items,
                    (char*)((kari_dict_val_t*)value)->items->keys->entries[i]))->type == KARI_ARRAY) {
                    tmp_s = "(array)";
                } else if(((kari_value_t*)kari_dict_find_value(((kari_dict_val_t*)value)->items,
                    (char*)((kari_dict_val_t*)value)->items->keys->entries[i]))->type == KARI_DICT) {
                    tmp_s = "(dict)";
                } else {
                    tmp_s = kari_inspect((kari_value_t*)kari_dict_find_value(((kari_dict_val_t*)value)->items,
                        (char*)((kari_dict_val_t*)value)->items->keys->entries[i]));   
                }
                tmp_size = strlen(tmp_s);
                while(s_len + tmp_size + 3 > s_cap) {
                    s_cap *= 2;
                    s = (char*)GC_REALLOC(s, s_cap);
                }
                memcpy(s + s_len, tmp_s, tmp_size);
                s_len += tmp_size;
            }
            s[s_len++] = ' ';
            s[s_len++] = '}';
            s[s_len] = 0;
            return s;
        default:
            return "(unknown type)";
    }
}

char* kari_str(kari_value_t* value)
{
    switch(value->type) {
        case KARI_NIL:
            return "";
        case KARI_STRING:
            return ((kari_string_t*)value)->str;
        default:
            return kari_inspect(value);
    }
}

kari_number_t* kari_create_number(double number)
{
    kari_number_t* n = (kari_number_t*)GC_MALLOC(sizeof(kari_number_t));
    n->base.type = KARI_NUMBER;
    n->number = number;
    return n;
}

kari_string_t* kari_create_string(char* str)
{
    kari_string_t* s = (kari_string_t*)GC_MALLOC(sizeof(kari_string_t));
    s->base.type = KARI_STRING;
    s->str = str;
    s->len = kari_utf8_strlen(str);
    return s;
}

kari_native_function_t* kari_create_native_function(kari_context_t* context, kari_nfn_t fn, void* state)
{
    kari_native_function_t* f = (kari_native_function_t*)GC_MALLOC(sizeof(kari_native_function_t));
    f->base.type = KARI_NATIVE_FUNCTION;
    f->state = state;
    f->context = context;
    f->call = fn;
    return f;
}

kari_value_t* kari_var_get(kari_context_t* ctx, char* name)
{
    while(ctx) {
        if(kari_dict_exists(ctx->variables, name)) {
            return (kari_value_t*)kari_dict_find_value(ctx->variables, name);
        }
        ctx = ctx->parent;
    }
    return NULL;
}

kari_array_t* kari_create_array()
{
    kari_array_t* arr = (kari_array_t*)GC_MALLOC(sizeof(kari_array_t));
    arr->base.type = KARI_ARRAY;
    arr->items = new_kari_vec();
    return arr;
}

size_t kari_utf8_strlen(char* s)
{
    size_t len = 0;
    while(*s) {
        if((*s & 0xc0) != 0x80) {
            len++;
        }
        s++;
    }
    return len;
}