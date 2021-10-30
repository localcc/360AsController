#pragma once
#include "ViGEm/Common.h"
#include <cstdint>

class controller_conversion {
public:
	static XUSB_REPORT data_to_report(uint8_t* data);

};