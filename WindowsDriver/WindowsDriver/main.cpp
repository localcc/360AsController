#include <cstdio>
#include "client.h"

int main() {
	client* cl = new client("192.168.1.158", "1182");
	int res = cl->_connect();
	printf("Res: %d\n", res);
	printf("Hello world!\n");
	
	return 0;
	
}