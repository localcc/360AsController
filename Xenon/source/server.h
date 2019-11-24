#include <network/network.h>
#include <lwip/tcp.h>
#include <stdlib.h>

#ifndef _S360_H_
#define _S360_H_

struct tcp_pcb* listen_pcb;
struct server_state {
	struct tcp_pcb* pcb;
	struct pbuf* p;
};

void server_send(struct tcp_pcb* pcb, struct server_state* state);
void conn_err(void *arg, err_t err);
void close_conn(struct tcp_pcb *pcb, struct server_state* state);
err_t server_poll(void *arg, struct tcp_pcb *pcb);
err_t server_sent(void *arg, struct tcp_pcb *pcb, u16_t len);
err_t server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
err_t server_accept(void *arg, struct tcp_pcb *pcb, err_t err);
void start_server(void);

#endif

