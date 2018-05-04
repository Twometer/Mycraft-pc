#pragma once
#include "McBuffer.h"
#include "IPacket.h"
#include "TcpClient.h"
class MinecraftSocket
{
public:
	MinecraftSocket();
	~MinecraftSocket();
	void connectToServer(const char* username, const char* hostname, unsigned short port);
	int sendPacketBuffer(SOCKET socket, int pid, McBuffer* buffer);
	int sendPacket(SOCKET socket, IPacket* packet);
	static char* createVarInt(int i, int* len);
private:
	int compressionThreshold = 0;
	char* concatArrays(char* a, int alen, char* b, int blen, int* resultLen);
	int readVarIntRaw(TcpClient* socket, int* resultptr);
	int receive(SOCKET socket, char* buf, int len);
	int GetChunkPacketSize(int length, bool skyLight, bool continuous);
	unsigned int countSetBits(unsigned int n);
};

