#include "stdafx.h"
#include "ConnectionManager.h"
#include "MinecraftSocket.h"

using namespace std;

const char* ConnectionManager::ip;
const char* ConnectionManager::name;
unsigned short ConnectionManager::port;
thread* ConnectionManager::network_thread_inst;
ConnectionManager::ConnectionManager()
{
}


ConnectionManager::~ConnectionManager()
{
}

void ConnectionManager::network_thread()
{
	MinecraftSocket socket;
	socket.connectToServer(name, ip, port);
}

void ConnectionManager::connect(const char* name, const char * ip, unsigned short port)
{
	ConnectionManager::name = name;
	ConnectionManager::ip = ip;
	ConnectionManager::port = port;
	network_thread_inst = new thread(&network_thread);
}

void ConnectionManager::disconnect() {
	MinecraftSocket::instance->disconnect();
	network_thread_inst->join();
	MinecraftSocket::instance = nullptr;
}