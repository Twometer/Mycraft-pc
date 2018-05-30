#pragma once
#include <thread>
class ConnectionManager
{
private:
	static const char* ip;
	static const char* name;
	static unsigned short port;
	static void network_thread();
	static std::thread* network_thread_inst;
public:
	ConnectionManager();
	~ConnectionManager();
	static void connect(const char* name, const char * ip, unsigned short port);
	static void disconnect();
};

