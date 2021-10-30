#include "server.h"
#include "controller.h"
#include <stdio.h>
#include <xenon_smc/xenon_smc.h>


uint8_t data_buf[14];

void udp_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf* p, struct ip_addr *addr, u16_t port) {

    LWIP_UNUSED_ARG(arg);
    if(p == NULL)
        return;
    unsigned char* data = (unsigned char*)p->payload;
    unsigned char command = data[0];
    pbuf_free(p);
    if(command == 1) {
        memset(data_buf, 0, 14);
        read_controller_data(data_buf);
        struct pbuf* t_pbuf = pbuf_alloc(PBUF_TRANSPORT, 14, PBUF_POOL);
        if(t_pbuf == NULL) {
            printf("Failed to allocate buffer!\n");
            return;
        }
        pbuf_take(t_pbuf, data_buf, 14);
        udp_sendto(pcb, t_pbuf, addr, port);
        pbuf_free(t_pbuf);
    }else if(command == 3) {
        write_controller_data(data + 1);
    }
}

void start_server(void) {
    struct udp_pcb* upcb = udp_new();
    udp_bind(upcb, IP_ADDR_ANY, 1182);
    udp_recv(upcb, udp_server_recv, NULL);
}
