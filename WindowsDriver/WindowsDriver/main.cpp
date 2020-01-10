#include <cstdio>
#include "client.h"
#include "feeder.h"


int main(int argc, char* argv[]) {
	/*
	if (argc < 2) {
		return -1;
	}*/
	feeder* f = new feeder("192.168.1.158", 1182);
	int res = f->connect();
	printf("%d\n", res);
	f->start_feeder_thread();
	//Keeping it open
	while (true) {}

	return 0;

}
