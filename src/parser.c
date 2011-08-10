#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "vec.h"
#include "kari.h"

static bool is_identifier_char(char c, bool first) {
    return isalpha(c) || (first && isdigit(c)) || c == '_';
}

size_t kari_parse(char* source, kari_token_t*** tokens_out, char** err)
{
    kari_vec_t* tokens = new_kari_vec();
    size_t source_len = strlen(source);
    size_t i = 0, identifier_start;
    kari_token_t* tmp_tok;
    char *ident, *tmp_endptr;
    double d;
    while(i < source_len) {
        if(isspace(source[i])) {
            i++;
            continue;
        }
        /* identifier */
        if(is_identifier_char(source[i], false)) {
            identifier_start = i;
            while(++i < source_len && is_identifier_char(source[i], true));
            
            ident = GC_MALLOC(i - identifier_start + 1);
            memcpy(ident, source + identifier_start, i - identifier_start);
            ident[i - identifier_start] = 0;
            
            tmp_tok = GC_MALLOC(sizeof(kari_identifier_token_t));
            tmp_tok->type = KARI_TOK_IDENTIFIER;
            ((kari_identifier_token_t*)tmp_tok)->is_reference = false;
            ((kari_identifier_token_t*)tmp_tok)->str = ident;
            ((kari_identifier_token_t*)tmp_tok)->len = i - identifier_start;
            kari_vec_push(tokens, tmp_tok);
            
            continue;
        }
        /* number */
        if(source[i] == '+' || source[i] == '-' || isdigit(source[i])) {
            tmp_endptr = NULL;
            d = strtod(source + i, &tmp_endptr);
            i = tmp_endptr - source;
            
            tmp_tok = GC_MALLOC(sizeof(kari_number_token_t));
            tmp_tok->type = KARI_TOK_NUMBER;
            ((kari_number_token_t*)tmp_tok)->number = d;
            kari_vec_push(tokens, tmp_tok);
        }
    }
    *err = NULL;
    *tokens_out = (kari_token_t**)tokens->entries;
    return tokens->count;
}