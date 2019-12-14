#include "controller.h"
#include <usb/usbmain.h>
#include <stdio.h>
#include <string.h>


void serialize_controller_data(struct controller_data_s controller_data, unsigned char* data_to_send) {
	//Assuming 14 bytes were allocated
	//Zeroing out
	memset(data_to_send, 0, 14);
	
	data_to_send[0] = ((controller_data.a) |
			    (controller_data.x << GB_OFF_X) |
			    (controller_data.b << GB_OFF_B) |
			    (controller_data.y << GB_OFF_Y) |
			    (controller_data.lb << GB_OFF_LB) |
			    (controller_data.rb << GB_OFF_RB) |
			    (controller_data.up << GB_OFF_UP) |
			    (controller_data.down << GB_OFF_DOWN));
	data_to_send[1] = ((controller_data.left) |
			    (controller_data.right << GB_OFF_RIGHT) |
			    (controller_data.s1_z << GB_OFF_S1_Z) |
			    (controller_data.s2_z << GB_OFF_S2_Z) |
			    (controller_data.start << GB_OFF_START) |
			    (controller_data.back << GB_OFF_BACK) |
			    (controller_data.logo << GB_OFF_LOGO) |
			    (0 << 7));
	//Triggers
	data_to_send[2] = controller_data.lt & 0xFF;
	data_to_send[3] = controller_data.lt >> 8;
	data_to_send[4] = controller_data.rt & 0xFF;
	data_to_send[5] = controller_data.rt >> 8;
	//Sticks
	data_to_send[6] = controller_data.s1_x & 0xFF; //x
	data_to_send[7] = controller_data.s1_x >> 8;
	data_to_send[8] = controller_data.s1_y & 0xFF; // y
	data_to_send[9] = controller_data.s1_y >> 8;
	data_to_send[10] = controller_data.s2_x & 0xFF; // x
	data_to_send[11] = controller_data.s2_x >> 8;
	data_to_send[12] = controller_data.s2_y & 0xFF; // y
	data_to_send[13] = controller_data.s2_y >> 8;
}




void read_controller_data(unsigned char* data_to_send) {
	//Assuming data_to_send was allocated 14 bytes long
	struct controller_data_s cur_controller_data;
	if(get_controller_data(&cur_controller_data, 0) > -1) {
		serialize_controller_data(cur_controller_data, data_to_send);
		prev_controller_data = cur_controller_data;

	}
	usb_do_poll();
}

void write_controller_data(unsigned char* data) {
	set_controller_rumble(0, data[0], data[1]);	
}
