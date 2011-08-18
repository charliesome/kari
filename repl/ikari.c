#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "kari.h"

char* readstr(bool end_on_newline)
{
    int c;
    size_t len = 0, capacity = 16;
    char* s = GC_MALLOC(capacity);
    while(true) {
        c = getchar();
        if(c != EOF && !(end_on_newline && c == '\n')) {
            s[len++] = c;
            if(len == capacity) {
                capacity *= 2;
                s = GC_REALLOC(s, capacity);
            }
        } else {
            s[len++] = 0;
            return GC_REALLOC(s, len);
        }
    }
}

int main(int argc, char** argv)
{
    kari_context_t* ctx;
    char *src, *err = NULL;
    kari_token_t** tokens;
    kari_value_t* value;
    size_t tokens_count;
    bool execute_mode = false;
    
    if(argc > 1 && strcmp(argv[1], "-x") == 0) {
        execute_mode = true;
    }
    
    GC_INIT();
    if(!execute_mode) {
        printf("Interactive Kari\n\n");
    }
    ctx = kari_create_std_context();
    
    if(execute_mode) {
        tokens_count = kari_parse(readstr(false), &tokens, &err);
        if(err) {
            fprintf(stderr, "Parse error: %s\n", err);
            exit(1);
        }
        value = kari_execute(ctx, tokens, tokens_count, &err);
        if(err) {
            fprintf(stderr, "Runtime error: %s\n", err);
            exit(1);
        }
    } else {
        while(true) {
            if(feof(stdin)) {
                break;
            }
            printf(">> ");
            src = readstr(true);
            if(src[0] == 0) {
                continue;
            }
            tokens_count = kari_parse(src, &tokens, &err);
            if(err) {
                printf("Parse error: %s\n", err);
                continue;
            }
            value = kari_execute(ctx, tokens, tokens_count, &err);
            if(err) {
                printf("Runtime error: %s\n", err);
                continue;
            }
            printf("%s\n", kari_inspect(value));
        }
    }
    
    return 0;
}