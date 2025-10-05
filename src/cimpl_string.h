#ifndef CIMPL_STRING_H
#define CIMPL_STRING_H

#include "cimpl_core.h"

#define DEFAULT_STRING_CAPACITY 256

/* String */

String* String_default(void);
CimplReturn String_reserve(String*, u32);
CimplReturn String_push_char(String*, char);
CimplReturn String_push_view(String*, StringView*);
CimplReturn String_push_literal(String*, const char*);
void String_clear(String*);
void String_free(String*);

/* StringView */

u16 StringView_to_u16(StringView*);
u8 StringView_to_u8(StringView*);

/* StringArray */

StringArray* StringArray_default(void);
CimplReturn StringArray_reserve(StringArray*, u32);
CimplReturn StringArray_push(StringArray*, String);
void StringArray_clear(StringArray*);
void StringArray_free(StringArray*);

/* StringRingBuffer */

CimplReturn StringRingBuffer_reserve(StringRingBuffer*, u32);
CimplReturn StringRingBuffer_push(StringRingBuffer*, StringView*);
void StringRingBuffer_clear(StringRingBuffer*);
void StringRingBuffer_free(StringRingBuffer*);

#endif /* CIMPL_STRING_H */
