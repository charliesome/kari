#include "kari.h"
#include "kari_stdlib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

/* read_file */
K_FN(read_file)
{
    FILE* f;
    size_t buff_capacity = 1024;
    size_t buff_i = 0;
    char* buff = (char*)GC_MALLOC(buff_capacity);
    KASSERT(K_TYPE_OF(argument) == KARI_STRING, "Expected string");
    f = fopen(((kari_string_t*)argument)->str, "r");
    if(f == NULL) {
        return kari_nil();
    }
    while(fread(buff + buff_i, buff_capacity - buff_i, 1, f) == buff_capacity - buff_i) {
        buff_i = buff_capacity;
        buff_capacity *= 2;
        buff = (char*)GC_REALLOC(buff, buff_capacity);
    }
    fclose(f);
    return (kari_value_t*)kari_create_string(buff);
}

/* open_file */
void finalize_file_handle(FILE* f)
{
    printf("*** closing file handle, thanks boehm! ***\n");
    fclose(f);
}

K_FN(_open_file_2)
{
    FILE* f;
    KASSERT(K_TYPE_OF(argument) == KARI_STRING, "Expected string");
    f = fopen((char*)state, ((kari_string_t*)argument)->str);
    if(f == NULL) {
        *err = strerror(errno);
        return NULL;
    }
    return (kari_value_t*)kari_create_data(f, KARI_TAG_FILE, (void(*)(void*))finalize_file_handle);
}

K_FN(open_file)
{
    KASSERT(K_TYPE_OF(argument) == KARI_STRING, "Expected string");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_open_file_2), ((kari_string_t*)argument)->str);
}