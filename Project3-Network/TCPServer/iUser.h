#ifndef _iUser_HG_
#define _iUser_HG_

#undef UNICODE
// exclude many header files of windows.h
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <string>
#include <cBuffer.h>

struct sUserInfo
{
	sUserInfo() : socket(-1), connectTime(0) {}
	SOCKET socket;
	std::string email;
	DWORD connectTime;
};
class iUser
{
public:
	virtual ~iUser() {}

	// begin of iUser
	virtual bool Compaire(iUser* user) = 0;
	virtual SOCKET SocketID() = 0;
	virtual bool IsLoggedIn() = 0;
	virtual DWORD ConnectionTime() = 0;
	virtual const std::string& Email() = 0;
	virtual cBuffer& GetReceiveBuffer() = 0;
	virtual void AssignLoginInfo(const sProtocolAccount& loginInfo) = 0;
	virtual void Logout() = 0;
	// end of iUser

};

#endif