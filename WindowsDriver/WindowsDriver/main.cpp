#include <iostream>
#include "client.h"
#include "feeder.h"


int main(int argc, char* argv[]) {
	
	if (argc < 2) {
		return -1;
	}
	feeder* f = new feeder(argv[1], 1182);
	int res = f->connect();
	if (res == 1) {
		std::cout << "WARNING: Failed to setup rumble callback, you will NOT have rumble" << std::endl;
	}
	std::cout << res << std::endl;
	f->start_feeder_thread();
	//Keeping it open
	while (true) {}

	return 0;

}
