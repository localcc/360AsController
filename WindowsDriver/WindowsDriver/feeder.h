#pragma once
#include "client.h"
#include "ViGEm/Client.h"
#include <stdexcept>
#include <thread>
#include <chrono>

class feeder {
public:
	feeder(const char* hostname, int port);
	~feeder();

	int connect();
	void disconnect();
	void start_feeder_thread();
	void stop_feeder_thread();
	static void controller_callback(PVIGEM_CLIENT Client,
		PVIGEM_TARGET Target,
		UCHAR LargeMotor,
		UCHAR SmallMotor,
		UCHAR LedNumber,
		LPVOID Instance);

private:
	UCHAR left_motor;
	UCHAR right_motor;

	void feed();
	PVIGEM_CLIENT driver_client;
	PVIGEM_TARGET driver_target;
	std::thread sender_thread;
	client udp_client;
	bool connected;
};