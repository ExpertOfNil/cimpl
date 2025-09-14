#ifndef CIMPL_STRING_H
#define CIMPL_STRING_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cimpl_types.h"

/* String */

// Provides a pointer to a new string with default capacity
// NOTE: This function allocates.  It is the responsibility of the user to free.
String* String_default(void) {
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

// Ensures capacity is at least the requested capacity
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

// Pushes a char onto the end of a string
CimplReturn String_push_char(String* str, char c) {
    if (String_reserve(str, str->count + 1) != RETURN_OK) {
        return RETURN_ERR;
    }
    str->items[str->count] = c;
    str->count++;
    return RETURN_OK;
}

// Concatenates a string view to the end of a string
CimplReturn String_push_view(String* dst, StringView* src) {
    u32 count = src->offset + 1;
    if (String_reserve(dst, dst->count + count) != RETURN_OK) {
        return RETURN_ERR;
    }
    memcpy(&dst->items[dst->count], src->begin, count);
    dst->count += count;
    return RETURN_OK;
}

// Pushes a string literal to the end of a string
CimplReturn String_push_literal(String* str, const char* items) {
    if (items == NULL) return RETURN_OK;
    u32 count = strlen(items);
    if (String_reserve(str, str->count + count) != RETURN_OK) {
        return RETURN_ERR;
    }
    memcpy(&str->items[str->count], items, count);
    str->count += count;
    return RETURN_OK;
}

// Zeros out items, but retains allocated memory
void String_clear(String* str) {
    memset(str->items, 0, str->count);
    str->count = 0;
}

// Releases allocated memory
void String_free(String* str) {
    free(str->items);
    str->items = NULL;
    str->count = 0;
    str->capacity = 0;
}

/* StringArray */

// Provides a pointer to a new string array with default capacity
// NOTE: This function allocates.  It is the responsibility of the user to free.
StringArray* StringArray_default(void) {
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

// Ensures capacity is at least the requested capacity
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

// Copies the provided string to a new string item
CimplReturn StringArray_push(StringArray* arr, String str) {
    if (StringArray_reserve(arr, arr->count + 1) != RETURN_OK) {
        return RETURN_ERR;
    }
    arr->items[arr->count] = str;
    arr->count++;
    return RETURN_OK;
}

// Zeros out all strings, but retains allocated memory
void StringArray_clear(StringArray* arr) {
    for (u32 i = 0; i < arr->count; ++i) {
        String_clear(&arr->items[i]);
    }
    arr->count = 0;
}

// Releases allocated memory for all strings
// NOTE: Do not free the string individually after calling this
void StringArray_free(StringArray* arr) {
    for (u32 i = 0; i < arr->count; ++i) {
        String_free(&arr->items[i]);
    }
    free(arr->items);
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

/* StringRingBuffer */

// Ensures capacity is at least the requested size
CimplReturn StringRingBuffer_reserve(StringRingBuffer* buf, u32 capacity) {
    if (capacity > buf->capacity) {
        if (buf->capacity == 0) {
            buf->capacity = DEFAULT_STRING_CAPACITY;
        }
        while (capacity > buf->capacity) {
            buf->capacity *= 2;
        }
        buf->items = realloc(buf->items, buf->capacity);
        if (buf->items == NULL) {
            fprintf(stderr, "Out of memory");
            return RETURN_ERR;
        }
    }
    return RETURN_OK;
}

// Copies string view contents to the buffer
CimplReturn StringRingBuffer_push(StringRingBuffer* dst, StringView* src) {
    u32 new_char_count = src->offset + 1;
    u32 vacant = dst->capacity - dst->count;
    if (new_char_count > vacant) {
        CimplReturn err =
            StringRingBuffer_reserve(dst, dst->capacity + new_char_count);
        if (err != RETURN_OK) return RETURN_ERR;
    }
    // Find index after last valid character
    u32 write_index = dst->read_index + dst->count % dst->capacity;
    memcpy(&dst->items[write_index], src->begin, new_char_count);
    dst->count += new_char_count;
    return RETURN_OK;
}

// Zeros the memory but retains allocated space
void StringRingBuffer_clear(StringRingBuffer* buf) {
    memset(buf->items, 0, buf->capacity);
    buf->count = 0;
    buf->read_index = 0;
}

// Release allocated memory
void StringRingBuffer_free(StringRingBuffer* buf) {
    free(buf->items);
    buf->items = NULL;
    buf->read_index = 0;
    buf->count = 0;
    buf->capacity = 0;
}

#endif /* CIMPL_STRING_H */
