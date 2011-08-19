#include <stdlib.h>
#include <stdio.h>
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
    kari_vec_t* tokens_stack = new_kari_vec();
    kari_vec_t* tokens = new_kari_vec();
    size_t source_len = strlen(source);
    size_t i = 0, identifier_start, cap, len;
    kari_token_t* tmp_tok;
    kari_vec_t* tmp_vec;
    kari_array_token_t* tmp_ary;
    char *tmp_s, *tmp_endptr;
    double d;
    bool is_after_assignment = false;
    bool is_reference = false;
    
    kari_vec_push(tokens_stack, tokens);
    
    while(i < source_len) {
        if(isspace(source[i])) {
            i++;
            continue;
        }
        /* identifier */
        if(!is_reference && source[i] == '@') {
            is_reference = true;
            ++i;
        }
        if(is_identifier_char(source[i], false)) {
            identifier_start = i;
            while(++i < source_len && is_identifier_char(source[i], true));
            
            tmp_s = (char*)GC_MALLOC(i - identifier_start + 1);
            memcpy(tmp_s, source + identifier_start, i - identifier_start);
            tmp_s[i - identifier_start] = 0;
            
            tmp_tok = (kari_token_t*)GC_MALLOC(sizeof(kari_identifier_token_t));
            tmp_tok->type = (is_after_assignment ? KARI_TOK_ASSIGN_TO_IDENTIFIER : KARI_TOK_IDENTIFIER);
            ((kari_identifier_token_t*)tmp_tok)->is_reference = is_reference;
            ((kari_identifier_token_t*)tmp_tok)->str = tmp_s;
            ((kari_identifier_token_t*)tmp_tok)->len = i - identifier_start;
            kari_vec_push(tokens, tmp_tok);
            
            is_reference = false;
            is_after_assignment = false;
            continue;
        }
        if(is_reference) {
            *err = "Expected identifier after reference operator";
            return 0;
        }
        if(is_after_assignment) {
            *err = "Expected identifier after assignment operator";
            return 0;
        }
        if(source[i] == '=' && i + 1 < source_len && source[i+1] == '>') {
            if(tokens->count == 0) {
                *err = "Expression must precede assignment operator";
                return 0;
            }
            is_after_assignment = true;
            i += 2;
            continue;
        }
        /* beginning of a function */
        if(source[i] == '(') {
            tmp_tok = (kari_token_t*)GC_MALLOC(sizeof(kari_function_token_t));
            tmp_tok->type = KARI_TOK_FUNCTION;
            ((kari_function_token_t*)tmp_tok)->argument = NULL;
            kari_vec_push(tokens, tmp_tok);
            tokens = new_kari_vec();
            ((kari_function_token_t*)tmp_tok)->tokens = tokens;
            kari_vec_push(tokens_stack, tokens);
            ++i;
            continue;
        }
        /* end of a function */
        if(source[i] == ')') {
            /* double check last token on parent tokens vec is a function token */
            if(tokens_stack->count > 1) {
                tmp_vec = (kari_vec_t*)tokens_stack->entries[tokens_stack->count - 2];
                if(tmp_vec->count > 0 && ((kari_token_t*)tmp_vec->entries[tmp_vec->count - 1])->type == KARI_TOK_FUNCTION) {
                    /* ok! */
                    kari_vec_pop(tokens_stack);
                    tokens = (kari_vec_t*)tokens_stack->entries[tokens_stack->count - 1];
                    ++i;
                    continue;
                }
            }
            *err = "Unexpected ')'";
            return 0;
        }
        if(source[i] == '[') {
            tmp_tok = (kari_token_t*)GC_MALLOC(sizeof(kari_array_token_t));
            tmp_tok->type = KARI_TOK_ARRAY;
            kari_vec_push(tokens, tmp_tok);
            
            ((kari_array_token_t*)tmp_tok)->items = new_kari_vec();
            tokens = new_kari_vec();
            kari_vec_push(((kari_array_token_t*)tmp_tok)->items, tokens);
            kari_vec_push(tokens_stack, tokens);
            ++i;
            continue;
        }
        if(source[i] == ']') {
            if(tokens_stack->count > 1) {
                tmp_vec = (kari_vec_t*)tokens_stack->entries[tokens_stack->count - 2];
                if(tmp_vec->count > 0 && ((kari_token_t*)tmp_vec->entries[tmp_vec->count - 1])->type == KARI_TOK_ARRAY) {
                    /* ok! */
                    tmp_ary = (kari_array_token_t*)tmp_vec->entries[tmp_vec->count - 1];
                    if(((kari_vec_t*)tmp_ary->items->entries[tmp_ary->items->count - 1])->count == 0) {
                        /* allow for trailing comma and don't add an extra element */
                        kari_vec_pop(tmp_ary->items);
                    }
                    kari_vec_pop(tokens_stack);
                    tokens = (kari_vec_t*)tokens_stack->entries[tokens_stack->count - 1];
                    ++i;
                    continue;
                }
            }
            *err = "Unexpected ']'";
            return 0;
        }
        if(source[i] == ',') {
            if(tokens_stack->count > 1) {
                tmp_vec = (kari_vec_t*)tokens_stack->entries[tokens_stack->count - 2];
                if(tmp_vec->count > 0) {
                    switch(((kari_token_t*)tmp_vec->entries[tmp_vec->count - 1])->type) {
                        case KARI_TOK_ARRAY:
                            
                            tmp_ary = (kari_array_token_t*)tmp_vec->entries[tmp_vec->count - 1];
                            kari_vec_pop(tokens_stack);
                            tokens = new_kari_vec();
                            kari_vec_push(tokens_stack, tokens);
                            kari_vec_push(tmp_ary->items, tokens);
                            
                            ++i;
                            continue;
                            
                        default:
                            *err = "Unexpected ','";
                            return 0;
                    }
                }
            }
            *err = "Unexpected ','";
            return 0;
        }
        /* '*' or '/' shorthand */
        if(source[i] == '*' || source[i] == '/') {
            tmp_tok = (kari_token_t*)GC_MALLOC(sizeof(kari_identifier_token_t));
            tmp_tok->type = KARI_TOK_IDENTIFIER;
            ((kari_identifier_token_t*)tmp_tok)->is_reference = false;
            ((kari_identifier_token_t*)tmp_tok)->str = (source[i] == '*' ? "mul" : "div");
            ((kari_identifier_token_t*)tmp_tok)->len = 3;
            kari_vec_push(tokens, tmp_tok);
            ++i;
            continue;
        }
        /* number */
        if(source[i] == '+' || source[i] == '-' || isdigit(source[i])) {
            if(source[i] == '-' && i + 1 < source_len && source[i+1] == '>') {
                /* function parameter operator, only valid if second operator in function after an identifier */
                if(tokens->count == 1 && ((kari_token_t*)tokens->entries[0])->type == KARI_TOK_IDENTIFIER) {
                    tmp_tok = (kari_token_t*)kari_vec_pop(tokens);
                    if(tokens_stack->count > 1) {
                        tmp_vec = (kari_vec_t*)tokens_stack->entries[tokens_stack->count - 2];
                        if(tmp_vec->count > 0 && ((kari_token_t*)tmp_vec->entries[tmp_vec->count - 1])->type == KARI_TOK_FUNCTION) {
                            /* ok! */
                            ((kari_function_token_t*)tmp_vec->entries[tmp_vec->count - 1])->argument = ((kari_identifier_token_t*)tmp_tok)->str;
                            i += 2;
                            continue;
                        }
                    }
                }
                *err = "Unexpected '->'";
                return 0;
            }
            if(source[i] == '+' || source[i] == '-') {
                if(i + 1 == source_len || !(isdigit(source[i+1]) || source[i+1] == '.')) {
                    /* + and - are legal shorthand characters for add and sub */
                    tmp_tok = (kari_token_t*)GC_MALLOC(sizeof(kari_identifier_token_t));
                    tmp_tok->type = KARI_TOK_IDENTIFIER;
                    ((kari_identifier_token_t*)tmp_tok)->is_reference = false;
                    ((kari_identifier_token_t*)tmp_tok)->str = (source[i] == '+' ? "add" : "sub");
                    ((kari_identifier_token_t*)tmp_tok)->len = 3;
                    kari_vec_push(tokens, tmp_tok);
                    ++i;
                    continue;
                }
            }
            tmp_endptr = NULL;
            d = strtod(source + i, &tmp_endptr);
            i = tmp_endptr - source;
            
            tmp_tok = (kari_token_t*)GC_MALLOC(sizeof(kari_number_token_t));
            tmp_tok->type = KARI_TOK_NUMBER;
            ((kari_number_token_t*)tmp_tok)->number = d;
            kari_vec_push(tokens, tmp_tok);
            
            continue;
        }
        if(source[i] == '"') {
            len = 0;
            cap = 4;
            tmp_s = (char*)GC_MALLOC(cap);
            while(true) {
                ++i;
                if(i == source_len) {
                    *err = "Unterminated string literal";
                    return 0;
                }
                if(source[i] == '"') {
                    ++i;
                    break;
                }
                if(len == cap) {
                    cap *= 2;
                    tmp_s = (char*)GC_REALLOC(tmp_s, cap);
                }
                if(source[i] == '\\') {
                    ++i;
                    if(i == source_len) {
                        *err = "Unterminated string literal";
                        return 0;
                    }
                    switch(source[i]) {
                        case 'n':
                            tmp_s[len++] = '\n';
                            break;
                        case 'r':
                            tmp_s[len++] = '\r';
                            break;
                        default:
                            tmp_s[len++] = source[i];
                            break;
                    }
                } else {    
                    tmp_s[len++] = source[i];
                }
            }
            
            tmp_tok = (kari_token_t*)GC_MALLOC(sizeof(kari_string_token_t));
            tmp_tok->type = KARI_TOK_STRING;
            ((kari_string_token_t*)tmp_tok)->str = tmp_s;
            ((kari_string_token_t*)tmp_tok)->len = len;
            kari_vec_push(tokens, tmp_tok);
            
            continue;
        }
        /* nil char */
        if(source[i] == '!') {
            tmp_tok = (kari_token_t*)GC_MALLOC(sizeof(kari_token_t));
            tmp_tok->type = KARI_TOK_NIL;
            kari_vec_push(tokens, tmp_tok);
            i++;
            continue;
        }
        
    /*illegal_char:*/
        *err = (char*)GC_MALLOC(64);
        sprintf(*err, "Illegal character '%c'", source[i]);
        return 0;
    }    
    if(is_after_assignment) {
        *err = "Expected identifier after => operator";
        return 0;
    }
    if(tokens_stack->count > 1) {
        kari_vec_pop(tokens_stack);
        tokens = (kari_vec_t*)tokens_stack->entries[tokens_stack->count - 1];
        *err = (char*)GC_MALLOC(64);
        tmp_tok = (kari_token_t*)tokens->entries[tokens->count - 1];
        sprintf(*err, "Unterminated %s (%d)", kari_string_for_token_type_t(tmp_tok->type), tmp_tok->type);
        return 0;
    }
    *err = NULL;
    *tokens_out = (kari_token_t**)tokens->entries;
    return tokens->count;
}