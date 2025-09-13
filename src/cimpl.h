#ifndef CIMPL_H
#define CIMPL_H

#define DEFAULT_ARRAY_CAPACITY 64
#define DEFAULT_STRING_CAPACITY 256

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef enum {
    RETURN_OK,
    RETURN_ERR,
} CimplReturn;

typedef struct String {
    char* items;
    u32 count;
    u32 capacity;
} String;

typedef struct StringView {
    const char* begin;
    u32 offset;
} StringView;

typedef struct StringArray {
    String* items;
    u32 count;
    u32 capacity;
} StringArray;

static u32 randi(u32 index) {
    index = (index << 13) ^ index;
    return (index * (index * index * 15731 + 789221) + 1276312589) & 0x7fffffff;
}

#endif /* CIMPL_H */
