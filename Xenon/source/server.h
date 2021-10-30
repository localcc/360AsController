#include <network/network.h>
#include <lwip/udp.h>
#include <lwip/sys.h>
#include <stdlib.h>

#ifndef _S360_H_
#define _S360_H_

void udp_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf* p, struct ip_addr *addr, u16_t port);
void start_server(void);

#endif

