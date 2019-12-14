#pragma once
#include "client.h"
#include "ViGEm/Client.h"
#include <thread>

class feeder {
public:
	feeder(const char* hostname, const char* port);
	~feeder();

	static feeder* GetInstance() { return c_instance; }

	int connect();
	void disconnect();
	void start_feeder_thread();
	void stop_feeder_thread();
	static void controller_callback(PVIGEM_CLIENT Client,
		PVIGEM_TARGET Target,
		UCHAR LargeMotor,
		UCHAR SmallMotor,
		UCHAR LedNumber);

private:

	void feed();
	static feeder* c_instance;
	PVIGEM_CLIENT driver_client;
	PVIGEM_TARGET driver_target;
	std::thread sender_thread;
	client* tcp_client;
	bool connected;
};