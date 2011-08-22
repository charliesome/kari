#include <stdlib.h>
#include <stdio.h>
#include <gc.h>
#include "kari.h"
#include "vec.h"

kari_value_t* kari_execute(kari_context_t* ctx, kari_token_t** tokens, size_t token_count, char** err)
{
    kari_vec_t* function_stack = new_kari_vec();
    kari_value_t *value = NULL, *tmp_val;
    kari_dict_t* lookup_dict;
    kari_function_t* tmp_fun;
    kari_array_token_t* tmp_ary;
    kari_vec_t* tmp_vec;
    kari_dict_val_t* tmp_dict;
    size_t i = 0, tmp_i = 0;
    while(i < token_count || function_stack->count > 0) {
        if(i < token_count) {
            switch(tokens[i]->type) {
                
                
                case KARI_TOK_IDENTIFIER:
                    lookup_dict = ctx->variables;
                    while(lookup_dict != NULL) {
                        if(kari_dict_exists(lookup_dict, ((kari_identifier_token_t*)tokens[i])->str)) {
                            value = (kari_value_t*)kari_dict_find_value(lookup_dict, ((kari_identifier_token_t*)tokens[i])->str);
                            if(K_IS_CALLABLE(value->type)) {
                                kari_vec_push(function_stack, value);
                                value = NULL;
                            }
                            goto found_value;
                        }
                        tmp_dict = (kari_dict_val_t*)kari_dict_find_value(lookup_dict, "$parent");
                        if(tmp_dict == NULL || tmp_dict->base.type != KARI_DICT) {
                            break;
                        }
                        lookup_dict = tmp_dict->items;
                    }
                    *err = (char*)GC_MALLOC(25 + ((kari_identifier_token_t*)tokens[i])->len);
                    sprintf(*err, "Undefined identifier '%s'", ((kari_identifier_token_t*)tokens[i])->str);
                    return NULL;
                    found_value: break;
                    
                    
                case KARI_TOK_ASSIGN_TO_IDENTIFIER:    
                    if(value == NULL) {
                        value = (kari_value_t*)kari_vec_pop(function_stack);
                    }
                    lookup_dict = ctx->variables;
                    while(lookup_dict != NULL) {
                        if(kari_dict_exists(lookup_dict, ((kari_identifier_token_t*)tokens[i])->str)) {
                            break;
                        }
                        tmp_dict = (kari_dict_val_t*)kari_dict_find_value(lookup_dict, "$parent");
                        if(tmp_dict == NULL || tmp_dict->base.type != KARI_DICT) {
                            break;
                        }
                        lookup_dict = tmp_dict->items;
                    }
                    if(lookup_dict == NULL) {
                        lookup_dict = ctx->variables;
                    }
                    kari_dict_set(lookup_dict, ((kari_identifier_token_t*)tokens[i])->str, value);
                    break;
                
                case KARI_TOK_MEMBER_ACCESS_STR:
                    if(value == NULL || value->type != KARI_DICT) {
                        *err = "Attempted to access string key from a non-dictionary value";
                        return NULL;
                    }
                    value = (kari_value_t*)kari_dict_find_value(((kari_dict_val_t*)value)->items, ((kari_identifier_token_t*)tokens[i])->str);
                    if(value == NULL) {
                        value = kari_nil();
                    }
                    if(K_IS_CALLABLE(value->type)) {
                        kari_vec_push(function_stack, value);
                        value = NULL;
                    }
                    break;
                    
                case KARI_TOK_MEMBER_ACCESS_INT:
                    if(value == NULL || value->type != KARI_ARRAY) {
                        *err = "Attempted to access index from a non-array value";
                        return NULL;
                    }
                    if(((kari_number_token_t*)tokens[i])->number < 0) {
                        tmp_i = ((kari_array_t*)value)->items->count + (long)((kari_number_token_t*)tokens[i])->number;
                    } else {
                        tmp_i = (long)((kari_number_token_t*)tokens[i])->number;
                    }
                    if(tmp_i >= ((kari_array_t*)value)->items->count) {
                        value = kari_nil();
                    } else {
                        value = (kari_value_t*)((kari_array_t*)value)->items->entries[tmp_i];
                    }
                    if(K_IS_CALLABLE(value->type)) {
                        kari_vec_push(function_stack, value);
                        value = NULL;
                    }
                    break;
                
                case KARI_TOK_FUNCTION:
                    tmp_fun = (kari_function_t*)GC_MALLOC(sizeof(kari_function_t));
                    tmp_fun->base.type = KARI_FUNCTION;
                    tmp_fun->parent = ctx;
                    tmp_fun->argument = (((kari_function_token_t*)tokens[i])->argument != NULL ? ((kari_function_token_t*)tokens[i])->argument : "");
                    tmp_fun->tokens = (kari_token_t**)((kari_function_token_t*)tokens[i])->tokens->entries;
                    tmp_fun->token_count = ((kari_function_token_t*)tokens[i])->tokens->count;
                    value = (kari_value_t*)tmp_fun;
                    tmp_fun = NULL;
                    break;
                
                    
                case KARI_TOK_NUMBER:
                    value = (kari_value_t*)GC_MALLOC(sizeof(kari_number_t));
                    value->type = KARI_NUMBER;
                    ((kari_number_t*)value)->number = ((kari_number_token_t*)tokens[i])->number;
                    break;
                    
                    
                case KARI_TOK_STRING:
                    value = (kari_value_t*)GC_MALLOC(sizeof(kari_string_t));
                    value->type = KARI_STRING;
                    ((kari_string_t*)value)->str = ((kari_string_token_t*)tokens[i])->str;
                    ((kari_string_t*)value)->len = ((kari_string_token_t*)tokens[i])->len;
                    break;
                    
                    
                case KARI_TOK_NIL:
                    value = kari_nil();
                    break;
                    
                    
                case KARI_TOK_ARRAY:    
                    value = (kari_value_t*)GC_MALLOC(sizeof(kari_array_t));
                    value->type = KARI_ARRAY;
                    ((kari_array_t*)value)->items = new_kari_vec();
                    tmp_ary = (kari_array_token_t*)tokens[i];
                    for(tmp_i = 0; tmp_i < tmp_ary->items->count; tmp_i++) {
                        tmp_val = kari_execute(ctx, (kari_token_t**)((kari_vec_t*)tmp_ary->items->entries[tmp_i])->entries,
                            ((kari_vec_t*)tmp_ary->items->entries[tmp_i])->count, err);
                        if(tmp_val == NULL) {
                            return NULL;
                        }
                        kari_vec_push(((kari_array_t*)value)->items, tmp_val);
                    }
                    break;
                
                case KARI_TOK_DICT:
                    value = (kari_value_t*)GC_MALLOC(sizeof(kari_dict_val_t));
                    value->type = KARI_DICT;
                    ((kari_dict_val_t*)value)->items = new_kari_dict();
                    tmp_vec = ((kari_dict_token_t*)tokens[i])->items;
                    for(tmp_i = 0; tmp_i < tmp_vec->count; tmp_i++) {
                        tmp_val = kari_execute(ctx, (kari_token_t**)((kari_dict_entry_token_t*)tmp_vec->entries[tmp_i])->tokens->entries,
                            ((kari_dict_entry_token_t*)tmp_vec->entries[tmp_i])->tokens->count, err);
                        if(tmp_val == NULL) {
                            return NULL;
                        }
                        kari_dict_set(((kari_dict_val_t*)value)->items, ((kari_dict_entry_token_t*)tmp_vec->entries[tmp_i])->identifier, tmp_val);
                    }
                    break;
                    
                default:
                    *err = "Internal error (unknown token)";
                    return NULL;
            }
            i++;
        }
        while(function_stack->count > 0) {
            if(value == NULL) {
                break;
            }
            value = kari_call((kari_value_t*)kari_vec_pop(function_stack), value, err);
            if(*err) {
                return NULL;
            }
            if(K_IS_CALLABLE(value->type)) {
                kari_vec_push(function_stack, value);
                value = NULL;
            }
        }
        
        if(i >= token_count && value == NULL) {
            value = (kari_value_t*)kari_vec_pop(function_stack);
        }
    }    
    return value ? value : kari_nil();
}