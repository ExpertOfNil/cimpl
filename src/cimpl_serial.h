#ifndef CIMPL_SERIAL_H
#define CIMPL_SERIAL_H

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "cimpl_core.h"

#ifndef SERIAL_DEVICE
#define SERIAL_DEVICE "/dev/tty/ACM0"
#endif

#ifndef SERIAL_BAUD
#define SERIAL_BAUD 115200
#endif

CimplReturn serial_configure(i32, u32);
i32 serial_start(const char*);

#endif /* CIMPL_SERIAL */
