#include <errno.h>

#include "cimpl_serial.h"
#include "cimpl_string.h"

void print_msg(String msg) { printf("Message: %s\n", msg.items); }

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
    const char* device;
    if (argc > 1) {
        device = argv[1];
    } else {
        device = SERIAL_DEVICE;
    }

    i32 serial_fd = serial_start(device);
    if (serial_fd < 0) return RETURN_ERR;

    char temp_buf[64];
    StringRingBuffer read_buf = {0};
    StringRingBuffer_reserve(&read_buf, DEFAULT_STRING_CAPACITY);
    while (1) {
        i64 read_bytes = read(serial_fd, temp_buf, sizeof(temp_buf));
        if (read_bytes < 0) {
            // Resource temporarily available, not critical error
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                fprintf(stderr, "Failed to read from %s", device);
                return RETURN_ERR;
            }
        }
        if (read_bytes == 0) continue;
        // TODO (mmckenna): do something with the message
        StringView sv = {
            .begin = temp_buf,
            .count = read_bytes,
        };
        StringRingBuffer_push(&read_buf, &sv);
        parse_message(&read_buf, print_msg);
    }

    StringRingBuffer_free(&read_buf);
    return RETURN_OK;
}
