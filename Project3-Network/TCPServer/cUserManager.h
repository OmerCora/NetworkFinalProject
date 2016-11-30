#ifndef _cUserManager_HG_
#define _cUserManager_HG_

#undef UNICODE
// exclude many header files of windows.h
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <map>
#include <set>

#include "iUser.h"

class cUserManager
{
	cUserManager();
	~cUserManager();
	static cUserManager* m_instance;
public:
	static cUserManager& GetInstance();
	static void DeleteInstance();

	iUser* CreateNewUser(const sUserInfo& userInfo);
	void DeleteUser(SOCKET socketID);

	iUser* GetUser(SOCKET socketID);
	bool UserExist(const std::string& email);
	void CollectLoggedInUsers(std::set<SOCKET>& outSet);

private:

	std::map<SOCKET, iUser*> m_users;
};

#endif