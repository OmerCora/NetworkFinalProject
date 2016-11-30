#ifndef _iLobby_HG_
#define _iLobby_HG_

#include "iUser.h"
#include <sProtocols.h>
#include <set>

class iLobby
{
public:
	virtual ~iLobby() {}

	virtual bool JoinLobby(iUser* user) = 0;
	virtual bool LeaveLobby(iUser* user) = 0;
	virtual void GetLobbyUsers(sProtocolLobbyUsers& lobbyUsers) = 0;
	virtual void CollectLobbyUsers(std::set<SOCKET>& outSet) = 0;

};

#endif