#pragma once
class McBuffer
{
private:
	char *i;
	int allocated;
	int inputSize;
	int readOffset;
public:
	McBuffer();
	McBuffer(char* inputBuffer, int inputSize);
	~McBuffer();
	char* getBytes();
	char* readToEnd(int* len);
	char* readString(int* len);
	int readInt();
	uint64_t readUlong();
	double readDouble();
	float readFloat();
	short readShort();
	void readArray(char* target, int len);
	int getAllocated();
	void replaceBuffer(char* newBuffer, int newLen);
	void putBytes(char* array, int len);
	void putVarInt(int vi);
	void putUshort(unsigned short ushort);
	void putString(const char* string);
	char readByte();
	int readVarInt();
};

