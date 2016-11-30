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
#include "iLobby.h"
#include <cBuffer.h>
#include <set>

class cLobby : public iLobby
{
public:
	cLobby();
	~cLobby();

	virtual bool JoinLobby(iUser* user);
	virtual bool LeaveLobby(iUser* user);
	virtual void CollectLobbyUsers(std::set<SOCKET>& outSet);

	virtual void GetLobbyUsers(sProtocolLobbyUsers& lobbyUsers);
private:
	std::vector<iUser*> m_users;
};

#endif