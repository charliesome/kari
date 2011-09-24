#include "kari.h"
#include "kari_stdlib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include <unistd.h>
#include <sys/stat.h>

/* quit */
K_FN(quit)
{
    if(argument->type != KARI_NUMBER) {
        exit(0);
    }
    exit((int)((kari_number_t*)argument)->number);
}

/* inspect */
K_FN(inspect)
{
    return (kari_value_t*)kari_create_string(kari_inspect(argument));
}

/* str */
K_FN(str)
{
    return (kari_value_t*)kari_create_string(kari_str(argument));
}

/* put */
K_FN(put)
{
    printf("%s\n", kari_str(argument));
    return kari_nil();
}

/* try */
K_FN(try)
{    
    char* e = NULL;
    kari_value_t* val = NULL;
    KASSERT(K_IS_CALLABLE(argument->type), "Expected function");
    
    val = kari_call(argument, kari_nil(), &e);
    if(e) {
        return (kari_value_t*)kari_create_string(e);
    } else {
        return val;
    }
}

/* require */
K_FN(require)
{
    size_t i, arg_len, tmp, src_len, src_capacity;
    struct stat stat_buf;
    kari_array_t* arr = (kari_array_t*)kari_var_get(context, "$inc");
    kari_vec_t* inc;
    char* buff;
    char* src;
    char* arg;
    FILE* f;
    
    KASSERT(argument->type == KARI_STRING, "Expected string");
    arg = ((kari_string_t*)argument)->str;
    
    arg_len = strlen(arg);
    KASSERT(arr != NULL && arr->base.type == KARI_ARRAY, "Expected array in $inc");
    inc = arr->items;
    
    for(i = 0; i < inc->count; i++) {
        if(((kari_value_t*)inc->entries[i])->type != KARI_STRING)
            continue;
        tmp = strlen(((kari_string_t*)inc->entries[i])->str);
        buff = (char*)GC_MALLOC(arg_len + tmp + 8);
        strcpy(buff, ((kari_string_t*)inc->entries[i])->str);
        buff[tmp] = '/';
        strcpy(buff + tmp + 1, arg);
        strcpy(buff + arg_len + tmp + 1, ".kari");
        if(stat(buff, &stat_buf) == 0) {
            /* found the file to include */
            f = fopen(buff, "r");
            if(f == NULL) {
                continue;
            }
            
            src_len = 0;
            src_capacity = 4096;
            src = (char*)GC_MALLOC(src_capacity);
            while(fread(src + src_len, 1, 4096, f) == 4096) {
                src_len += 4096;
                src_capacity += 4096;
                src = (char*)GC_REALLOC(src, src_capacity);
            }
            
            kari_eval(context, src, err);
            if(*err) {
                return NULL;
            }
            return kari_true();
        }
    }
    
    buff = (char*)GC_MALLOC(strlen("No such file '") + arg_len + 2);
    strcpy(buff, "No such file '");
    strcpy(buff + strlen("No such file '"), arg);
    strcpy(buff + strlen("No such file '") + arg_len, "'");
    *err = buff;
    return NULL;
}