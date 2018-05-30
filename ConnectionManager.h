#pragma once
class ConnectionManager
{
private:
	static const char* ip;
	static const char* name;
	static unsigned short port;
	static void network_thread();
public:
	ConnectionManager();
	~ConnectionManager();
	static void connect(const char* name, const char * ip, unsigned short port);
};

