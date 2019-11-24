#include "server.h"

struct tcp_pcb* listen_pcb;



void server_send(struct tcp_pcb* pcb, struct server_state* state){
	struct pbuf* ptr;
	err_t r_err = ERR_OK;
	while((r_err == ERR_OK) && (state->p != NULL) && (state->p->len <= tcp_sndbuf(pcb))){
		ptr = state->p;

		r_err = tcp_write(pcb, ptr->payload, ptr->len, 1);
		if(r_err == ERR_OK){
			//Data sending
			u16_t plen;
			u8_t freed;

			plen = ptr->len;
			state->p = ptr->next;
			if(state->p != NULL){
				pbuf_ref(state->p);
			}

			do {
				freed = pbuf_free(ptr);
			}while(freed == 0);

			tcp_recved(pcb, plen);
		}else if(r_err == ERR_MEM) {
			state->p = ptr;
		}
	}
}


void conn_err(void *arg, err_t err) {
	printf("err\n");
}

void close_conn(struct tcp_pcb *pcb, struct server_state* state) {
	tcp_arg(pcb, NULL);
	tcp_sent(pcb, NULL);
	tcp_recv(pcb, NULL);
	tcp_err(pcb, NULL);
	tcp_poll(pcb, NULL, 0);
	if(state != NULL) {
		free(state);
	}
	tcp_close(pcb);
}


err_t server_poll(void *arg, struct tcp_pcb *pcb) {	
	struct server_state* state;

	state = (struct server_state*)arg;
	if(state != NULL){
		if(state->p != NULL){
			tcp_sent(pcb, server_sent);
			server_send(pcb, state);
		}
	}

	printf("polling!\n");

	return ERR_OK;
}

err_t server_sent(void *arg, struct tcp_pcb *pcb, u16_t len) {
	struct server_state* state;


	LWIP_UNUSED_ARG(len);

	state = (struct server_state*)arg;

	if(state->p != NULL){
		tcp_sent(pcb, server_sent);
		server_send(pcb, state);
	}
	return ERR_OK;
}

err_t server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err) {

	struct server_state* state;
	err_t r_err;

	state = (struct server_state*)arg;
	if(p == NULL) {
		//Conn closed
		if(state->p == NULL){
			//Done
			close_conn(pcb, state);
		}else{
			tcp_sent(pcb, server_sent);
			server_send(pcb, state);
		}
		r_err = ERR_OK;
	}else if(err != ERR_OK){
		if(p != NULL) {
			state->p = NULL;
			pbuf_free(p);
		}
		r_err = err;
	}else {
		if(state->p == NULL){
			state->p = p;

			tcp_sent(pcb, server_sent);
			//Send controller data
		}else {
			struct pbuf *buf;
			buf = state->p;
			pbuf_chain(buf, p);
		}

		r_err = ERR_OK;

	}


	return r_err;
}

err_t server_accept(void *arg, struct tcp_pcb *pcb, err_t err) {
	struct server_state* state;
	err_t r_err;

	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);

	tcp_setprio(pcb, TCP_PRIO_MIN);
	
	state = (struct server_state*)malloc(sizeof(struct server_state));
	if(state != NULL) {
		state->pcb = pcb;
		state->p = NULL;

		tcp_arg(pcb, state);

		tcp_recv(pcb, server_recv);

		tcp_err(pcb, conn_err);
		tcp_sent(pcb, server_sent);
		
		tcp_poll(pcb, server_poll, 4);
		
		tcp_accepted(listen_pcb); 
		r_err = ERR_OK;
	}else {
		r_err = ERR_MEM;
	}

	printf("accept!\n");
	return r_err;
}

void start_server(void) {
	//struct tcp_pcb *pcb;

	listen_pcb = tcp_new();
	if(listen_pcb != NULL) {
		err_t err;
		err = tcp_bind(listen_pcb, IP_ADDR_ANY, 1182);
		if(err == ERR_OK) {		
			listen_pcb = tcp_listen(listen_pcb);
			tcp_accept(listen_pcb, server_accept);
		}
	}
}






