#include "stdafx.h"
#define ZLIB_WINAPI

#include "ZlibUtils.h"
#include "zlib.h"
#pragma comment(lib, "zlibwapi.lib")


ZlibUtils::ZlibUtils()
{
}


ZlibUtils::~ZlibUtils()
{
}

char* ZlibUtils::decompress(char* toDecompress, int toDecompressLen, int sizeUncompressed) {
	char* out = new char[sizeUncompressed];
	int i = UncompressData((BYTE*)toDecompress, toDecompressLen, (BYTE*)out, sizeUncompressed);
	delete[] toDecompress;
	return out;
}

int ZlibUtils::UncompressData(const BYTE* abSrc, int nLenSrc, BYTE* abDst, int nLenDst)
{
	z_stream zInfo = { 0 };
	zInfo.total_in = zInfo.avail_in = nLenSrc;
	zInfo.total_out = zInfo.avail_out = nLenDst;
	zInfo.next_in = (BYTE*)abSrc;
	zInfo.next_out = abDst;

	int nErr, nRet = -1;
	nErr = inflateInit(&zInfo);               // zlib function
	if (nErr == Z_OK) {
		nErr = inflate(&zInfo, Z_FINISH);     // zlib function
		if (nErr == Z_STREAM_END) {
			nRet = zInfo.total_out;
		}
	}
	inflateEnd(&zInfo);   // zlib function
	return(nRet); // -1 or len of output
}
