#include "kari.h"
#include "kari_stdlib.h"
#include <gc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

K_FN(len)
{
    /* overloaded here for array as well */
    
    switch(argument->type) {
        case KARI_STRING:
            return (kari_value_t*)kari_create_number(((kari_string_t*)argument)->len);
        case KARI_ARRAY:
            return (kari_value_t*)kari_create_number(((kari_array_t*)argument)->items->count);
        default:
            *err = "Expected string or array";
            return NULL;
    }
}

K_FN(chr)
{   
    char* str = (char*)GC_MALLOC(5);
    size_t codepoint;
    KASSERT(argument->type == KARI_NUMBER, "Expected number");
    codepoint = (size_t)((kari_number_t*)argument)->number;
    
    if(codepoint < 0x7f) {
        str[0] = (char)codepoint;
    } else if(codepoint < 0x07ff) {
        str[1] = (char)(((codepoint >> 0) & 0x3f) | 0x80);
        str[0] = (char)(((codepoint >> 6) & 0x1f) | 0xc0);
    } else if(codepoint < 0xffff) {
        str[2] = (char)(((codepoint >> 0) & 0x3f) | 0x80);
        str[1] = (char)(((codepoint >> 6) & 0x3f) | 0x80);
        str[0] = (char)(((codepoint >> 12) & 0x0f) | 0xe0);
    } else if(codepoint < 0x10ffff) {
        str[3] = (char)(((codepoint >> 0) & 0x3f) | 0x80);
        str[2] = (char)(((codepoint >> 6) & 0x3f) | 0x80);
        str[1] = (char)(((codepoint >> 12) & 0x3f) | 0x80);
        str[0] = (char)(((codepoint >> 18) & 0x07) | 0xf0);
    }
    
    return (kari_value_t*)kari_create_string(str);
}

K_FN(ord)
{
    kari_string_t* str;
    KASSERT(argument->type == KARI_STRING, "Expected string");
    str = (kari_string_t*)argument;
    if(str->len != 1) {
        *err = "String must have only one character";
        return NULL;
    }
    if(str->str[0] & 0x80) {
        int ord = 0;
        if((str->str[0] & 0xe0) == 0xc0) {
            /* two byte utf8 char */
            ord = (str->str[1] & 0x3f) | ((str->str[0] & 0x1f) << 6);
        } else if((str->str[0] & 0xf0) == 0xe0) {
            /* three byte utf8 char */
            ord = (str->str[2] & 0x3f) | ((str->str[1] & 0x3f) << 6) | ((str->str[0] & 0x0f) << 12);
        } else if((str->str[0] & 0xf8) == 0xf0) {
            /* four byte utf8 char */
            ord = (str->str[3] & 0x3f) | ((str->str[2] & 0x3f) << 6) | ((str->str[1] & 0x3f) << 12) | ((str->str[0] & 0x07) << 18);
        }
        return (kari_value_t*)kari_create_number(ord);
    } else {
        return (kari_value_t*)kari_create_number(str->str[0]);
    }
}

/* split */
K_FN(_split_2)
{
    kari_array_t* ary;
    kari_string_t* str = (kari_string_t*)state;
    kari_string_t* delim = (kari_string_t*)argument;
    size_t delim_bytes;
    char *strptr, *tmp, *delimptr, *endptr;
    KASSERT(argument->type == KARI_STRING, "Expected string");
    
    delim_bytes = strlen(delim->str);
    
    ary = (kari_array_t*)GC_MALLOC(sizeof(kari_array_t));
    ary->base.type = KARI_ARRAY;
    ary->items = new_kari_vec();
    
    strptr = str->str;
    endptr = strptr + strlen(strptr);
    do {
        delimptr = strstr(strptr, delim->str);
        tmp = (char*)GC_MALLOC((delimptr ? delimptr : endptr) - strptr + 1);
        memcpy(tmp, strptr, (delimptr ? delimptr : endptr) - strptr);
        kari_vec_push(ary->items, kari_create_string(tmp));
        strptr = (delimptr ? delimptr : endptr) + delim_bytes;
    } while(delimptr != NULL);
    
    return (kari_value_t*)ary;
}

K_FN(split)
{    
    KASSERT(argument->type == KARI_STRING, "Expected string");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_split_2), argument);
}

/* concat */
K_FN(_concat_2)
{
    size_t a_sz = strlen((char*)state);
    size_t b_sz = strlen(((kari_string_t*)argument)->str);
    char* buff = (char*)GC_MALLOC(a_sz + b_sz + 1);
    memcpy(buff, (char*)state, a_sz);
    memcpy(buff + a_sz, ((kari_string_t*)argument)->str, b_sz);
    return (kari_value_t*)kari_create_string(buff);
}

K_FN(concat)
{
    return (kari_value_t*)kari_create_native_function(context, K_REF(_concat_2), kari_str(argument));
}

/* index_of */
K_FN(_index_of_2)
{
    char *idx, *ptr;
    size_t cp_offset = 0;
    KASSERT(argument->type == KARI_STRING, "Expected string");
    idx = strstr(((kari_string_t*)state)->str, ((kari_string_t*)argument)->str);
    if(idx) {
        ptr = ((kari_string_t*)state)->str;
        while(ptr < idx) {
            if((*ptr & 0xc0) != 0x80) {
                cp_offset++;
            }
            ptr++;
        }
        return (kari_value_t*)kari_create_number(cp_offset);
    }
    return kari_nil();
}

K_FN(index_of)
{
    KASSERT(argument->type == KARI_STRING, "Expected string");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_index_of_2), argument);
}

/* interpolate */
K_FN(_interpolate_2)
{
    size_t len = 0, capacity = 4, ary_offset;
    char* ptr = ((kari_string_t*)state)->str;
    char* buff = (char*)GC_MALLOC(capacity);
    char* tmp;
    kari_array_t* ary = (kari_array_t*)argument;
    KASSERT(argument->type == KARI_ARRAY, "Expected array");
    
    while(*ptr) {
        if(*ptr == '%' && ptr[1] != 0 && ptr[1] >= '0' && ptr[1] <= '9') {
            ary_offset = 0;
            do {
                ary_offset *= 10;
                ary_offset += *(++ptr) - '0';
            } while(ptr[1] >= '0' && ptr[1] <= '9');
            tmp = kari_str(ary_offset < ary->items->count ? (kari_value_t*)ary->items->entries[ary_offset] : kari_nil());
            while(*tmp) {
                if(len + 2 >= capacity) {
                    buff = (char*)GC_REALLOC(buff, capacity * 2);
                    capacity *= 2;
                }
                buff[len++] = *tmp++;
            }
        } else {
            if(len + 2 >= capacity) {
                buff = (char*)GC_REALLOC(buff, capacity * 2);
                capacity *= 2;
            }
            buff[len++] = *ptr;
        }
        ptr++;
    }
    buff[len] = 0;
    return (kari_value_t*)kari_create_string(buff);
}

K_FN(interpolate)
{
    KASSERT(argument->type == KARI_STRING, "Expected string");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_interpolate_2), argument);
}