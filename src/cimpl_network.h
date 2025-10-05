#ifndef CIMPL_NETWORK_H
#define CIMPL_NETWORK_H

#include "cimpl_core.h"

typedef struct IpV4Addr {
    u32 addr;
    u16 port;
} IpV4Addr;

void IpV4Addr_print(IpV4Addr*);
i32 IpV4Addr_from_str(IpV4Addr*, char*);
i32 parse_ip_str(IpV4Addr*, const char*);
i32 udp_listener_setup(isize*, IpV4Addr);

#endif /* CIMPL_NETWORK_H */
