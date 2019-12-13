#include "controller_conversion.h"
#include "controller_offsets.h"
#include <cstdio>

XUSB_REPORT controller_conversion::data_to_report(char* data) {
	XUSB_REPORT report;
	
	report.wButtons = 0;
	if (data[0] & 0b00000001) report.wButtons |= XUSB_GAMEPAD_A;
	if ((data[0] >> GB_OFF_X) & 0b00000001) report.wButtons |= XUSB_GAMEPAD_X;
	if ((data[0] >> GB_OFF_B) & 0b00000001) report.wButtons |= XUSB_GAMEPAD_B;
	if ((data[0] >> GB_OFF_Y) & 0b00000001) report.wButtons |= XUSB_GAMEPAD_Y;
	if ((data[0] >> GB_OFF_LB) & 0b00000001) report.wButtons |= XUSB_GAMEPAD_LEFT_SHOULDER;
	if ((data[0] >> GB_OFF_RB) & 0b00000001) report.wButtons |= XUSB_GAMEPAD_RIGHT_SHOULDER;
	if ((data[0] >> GB_OFF_UP) & 0b00000001) report.wButtons |= XUSB_GAMEPAD_DPAD_UP;
	if ((data[0] >> GB_OFF_DOWN) & 0b00000001) report.wButtons |= XUSB_GAMEPAD_DPAD_DOWN;
	if (data[1] & 0b00000001) report.wButtons |= XUSB_GAMEPAD_DPAD_LEFT;
	if ((data[1] >> GB_OFF_RIGHT) & 0b00000001) report.wButtons |= XUSB_GAMEPAD_DPAD_RIGHT;
	if ((data[1] >> GB_OFF_S1_Z) & 0b00000001) report.wButtons |= XUSB_GAMEPAD_LEFT_THUMB;
	if ((data[1] >> GB_OFF_S2_Z) & 0b00000001) report.wButtons |= XUSB_GAMEPAD_RIGHT_THUMB;
	if ((data[1] >> GB_OFF_START) & 0b00000001) report.wButtons |= XUSB_GAMEPAD_START;
	if ((data[1] >> GB_OFF_BACK) & 0b00000001) report.wButtons |= XUSB_GAMEPAD_BACK;
	if ((data[1] >> GB_OFF_LOGO) & 0b00000001) report.wButtons |= XUSB_GAMEPAD_GUIDE;

	report.bLeftTrigger = data[2] | (data[3] << 8);
	report.bRightTrigger = data[4] | (data[5] << 8);

	
	report.sThumbLX = (data[7] << 8) | (0x00ff & data[6]);
	report.sThumbLY = (data[9] << 8) | (0x00ff & data[8]);

	report.sThumbRX = (data[11] << 8) | (0x00ff & data[10]);
	report.sThumbRY = (data[13] << 8) | (0x00ff & data[12]);



	return report;
}