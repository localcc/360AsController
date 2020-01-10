#include "server.h"
#include "controller.h"




void udp_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf* p, struct ip_addr *addr, u16_t port) {

    LWIP_UNUSED_ARG(arg);
    if(p == NULL)
        return;
    unsigned char* data = (unsigned char*)p->payload;
    unsigned char command = data[0];
    pbuf_free(p);
    if(command == 1) {
        unsigned char* cdata_buffer = (unsigned char*)malloc(14);
        read_controller_data(cdata_buffer);
        struct pbuf* t_pbuf = pbuf_alloc(PBUF_TRANSPORT, 14, PBUF_REF);
        t_pbuf->payload = cdata_buffer;
        t_pbuf->len = t_pbuf->tot_len = 14;
        udp_sendto(pcb, t_pbuf, addr, port);
        pbuf_free(t_pbuf);
        free(cdata_buffer);
    }else if(command == 2) {
        //TODO: turn console off
    }else if(command == 3) {
        write_controller_data(data + 1);
    }

}


void start_server(void) {

    upcb = udp_new();
    udp_bind(upcb, IP_ADDR_ANY, 1182);
    udp_recv(upcb, udp_server_recv, NULL);
}
