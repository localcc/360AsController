#include "feeder.h"
#include "controller_conversion.h"



static char get[1] = { 1 };
static char poweroff[1] = { 2 };
static char cb[1] = { 3 };

feeder* feeder::c_instance = nullptr;

feeder::feeder(const char* hostname, int port) {
	this->udp_client = new client(hostname, port);
	this->driver_client = vigem_alloc();
	this->driver_target = vigem_target_x360_alloc();
	connected = false;
	c_instance = this;
}

feeder::~feeder() {
	this->udp_client->close_socket();
	connected = false;
	this->udp_client->~client();
	vigem_target_remove(this->driver_client, this->driver_target);
	vigem_disconnect(this->driver_client);
	vigem_target_free(this->driver_target);
	vigem_free(this->driver_client);
	
}

int feeder::connect() {

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

void feeder::controller_callback(PVIGEM_CLIENT Client,
	PVIGEM_TARGET Target,
	UCHAR LeftMotor,
	UCHAR RightMotor,
	UCHAR LedNumber) {
	char data[3] = { cb[0], LeftMotor, RightMotor };
	feeder::GetInstance()->udp_client->client_write(data, 3);
}




/*
	Commands:
	1 -		get, read 14 bytes after
	2 -		poweroff
	3 -		cb, write 2 bytes after, 1st byte - left rumble, 2nd - right rumble
*/


void feeder::feed() {
	
	char* buf = (char*)malloc(14); // buffer for data
	char data[2] = { 0, 0 };
	while (connected) {
		
		//Do work
		this->udp_client->client_write(get, 1); // asking for controller data
		memset(buf, 0, 14);
		this->udp_client->client_read(buf, 14); // receiving it
		vigem_target_x360_register_notification(this->driver_client, this->driver_target,
			reinterpret_cast<PFN_VIGEM_X360_NOTIFICATION>(&feeder::controller_callback));
		
		vigem_target_x360_update(this->driver_client, this->driver_target, controller_conversion::data_to_report(buf));
		
	}
	
	this->udp_client->client_write(poweroff, 1);
}

void feeder::disconnect() {
	this->udp_client->close_socket();
}

