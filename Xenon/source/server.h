#include <network/network.h>
#include <lwip/tcp.h>
#include <stdlib.h>

#ifndef _S360_H_
#define _S360_H_

struct tcp_pcb* listen_pcb;

void conn_err(void *arg, err_t err);
void close_conn(struct tcp_pcb *pcb);
void reply_controller_data(struct tcp_pcb* pcb);
void accept_controller_data(unsigned char* buffer);
err_t server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
err_t server_accept(void *arg, struct tcp_pcb *pcb, err_t err);
void start_server(void);

#endif

