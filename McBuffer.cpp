#include "stdafx.h"
#include "McBuffer.h"
#include <algorithm>
#include <iostream>
#include "MinecraftSocket.h"

using namespace std;

McBuffer::McBuffer()
{
	i = new char[8192*32];
}

McBuffer::McBuffer(char* inputBuffer, int inputSize) {
	i = inputBuffer;
	this->inputSize = inputSize;
}

McBuffer::~McBuffer()
{
	//delete[] i;
}

int McBuffer::readInt() {
	int i = 0;
	int size = sizeof(i);
	char* tmp = new char[size];
	readArray(tmp, size);
	reverse(tmp, tmp + size);
	memcpy(&i, tmp, size);
	free(tmp);
	return i;
}

double McBuffer::readDouble() {
	char* tmp = new char[8];
	readArray(tmp, 8);
	std::reverse(tmp, tmp + 8);
	double dbl = 0;
	memcpy(&dbl, tmp, 8);
	free(tmp);
	return dbl;
}

float McBuffer::readFloat() {
	char* tmp = new char[4];
	readArray(tmp, 4);
	std::reverse(tmp, tmp + 4);
	float dbl = 0;
	memcpy(&dbl, tmp, 4);
	free(tmp);
	return dbl;
}

void McBuffer::readArray(char* target, int len) {
	if (readOffset + len > inputSize) {
		cout << "Buffer underflow: tried reading " << len << " but only got " << (inputSize-readOffset) << " left" << endl;
		return;
	}
	memcpy(target, i + readOffset, len);
	readOffset += len;
}

short McBuffer::readShort() {
	short s = 0;
	int size = sizeof(s);
	char* tmp = new char[size];
	readArray(tmp, size);
	reverse(tmp, tmp + size);
	memcpy(&s, tmp, size);
	free(tmp);
	return s;
}

int McBuffer::readVarInt() {
	int i = 0;
	int j = 0;
	int k = 0;
	while (true)
	{
		k = (int)((unsigned char)readByte());
		i |= (k & 0x7F) << j++ * 7;
		if (j > 5) return 0;
		if ((k & 0x80) != 128) break;
	}
	return i;
}

char* McBuffer::readString(int* len) {
	int stringlen = readVarInt();
	char* result = new char[stringlen + 1];
	*(result + stringlen) = 0;
	memcpy(result, i + readOffset, stringlen);
	readOffset += stringlen;
	return result;
}

char* McBuffer::readToEnd(int* len) {
	int l = inputSize - readOffset;
	char* result = new char[l];
	memcpy(result, i + readOffset, l);
	*len = l;
	return result;
}

char McBuffer::readByte() {
	char b = *(i + readOffset);
	readOffset++;
	return b;
}

void McBuffer::putString(const char* str) {
	int len = strlen(str);
	putVarInt(len);
	putBytes((char*)str, len);
}

void McBuffer::putUshort(unsigned short us) {
	int sz = sizeof(us);
	char* b = new char[sz];
	for (int i = 0; i < sz; i++) {
		*(b + i) = (char)*(&us + i);
	}
	putBytes(b, sz);
	delete[] b;
}

void McBuffer::putVarInt(int vi) {
	int len = 0;
	char* chr = MinecraftSocket::createVarInt(vi, &len);
	putBytes(chr, len);
	delete[] chr;
}

void McBuffer::putBytes(char* array, int len) {
	memcpy(i+allocated, array, len);
	allocated += len;
}

int McBuffer::getAllocated() {
	return allocated;
}

char* McBuffer::getBytes() {
	return &i[0];
}

void McBuffer::replaceBuffer(char* newBuffer, int newLen) {
	i = newBuffer;
	allocated = 0;
	inputSize = newLen;
	readOffset = 0;
}