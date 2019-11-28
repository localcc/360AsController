
#include "client.h"

class feeder {
public:
	feeder();
	~feeder();

	int connect(const char* hostname, const char* port);
	void disconnect();

private:
	client* client;

};