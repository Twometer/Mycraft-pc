#include "stdafx.h"
#include "MinecraftSocket.h"
#include "McBuffer.h"
#include <iostream>
#include "IPacket.h"
#include "C00PacketHandshake.h"
#include "C00PacketLogin.h"
#include "C00PacketKeepAlive.h"
#include "C01PacketChat.h"
#include "TcpClient.h"
#include "ZlibUtils.h"
#include <sstream>
#include "OpenGLRenderer.h"
#include "Chunk.h"
#include <string>
#include "ChatParser.h"
#include "Position.h"
#include "C16Respawn.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 65565


MinecraftSocket* MinecraftSocket::instance;
bool MinecraftSocket::connected;


using namespace std;

int compressionThreshold;

MinecraftSocket::MinecraftSocket()
{
	instance = this;
}


MinecraftSocket::~MinecraftSocket()
{
}

void MinecraftSocket::connectToServer(const char* username, const char* hostname, unsigned short port) {
	cout << "Connecting ..." << endl;

	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	int iResult;

	TcpClient client;
	client.connectToServer(hostname, to_string(port).c_str());
	ConnectSocket = client.getSocket();

	if (ConnectSocket == INVALID_SOCKET)
		return;

	sendPacket(new C00PacketHandshake(47, hostname, port, 2));
	sendPacket(new C00PacketLogin(username));

	int packetLen;
	int packetId;
	int sizeUncompressed;
	char* new_ = nullptr;
	bool isLoginMode = true;
	do {
		if (new_ != nullptr) {
			delete[] new_;
			new_ = nullptr;
		}
		packetLen = readVarIntRaw(&client, &iResult);
		if (packetLen > 0) {
			iResult = receive(ConnectSocket, recvbuf, packetLen);
			if (iResult > 0) {
				McBuffer* buf = new McBuffer(recvbuf, packetLen);
				if (compressionThreshold > 0) {
					sizeUncompressed = buf->readVarInt();
					if (sizeUncompressed != 0) {
						int oldLen;
						char* old = buf->readToEnd(&oldLen);
						new_ = ZlibUtils::decompress(old, oldLen, sizeUncompressed);
						buf->replaceBuffer(new_, sizeUncompressed);
					}
				}
				packetId = buf->readVarInt();

				if (isLoginMode) {
					if (compressionThreshold == 0 && packetId == 3) {
						compressionThreshold = buf->readVarInt();
						printf("Set compression threshold to %d\n", compressionThreshold);
					}
					if (packetId == 2) {
						cout << "Login completed." << endl;
						isLoginMode = false;
						connected = true;
					}
				}
				else {
					if (packetId == 0) {
						sendPacket(new C00PacketKeepAlive(buf->readVarInt()));
					}
					if (packetId == 2) {
						int* len = 0;
						string* str = new string(buf->readString(len));
						//cout << "[Chat Message] " << *str << endl;
						OpenGLRenderer::chatMessages->push_back(ChatParser::parseJson(str));
					}
					if (packetId == 0x40) {
						int* len = 0;
						char* str = buf->readString(len);
						cout << "[Kicked from Server] " << str << endl;
						delete[] str;
					}
					if (packetId == 0x06) {
						float health = buf->readFloat();
						if (health <= 0.1) {
							sendPacket(new C16Respawn());
						}
					}
					if (packetId == 0x08) {
						double x = buf->readDouble();
						double y = buf->readDouble();
						double z = buf->readDouble();

						float yaw = buf->readFloat();
						float pitch = buf->readFloat();
						char flags = buf->readByte();
						OpenGLRenderer::controls->setPosition(x, y, z);
					}
					if (packetId == 0x23) {
						uint64_t pos = buf->readUlong();
						int id = buf->readVarInt() >> 4;
						POSITION position = POSITION(pos);
						OpenGLRenderer::world->setBlock(position.x, position.y, position.z, id);
					}
					if (packetId == 0x26) {
						bool skylight = buf->readByte() == 0x01;
						int colCount = buf->readVarInt();
						int* xCoords = new int[colCount];
						int* zCoords = new int[colCount];
						ChunkExtracted** extr = new ChunkExtracted*[colCount];

						for (int i = 0; i < colCount; i++) {
							int xC = buf->readInt();
							int zC = buf->readInt();
							short bitmask = buf->readShort();
							*(xCoords + i) = xC;
							*(zCoords + i) = zC;
							ChunkExtracted * ce = new ChunkExtracted();
							ce->bitmask = bitmask;
							int bc = countSetBits(bitmask);
							ce->storageLen = GetChunkPacketSize(bc, skylight, true);
							ce->storage = new char[ce->storageLen];
							*(extr + i) = ce;
						}

						for (int i = 0; i < colCount; i++) {
							ChunkExtracted* chk = *(extr + i);
							buf->readArray(chk->storage, chk->storageLen);
						}
						for (int i = 0; i < colCount; i++) {
							Chunk* chk = new Chunk(*(xCoords + i), *(zCoords + i));
							ChunkExtracted* e = *(extr + i);
							chk->initialize(e);
							delete[] e->storage;
							delete e;
							OpenGLRenderer::world->addChunk(chk);
						}

						delete[] extr;
					}
				}
				delete buf;
			}
			else if (iResult == 0)
				printf("Connection closed\n");
			else
				printf("recv failed with error: %d\n", WSAGetLastError());
		}
	} while (iResult > 0 && !closeRequested);

	client.close();
}

int MinecraftSocket::GetChunkPacketSize(int length, bool skyLight, bool continuous)
{
	int var3 = length * 2 * 16 * 16 * 16;
	int var4 = length * 16 * 16 * 16 / 2;
	int var5 = skyLight ? length * 16 * 16 * 16 / 2 : 0;
	int var6 = continuous ? 256 : 0;
	return var3 + var4 + var5 + var6;
}

unsigned int MinecraftSocket::countSetBits(unsigned int n)
{
	unsigned int count = 0;
	while (n)
	{
		count += n & 1;
		n >>= 1;
	}
	return count;
}


int MinecraftSocket::receive(SOCKET socket, char* buf, int len) {
	int read = 0;
	int c = 0;
	while (read < len) {
		c = recv(socket, buf + read, len - read, 0);
		if (c < 0)
			return c;
		read += c;
	}
	return read > 0 ? read : -1;
}

int MinecraftSocket::sendPacket(IPacket* packet) {
	McBuffer* buffer = new McBuffer();
	packet->write(buffer);
	int iResult = sendPacketBuffer(packet->getId(), buffer);
	delete packet;
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return iResult;
	}
	return iResult;
}

int MinecraftSocket::sendPacketBuffer(int pid, McBuffer* buffer) {
	char* packetContent = buffer->getBytes();
	int packetIdViLen = 0;
	char* packetIdVi = createVarInt(pid, &packetIdViLen);
	int packetBodyLen = 0;
	char* packetBody = concatArrays(packetIdVi, packetIdViLen, packetContent, buffer->getAllocated(), &packetBodyLen);

	if (compressionThreshold > 0) {
		// TODO: What if it's not smaller than the compressionThreshold?
		if (packetBodyLen <= compressionThreshold) {
			int len;
			char* uncompressedLen = createVarInt(0, &len);
			int nlen;
			packetBody = concatArrays(uncompressedLen, len, packetBody, packetBodyLen, &nlen);
			packetBodyLen = nlen;
		}
	}

	int packetHeaderLen = 0;
	char* packetHeader = createVarInt(packetBodyLen, &packetHeaderLen);
	int fullPacketLen = 0;
	char* fullPacket = concatArrays(packetHeader, packetHeaderLen, packetBody, packetBodyLen, &fullPacketLen);
	int result = send(ConnectSocket, fullPacket, fullPacketLen, 0);
	delete[] fullPacket;
	delete buffer;
	return result;
}

char* MinecraftSocket::concatArrays(char* a, int alen, char* b, int blen, int* resultLen) {
	int t = alen + blen;
	char* result = new char[t];
	memcpy(result, a, alen);
	memcpy(result + alen, b, blen);
	delete[] a;
	delete[] b;
	*(resultLen) = t;
	return result;
}

int MinecraftSocket::readVarIntRaw(TcpClient* socket, int* resultptr) {
	int i = 0;
	int j = 0;
	int k = 0;
	while (true)
	{
		k = (int)((unsigned char)socket->readByte(resultptr));
		i |= (k & 0x7F) << j++ * 7;
		if (j > 5) return 0;
		if ((k & 0x80) != 128) break;
	}
	return i;
}

char* MinecraftSocket::createVarInt(int i, int* len) {
	char* c = new char[5];
	int allocated = 0;
	do {
		char chr = (char)(i & 0b01111111);
		i = ((unsigned int)i) >> 7;
		if (i != 0) {
			chr |= 0b10000000;
		}
		*(c + (allocated++)) = chr;
	} while (i != 0);
	*len = allocated;
	char* r = new char[allocated];
	memcpy(r, c, allocated);
	delete[] c;
	return r;
}

void MinecraftSocket::disconnect() {
	closeRequested = true;
}