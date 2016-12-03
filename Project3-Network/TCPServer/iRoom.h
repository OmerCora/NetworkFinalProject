#ifndef _iRoom_HG_
#define _iRoom_HG_

#undef UNICODE
// exclude many header files of windows.h
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <string>
#include <set>

#include "iUser.h"
#include <cBuffer.h>
#include <sProtocols.h>

class iRoom
{
public:
	virtual ~iRoom() {}

	virtual const std::string& Name() = 0;
	virtual bool CheckUser(iUser* user) = 0;
	virtual short GetNumUsers() = 0;

	virtual bool Join(iUser* user, const std::string& username) = 0;
	virtual void Leave(iUser* user) = 0;

	virtual bool BroadcastData() = 0;

	virtual void AddChat(const std::string& username, const std::string& message) = 0;

	virtual void ProcessReceivedPlayData(cBuffer& receiveBuffer) = 0;
	virtual void UpdateGameLoop() = 0;

};


#endif