#ifndef CIMPL_TYPES_H
#define CIMPL_TYPES_H

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef LOG_LEVEL
#define LOG_LEVEL 1
#endif

#define DEFAULT_ARRAY_CAPACITY 16
#define ARRAY_COUNT(arr) (sizeof((arr)) / sizeof((arr)[0]))

typedef size_t usize;
typedef ssize_t isize;

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

typedef unsigned char uchar;
typedef signed char ichar;

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
    char* items;
    u32 count;
} StringView;

typedef struct StringArray {
    String* items;
    u32 count;
    u32 capacity;
} StringArray;

typedef struct StringRingBuffer {
    char* items;
    // Always pointing at the first valid position
    u32 read_index;
    u32 count;
    u32 capacity;
} StringRingBuffer;

inline static u32 randi(u32 index) {
    index = (index << 13) ^ index;
    return (index * (index * index * 15731 + 789221) + 1276312589) & 0x7fffffff;
}

const char* get_timestamp(void);

void log_message(const char* level_str, const char* format, va_list args);
void log_trace(const char* format, ...);
void log_debug(const char* format, ...);
void log_info(const char* format, ...);
void log_warn(const char* format, ...);
void log_error(const char* format, ...);

#endif /* CIMPL_TYPES_H */
