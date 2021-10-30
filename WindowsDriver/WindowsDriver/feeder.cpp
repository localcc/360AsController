#include "feeder.h"
#include "controller_conversion.h"


/*
	Commands:
	1 -		get, read 14 bytes after
	2 -		poweroff
	3 -		cb, write 2 bytes after, 1st byte - left rumble, 2nd - right rumble
*/
static uint8_t get[1] = { 1 };
static uint8_t poweroff[1] = { 2 };
static uint8_t cb = 3;
feeder::feeder(const char* hostname, int port) : udp_client(hostname, port) {
	this->driver_client = vigem_alloc();
	if (this->driver_client == nullptr) {
		throw std::runtime_error("Failed to allocate ViGEm bus driver!");
	}
	if (!VIGEM_SUCCESS(vigem_connect(this->driver_client))) {
		throw std::runtime_error("Failed to connect to ViGEm bus driver!");
	}
	this->driver_target = vigem_target_x360_alloc();
	if (this->driver_target == nullptr) {
		throw std::runtime_error("Failed to allocate driver handle!");
	}
}

feeder::~feeder() {
	this->udp_client.close_socket();
	this->connected = false;
	vigem_target_remove(this->driver_client, this->driver_target);
	vigem_disconnect(this->driver_client);
	vigem_target_free(this->driver_target);
	vigem_free(this->driver_client);
	
}

int feeder::connect() {
	if (!VIGEM_SUCCESS(vigem_target_add(this->driver_client, this->driver_target))) return -81;

	this->connected = true;
	if (!VIGEM_SUCCESS(vigem_target_x360_register_notification(this->driver_client, this->driver_target,
		reinterpret_cast<PFN_VIGEM_X360_NOTIFICATION>(&feeder::controller_callback), this))) {
		return 1;
	}
	return 0;
}

void feeder::start_feeder_thread() {
	this->sender_thread = std::thread(&feeder::feed, this);
	this->sender_thread.detach();
}

void feeder::stop_feeder_thread() {
	this->disconnect();
	this->sender_thread.join();
}

void feeder::controller_callback(PVIGEM_CLIENT Client,
	PVIGEM_TARGET Target,
	UCHAR LeftMotor,
	UCHAR RightMotor,
	UCHAR LedNumber,
	LPVOID Instance) {
	feeder* f = reinterpret_cast<feeder*>(Instance);
	if (LeftMotor == f->left_motor && RightMotor == f->right_motor) return;
	f->left_motor = LeftMotor;
	f->right_motor = RightMotor;

	uint8_t data[3] = { cb, LeftMotor, RightMotor };
	f->udp_client.client_write(data, 3);
}

void feeder::feed() {
	uint8_t* buf = new uint8_t[14];
	uint8_t data[2]{ 0 };

	while (connected) {
		this->udp_client.client_write(get, 1); // asking for controller data
		memset(buf, 0, 14);
		this->udp_client.client_read(buf, 14);;
		
		vigem_target_x360_update(this->driver_client, this->driver_target, controller_conversion::data_to_report(buf));
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}
	
	this->udp_client.client_write(poweroff, 1);
}

void feeder::disconnect() {
	this->connected = false;
	this->udp_client.close_socket();
}

