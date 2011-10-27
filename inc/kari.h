#ifndef _KARI_H
#define _KARI_H

#include <stdbool.h>
#include <stdint.h>
#include <gc.h>
#include "st.h"
#include "dict.h"
#include "vec.h"

/*
// CONTEXT
*/
typedef struct kari_context {
    struct kari_context* parent;
    st_table* variables;
} kari_context_t;

/*
// TOKENS
*/
typedef enum kari_token_type {
    KARI_TOK_NIL,
    KARI_TOK_NUMBER,
    KARI_TOK_IDENTIFIER,
    KARI_TOK_STRING,
    KARI_TOK_ASSIGN_TO_IDENTIFIER,
    KARI_TOK_FUNCTION,
    KARI_TOK_ARRAY,
    KARI_TOK_DICT,
    KARI_TOK_MEMBER_ACCESS_STR,
    KARI_TOK_MEMBER_ACCESS_INT
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

typedef struct kari_function_token {
    kari_token_t base;
    char* argument;
    kari_vec_t* tokens;
} kari_function_token_t;

typedef struct kari_array_token {
    kari_token_t base;
    kari_vec_t* items;
} kari_array_token_t;

typedef struct kari_dict_entry_token {
    char* identifier;
    kari_vec_t* tokens;
} kari_dict_entry_token_t;

typedef struct kari_dict_token {
    kari_token_t base;
    kari_vec_t* items;
} kari_dict_token_t;

/*
// RUNTIME VALUES
*/
typedef enum kari_value_type {
    KARI_NIL = 0x00,
    KARI_NUMBER = 0x01,
    KARI_STRING = 0x02,
    KARI_NATIVE_FUNCTION = 0x80,
    KARI_FUNCTION = 0x81,
    KARI_TRUE = 0x40,
    KARI_FALSE = 0x41,
    KARI_ARRAY = 0x05,
    KARI_DICT = 0x06
} kari_value_type_t;
#define K_IS_CALLABLE(t) (t & 0x80)
#define K_IS_BOOLEAN(t) (t & 0x40)

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

typedef struct kari_array {
    kari_value_t base;
    kari_vec_t* items;
} kari_array_t;

typedef struct kari_dict_val {
    kari_value_t base;
    kari_dict_t* items;
} kari_dict_val_t;

typedef kari_value_t*(*kari_nfn_t)(kari_context_t*,void*,kari_value_t*,char**);
typedef struct kari_native_function {
    kari_value_t base;
    kari_context_t* context;
    void* state;
    kari_value_t*(*call)(kari_context_t*, void*, kari_value_t*, char**);
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
kari_value_t* kari_true();
kari_value_t* kari_false();
kari_value_t* kari_boolean(bool boolean);
kari_context_t* kari_create_std_context();
void kari_load_stdlib(kari_context_t* context);
kari_value_t* kari_call(kari_value_t* function, kari_value_t* argument, char** err);
size_t kari_parse(char* source, kari_token_t*** tokens_out, char** err);
kari_value_t* kari_execute(kari_context_t* ctx, kari_token_t** tokens, size_t token_count, char** err);
kari_value_t* kari_eval(kari_context_t* ctx, char* source, char** err);
kari_value_t* kari_var_get(kari_context_t* ctx, char* name);
char* kari_string_for_value_type_t(kari_value_type_t t);
char* kari_string_for_token_type_t(kari_token_type_t t);
char* kari_inspect(kari_value_t* value);
char* kari_str(kari_value_t* value);

kari_number_t* kari_create_number(double number);
kari_string_t* kari_create_string(char* str);
kari_array_t* kari_create_array();
kari_native_function_t* kari_create_native_function(kari_context_t* context, kari_nfn_t fn, void* state);

size_t kari_utf8_strlen(char* s);

#endif