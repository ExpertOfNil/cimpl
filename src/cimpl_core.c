#include "cimpl_core.h"

const char* get_timestamp(void) {
    static char timestamp[20];
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", timeinfo);
    return timestamp;
}

void log_message(
    const char* level_str, const char* format, va_list args
) {
    printf("[%s] %s: ", get_timestamp(), level_str);
    vprintf(format, args);
    printf("\n");
    fflush(stdout);

    va_end(args);
}
void log_trace(const char* format, ...) {
    const u8 level = 0;
    if (LOG_LEVEL > level) return;
    va_list args;
    va_start(args, format);
    log_message("TRACE", format, args);
}
void log_debug(const char* format, ...) {
    const u8 level = 1;
    if (LOG_LEVEL > level) return;
    va_list args;
    va_start(args, format);
    log_message("DEBUG", format, args);
}
void log_info(const char* format, ...) {
    const u8 level = 2;
    if (LOG_LEVEL > level) return;
    va_list args;
    va_start(args, format);
    log_message("INFO", format, args);
}
void log_warn(const char* format, ...) {
    const u8 level = 3;
    if (LOG_LEVEL > level) return;
    va_list args;
    va_start(args, format);
    log_message("WARN", format, args);
}
void log_error(const char* format, ...) {
    const u8 level = 4;
    if (LOG_LEVEL > level) return;
    va_list args;
    va_start(args, format);

    fprintf(stderr, "[%s] %s: ", get_timestamp(), "ERROR");
    vprintf(format, args);
    printf("\n");

    fflush(stderr);
    va_end(args);
}
