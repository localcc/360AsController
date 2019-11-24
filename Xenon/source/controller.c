#include "controller.h"


bool compare_controller_data(struct controller_data_s f, struct controller_data_s s) {
	return f.s1_x == s.s1_x && f.s1_y == s.s1_y && 
	       f.s2_x == s.s2_x && f.s2_y == s.s2_y &&
	       f.s1_z == s.s1_z && f.s2_z == f.s2_z &&
	       f.lb == s.lb && f.rb == s.rb && 
	       f.start == s.start && f.back == s.back &&
	       f.a == s.a && f.b == s.b && f.x == s.x && f.y == s.y &&
	       f.up == s.up && f.down == s.down && f.left == s.left && f.right == s.right &&
	       f.rt == s.rt && f.lt == s.lt &&
	       f.logo == s.logo;
}

void serialize_controller_data(struct controller_data_s controller_data, unsigned char* data_to_send) {
	//Assuming 14 bytes were allocated
	//Zeroing out
	for(char i = 0; i < 14; i++){
		data_to_send[i] = 0;
	}
	data_to_send[0] |= ((controller_data.a << GAMEPAD_OFFSET_A) |
			    (controller_data.x << GAMEPAD_OFFSET_X) |
			    (controller_data.b << GAMEPAD_OFFSET_B) |
			    (controller_data.y << GAMEPAD_OFFSET_Y) |
			    (controller_data.lb << GAMEPAD_OFFSET_LB) |
			    (controller_data.rb << GAMEPAD_OFFSET_RB) |
			    (controller_data.up << GAMEPAD_OFFSET_UP) |
			    (controller_data.down << GAMEPAD_OFFSET_DOWN));
	data_to_send[1] |= ((controller_data.left << GAMEPAD_OFFSET_LEFT) |
			    (controller_data.right << GAMEPAD_OFFSET_RIGHT) |
			    (controller_data.s1_z << GAMEPAD_OFFSET_S1_Z) |
			    (controller_data.s2_z << GAMEPAD_OFFSET_S2_Z) |
			    (controller_data.start << GAMEPAD_OFFSET_START) |
			    (controller_data.back << GAMEPAD_OFFSET_BACK) |
			    (controller_data.logo << GAMEPAD_OFFSET_LOGO) |
			    (0 << 8));
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
		if(!compare_controller_data(cur_controller_data, prev_controller_data)) {
			serialize_controller_data(cur_controller_data, data_to_send);
			prev_controller_data = cur_controller_data;
			
		}
		usb_do_poll();
	}
}
