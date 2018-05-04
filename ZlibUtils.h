#pragma once
class ZlibUtils
{
public:
	ZlibUtils();
	~ZlibUtils();
	static char* decompress(char* toDecompress, int toDecompressLen, int sizeUncompressed);
private:
	static int UncompressData(const BYTE* abSrc, int nLenSrc, BYTE* abDst, int nLenDst);
};

