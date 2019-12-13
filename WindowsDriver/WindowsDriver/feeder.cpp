#include "feeder.h"
#include "controller_conversion.h"

feeder::feeder(const char* hostname, const char* port) {
	this->tcp_client = new client(hostname, port);
	this->driver_client = vigem_alloc();
	this->driver_target = vigem_target_x360_alloc();
	connected = false;
}

feeder::~feeder() {
	this->tcp_client->disconnect();
	connected = false;
	this->tcp_client->~client();
	vigem_target_remove(this->driver_client, this->driver_target);
	vigem_disconnect(this->driver_client);
	vigem_target_free(this->driver_target);
	vigem_free(this->driver_client);
	
}

int feeder::connect() {
	int res = this->tcp_client->_connect();
	if (res != 0) return res;

	if (vigem_connect(this->driver_client) != VIGEM_ERROR_NONE) return -80;
	if (vigem_target_add(this->driver_client, this->driver_target) != VIGEM_ERROR_NONE) return -81;
	connected = true;
	return 0;
}

void feeder::start_feeder_thread() {
	this->sender_thread = std::thread(&feeder::feed, this);
	this->sender_thread.detach();
}

void feeder::stop_feeder_thread() {
	connected = false;
	this->sender_thread.join();
}

void feeder::feed() {
	char command[1] = { 1 }; // this is get command
	char* buf = (char*)malloc(14); // buffer for data
	while (connected) {
		//Do work
		this->tcp_client->_write(command, 1); // asking for controller data
		memset(buf, 0, 14);
		this->tcp_client->_read(buf, 14); // receiving it
		
		vigem_target_x360_update(this->driver_client, this->driver_target, controller_conversion::data_to_report(buf));
	}
	command[0] = 2; // shutdown
	this->tcp_client->_write(command, 1);
}

void feeder::disconnect() {
	this->tcp_client->disconnect();
}

