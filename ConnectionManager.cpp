#include "stdafx.h"
#include "ConnectionManager.h"
#include "MinecraftSocket.h"
#include <thread>

using namespace std;

const char* ConnectionManager::ip;
const char* ConnectionManager::name;
unsigned short ConnectionManager::port;

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
	thread* t = new thread(&network_thread);
}