#include <input/input.h>
#include <stdbool.h>

#ifndef _C360_H_
#define _C360_H_

#define GB_OFF_X 1
#define GB_OFF_B 2
#define GB_OFF_Y 3
#define GB_OFF_LB 4
#define GB_OFF_RB 5
#define GB_OFF_UP 6
#define GB_OFF_DOWN 7
#define GB_OFF_RIGHT 1
#define GB_OFF_S1_Z 2
#define GB_OFF_S2_Z 3
#define GB_OFF_START 4
#define GB_OFF_BACK 5
#define GB_OFF_LOGO 6

struct controller_data_s prev_controller_data;
void serialize_controller_data(struct controller_data_s controller_data, unsigned char* data_to_send);
void read_controller_data(unsigned char* data_to_send);

#endif

