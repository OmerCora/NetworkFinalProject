#include "cErrorReport.h"

#include <iostream>

cErrorReport::cErrorReport()
{
}


cErrorReport::~cErrorReport()
{
}


cErrorReport& cErrorReport::GetInstance()
{
	static cErrorReport instance;
	return instance;
}



void cErrorReport::PrintWSAGetLastMessage(const char* message)
{
	DWORD errCode = GetLastError();
	LPTSTR errMessage = 0;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errMessage, 0, NULL);
	std::cout << message << errMessage << std::endl;
	LocalFree(errMessage);
}
