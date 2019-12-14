#include "server.h"
#include "controller.h"

void conn_err(void *arg, err_t err) {
	printf("error!\n");
}

void close_conn(struct tcp_pcb *pcb) {
	tcp_arg(pcb, NULL);
	tcp_sent(pcb, NULL);
	tcp_recv(pcb, NULL);
	tcp_poll(pcb, NULL, 0);
	tcp_err(pcb, NULL);
	tcp_close(pcb);
}


void reply_controller_data(struct tcp_pcb* pcb) {
	//TODO: actually reply something!
	unsigned char* buffer = (unsigned char*)malloc(14); // 14 bytes for controller data, see controller.c for more info
	read_controller_data(buffer);
	tcp_write(pcb, buffer, 14, 0);
	tcp_sent(pcb, NULL); // no cb
}

void accept_controller_data(unsigned char* buffer) {

	write_controller_data(buffer);
}

err_t server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err) {

	if(err == ERR_OK && p != NULL){
		//Inform about data being taken
		tcp_recved(pcb, p->tot_len);
	
		unsigned char command = ((unsigned char*)p->payload)[0];
		if(command == 1) {
			reply_controller_data(pcb);
		}else if(command == 2) {
			close_conn(pcb);
		}else if(command == 3) {
			accept_controller_data((unsigned char*)p->payload + 1);
		}
		pbuf_free(p);

		tcp_sent(pcb, NULL);

	}else if(err == ERR_OK && p == NULL){
		close_conn(pcb);
	}else {
		return err;
	}
	return ERR_OK;

}

err_t server_accept(void *arg, struct tcp_pcb *pcb, err_t err) {		
	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);
	tcp_setprio(pcb, TCP_PRIO_MIN);

	tcp_arg(pcb, NULL);
	tcp_recv(pcb, server_recv);
	tcp_err(pcb, conn_err);
	tcp_poll(pcb, NULL, 0); // no polling
	return ERR_OK;
}

void start_server(void) {

	listen_pcb = tcp_new();
	tcp_bind(listen_pcb, IP_ADDR_ANY, 1182);

	listen_pcb = tcp_listen(listen_pcb);
	tcp_accept(listen_pcb, server_accept);
}






