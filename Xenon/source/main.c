


#include <stdio.h>
#include <stdlib.h>
#include <xenos/xenos.h>
#include <console/console.h>
#include <usb/usbmain.h>
#include <network/network.h>
#include "server.h"

void mainInit() {
	network_init();
	xenos_init(VIDEO_MODE_AUTO);
	console_init();
	
	usb_init();
	usb_do_poll();
}
int main() {
	mainInit();
	start_server();
	printf("Waiting for clients\n");
 	for(;;) {
		network_poll();
		usb_do_poll();
	}
	return 0;


}
