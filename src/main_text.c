#include "cimpl_impl.h"

void print_msg(String msg) { printf("Message: %s", msg.items); }

CimplReturn parse_message(StringRingBuffer* buf, void (*msg_handler)(String)) {
    // Nothing to do
    if (buf->count == 0) return RETURN_OK;
    // Only allocate once.  Clear before return.  No need to free `items`.
    static String temp_msg_buf = {0};
    u32 read_index = buf->read_index;
    for (u32 count = buf->count; count > 0; count--) {
        char c = buf->items[read_index++];
        String_push_char(&temp_msg_buf, c);
        if (c == '\n') {
            // We have a complete message
            // Update read index to start at the next message
            buf->read_index = read_index;
            // Update count to account for flushed chars
            buf->count = count;
            // Do whatever needs to be done with the message
            msg_handler(temp_msg_buf);
            // Clear for the next message
            String_clear(&temp_msg_buf);
        }
    }
    printf("Remaining: %.*s\n", buf->count, &buf->items[buf->read_index]);
    String_clear(&temp_msg_buf);
    return RETURN_OK;
}

i32 main(int argc, char** argv) {
    const char* fname;
    if (argc > 1) {
        fname = argv[1];
    } else {
        fname = "test_messages.txt";
    }

    i32 fd = open(fname, O_RDONLY);
    char temp_buf[64];
    StringRingBuffer read_buf = {0};
    StringRingBuffer_reserve(&read_buf, DEFAULT_STRING_CAPACITY);

    i64 read_bytes = read(fd, temp_buf, sizeof(temp_buf));
    if (read_bytes < 0) {
        fprintf(stderr, "Failed to read from file %s", fname);
        return RETURN_ERR;
    }
    if (read_bytes == 0) {
        printf("Nothing to read");
        return RETURN_OK;
    }
    StringView sv = {
        .begin = temp_buf,
        .offset = read_bytes - 1,
    };
    StringRingBuffer_push(&read_buf, &sv);
    parse_message(&read_buf, print_msg);

    StringRingBuffer_free(&read_buf);
    return RETURN_OK;
}
