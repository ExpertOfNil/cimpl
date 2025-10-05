#include "cimpl_core.h"

const char* get_timestamp(void) {
    static char timestamp[20];
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", timeinfo);
    return timestamp;
}
