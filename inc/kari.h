#ifndef _KARI_H
#define _KARI_H

#include <stdbool.h>
#include <stdint.h>
#include <gc.h>
#include "dict.h"

/*
// CONTEXT
*/
typedef struct kari_context {
    struct kari_context* parent;
    kari_dict_t* variables;
} kari_context_t;

/*
// TOKENS
*/
typedef enum kari_token_type {
    KARI_TOK_NUMBER,
    KARI_TOK_IDENTIFIER,
    KARI_TOK_STRING
} kari_token_type_t;

typedef struct kari_token {
    kari_token_type_t type;
} kari_token_t;

typedef struct kari_identifier_token {
    kari_token_t base;
    bool is_reference;
    char* str;
    size_t len;
} kari_identifier_token_t;

typedef struct kari_number_token {
    kari_token_t base;
    double number;
} kari_number_token_t;

typedef struct kari_string_token {
    kari_token_t base;
    char* str;
    size_t len;
} kari_string_token_t;

/*
// RUNTIME VALUES
*/
typedef enum kari_value_type {
    KARI_NIL,
    KARI_NUMBER,
    KARI_STRING,
    KARI_NATIVE_FUNCTION,
    KARI_FUNCTION
} kari_value_type_t;

typedef struct kari_value {
    kari_value_type_t type;
} kari_value_t;

typedef struct kari_number {
    kari_value_t base;
    double number;
} kari_number_t;

typedef struct kari_string {
    kari_value_t base;
    char* str;
    size_t len;
} kari_string_t;

typedef struct kari_native_function {
    kari_value_t base;
    void* state;
    kari_value_t*(*call)(void*, kari_value_t*, char**);
} kari_native_function_t;

typedef struct kari_function {
    kari_value_t base;
    kari_context_t* parent;
    char* argument;
    kari_token_t** tokens;
    size_t token_count;
} kari_function_t;

/*
// API
*/
kari_value_t* kari_nil();
kari_context_t* kari_create_std_context();
void kari_load_stdlib(kari_context_t* context);
kari_value_t* kari_call(kari_value_t* function, kari_value_t* argument, char** err);
size_t kari_parse(char* source, kari_token_t*** tokens_out, char** err);
kari_value_t* kari_execute(kari_context_t* ctx, kari_token_t** tokens, size_t token_count, char** err);
char* kari_string_for_value_type_t(kari_value_type_t t);
char* kari_inspect(kari_value_t* value);

#endif