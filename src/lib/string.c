#include "kari.h"
#include "kari_stdlib.h"
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
        
    } else if(codepoint < 0x10ffff) {
        
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