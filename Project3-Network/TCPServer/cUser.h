#ifndef _cLobby_HG_
#define _cLobby_HG_


#include <string>
#include <vector>

#undef UNICODE
// exclude many header files of windows.h
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "iUser.h"

class cUser : public iUser
{
public:
	cUser(const sUserInfo& userInfo);
	~cUser();

	// begin of iUser
	virtual bool Compare(iUser* user);
	virtual SOCKET SocketID();
	virtual bool IsLoggedIn();
	virtual DWORD ConnectionTime();
	virtual const std::string& Email();
	virtual cBuffer& GetReceiveBuffer();
	virtual void AssignLoginInfo(const sProtocolAccount& loginInfo);
	virtual void Logout();
	// end of iUser

private:
	cBuffer m_receiveBuffer;
	//cPacketReceiver m_receiver;

	bool m_loggedIn;
	sUserInfo m_userInfo;
};

#endif