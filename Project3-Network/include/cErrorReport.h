#ifndef _cErrorReport_HG_
#define _cErrorReport_HG_

#undef UNICODE
// exclude many header files of windows.h
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

class cErrorReport
{
	cErrorReport();
	~cErrorReport();
public:
	static cErrorReport& GetInstance();

	void PrintWSAGetLastMessage(const char* message);

};

#endif