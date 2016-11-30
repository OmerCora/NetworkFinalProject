#include "cUserManager.h"

#include <cErrorReport.h>
#include <iostream>

#include "cUser.h"

#include "cRoomManager.h"

cUserManager::cUserManager()
{
}


cUserManager::~cUserManager()
{
	{
		std::map<SOCKET, iUser*>::iterator iter = m_users.begin();
		for (; iter != m_users.end(); ++iter)
		{
			iUser* p = iter->second;
			delete p;
		}
		m_users.clear();
	}
}

cUserManager* cUserManager::m_instance = 0;
cUserManager& cUserManager::GetInstance()
{
	if (m_instance == 0)
		m_instance = new cUserManager();
	return *m_instance;
}
void cUserManager::DeleteInstance()
{
	if (m_instance)
		delete m_instance;
	m_instance = 0;
}


iUser* cUserManager::CreateNewUser(const sUserInfo& userInfo)
{
	iUser* user = this->GetUser(userInfo.socket);
	if (user == 0)
	{
		user = new cUser(userInfo);
		m_users[userInfo.socket] = (user);
		return user;
	}

	return 0;
}

void cUserManager::DeleteUser(SOCKET socketID)
{

	std::map<SOCKET, iUser*>::iterator iter = m_users.find(socketID);
	if (iter != m_users.end())
	{
		iUser* p = iter->second;
		iRoom* room = cRoomManager::GetInstance().GetRoomByUser(p);
		if (room)
			room->Leave(p);

		cRoomManager::GetInstance().GetLobby()->LeaveLobby(p);
		delete p;
		m_users.erase(iter);
	}
}

iUser* cUserManager::GetUser(SOCKET socketID)
{
	std::map<SOCKET, iUser*>::iterator iter = m_users.find(socketID);
	if (iter != m_users.end())
		return iter->second;
	return 0;
}
bool cUserManager::UserExist(const std::string& email)
{
	std::map<SOCKET, iUser*>::iterator iter = m_users.begin();
	for (; iter != m_users.end(); ++iter)
	{
		iUser* user = iter->second;
		if (user->Email() == email)
			return true;
	}
	return false;
}

void cUserManager::CollectLoggedInUsers(std::set<SOCKET>& outSet)
{

	std::map<SOCKET, iUser*>::iterator iter = m_users.begin();
	for (; iter != m_users.end(); ++iter)
	{
		iUser* user = iter->second;
		if (user->IsLoggedIn())
		{
			outSet.insert(user->SocketID());
		}
	}
}
