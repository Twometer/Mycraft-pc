#pragma once
class TcpClient
{
private:
	SOCKET mySocket = INVALID_SOCKET;
	char* sbbuf;
public:
	TcpClient();
	~TcpClient();
	void connectToServer(PCSTR host, PCSTR port);
	SOCKET getSocket();
	void close();
	char readByte(int * resultptr);
};

