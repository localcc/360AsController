#pragma once
#include "client.h"
#include "ViGEm/Client.h"
#include <thread>

class feeder {
public:
	feeder(const char* hostname, const char* port);
	~feeder();

	int connect();
	void disconnect();
	void start_feeder_thread();
	void stop_feeder_thread();

private:

	void feed();
	client* tcp_client;
	PVIGEM_CLIENT driver_client;
	PVIGEM_TARGET driver_target;
	std::thread sender_thread;
	bool connected;
};