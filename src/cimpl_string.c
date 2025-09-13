#ifndef CIMPL_STRING_H
#define CIMPL_STRING_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cimpl.h"

String* String_default() {
    char* items = calloc(DEFAULT_STRING_CAPACITY, 1);
    if (items == NULL) {
        fprintf(stderr, "Out of memory");
        return NULL;
    }
    String* str = malloc(sizeof(String));
    if (str == NULL) {
        free(items);
        fprintf(stderr, "Out of memory");
        return NULL;
    }
    str->items = items;
    str->capacity = DEFAULT_STRING_CAPACITY;
    str->count = 0;
    return str;
}

CimplReturn String_reserve(String* str, u32 capacity) {
    if (capacity > str->capacity) {
        if (str->capacity == 0) {
            str->capacity = DEFAULT_STRING_CAPACITY;
        }
        while (capacity > str->capacity) {
            str->capacity *= 2;
        }
        str->items = realloc(str->items, str->capacity);
        if (str->items == NULL) {
            fprintf(stderr, "Out of memory");
            return RETURN_ERR;
        }
    }
    return RETURN_OK;
}

CimplReturn String_append(String* str, char c) {
    if (String_reserve(str, str->count + 1) != RETURN_OK) {
        return RETURN_ERR;
    }
    str->items[str->count] = c;
    str->count++;
    return RETURN_OK;
}

CimplReturn String_concat(String* dst, StringView* src) {
    u32 count = src->offset + 1;
    if (String_reserve(dst, dst->count + count) != RETURN_OK) {
        return RETURN_ERR;
    }
    memcpy(&dst->items[dst->count], src->begin, count);
    dst->count += count;
    return RETURN_OK;
}

CimplReturn String_append_literal(String* str, const char* items) {
    if (items == NULL) return RETURN_OK;
    u32 count = strlen(items);
    if (String_reserve(str, str->count + count) != RETURN_OK) {
        return RETURN_ERR;
    }
    memcpy(&str->items[str->count], items, count);
    str->count += count;
    return RETURN_OK;
}

void String_clear(String* str) {
    memset(str->items, 0, str->count);
    str->count = 0;
}

void String_free(String* str) {
    free(str->items);
    str->items = NULL;
    str->count = 0;
    str->capacity = 0;
}

StringArray* StringArray_default() {
    String* items = calloc(DEFAULT_ARRAY_CAPACITY, sizeof(String));
    if (items == NULL) {
        fprintf(stderr, "Out of memory");
        return NULL;
    }
    StringArray* arr = malloc(sizeof(StringArray));
    if (arr == NULL) {
        free(items);
        fprintf(stderr, "Out of memory");
        return NULL;
    }
    arr->items = items;
    arr->capacity = DEFAULT_ARRAY_CAPACITY;
    arr->count = 0;
    return arr;
}

CimplReturn StringArray_reserve(StringArray* arr, u32 capacity) {
    if (capacity > arr->capacity) {
        if (arr->capacity == 0) {
            arr->capacity = DEFAULT_ARRAY_CAPACITY;
        }
        while (capacity > arr->capacity) {
            arr->capacity *= 2;
        }
        arr->items = realloc(arr->items, sizeof(*arr) * arr->capacity);
        if (arr->items == NULL) {
            fprintf(stderr, "Out of memory");
            return RETURN_ERR;
        }
    }
    return RETURN_OK;
}

CimplReturn StringArray_push(StringArray* arr, String str) {
    if (StringArray_reserve(arr, arr->count + 1) != RETURN_OK) {
        return RETURN_ERR;
    }
    arr->items[arr->count] = str;
    arr->count++;
    return RETURN_OK;
}

void StringArray_clear(StringArray* arr) {
    for (u32 i = 0; i < arr->count; ++i) {
        String_clear(&arr->items[i]);
    }
    arr->count = 0;
}

void StringArray_free(StringArray* arr) {
    for (u32 i = 0; i < arr->count; ++i) {
        String_free(&arr->items[i]);
    }
    free(arr->items);
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

#endif /* CIMPL_STRING_H */
