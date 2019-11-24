#include <input/input.h>
#include <usb/usbmain.h>
#include <stdbool.h>

#ifndef _C360_H_
#define _C360_H_

#define GAMEPAD_OFFSET_A 1
#define GAMEPAD_OFFSET_X 2
#define GAMEPAD_OFFSET_B 3
#define GAMEPAD_OFFSET_Y 4
#define GAMEPAD_OFFSET_LB 5
#define GAMEPAD_OFFSET_RB 6
#define GAMEPAD_OFFSET_UP 7
#define GAMEPAD_OFFSET_DOWN 8
#define GAMEPAD_OFFSET_LEFT 1
#define GAMEPAD_OFFSET_RIGHT 2
#define GAMEPAD_OFFSET_S1_Z 3
#define GAMEPAD_OFFSET_S2_Z 4
#define GAMEPAD_OFFSET_START 5
#define GAMEPAD_OFFSET_BACK 6
#define GAMEPAD_OFFSET_LOGO 7

struct controller_data_s prev_controller_data;
bool compare_controller_data(struct controller_data_s f, struct controller_data_s s);
void serialize_controller_data(struct controller_data_s controller_data, unsigned char* data_to_send);
void read_controller_data(unsigned char* data_to_send);

#endif

