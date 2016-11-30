#include "cLobby.h"

#include <iostream>
#include <cErrorReport.h>

cLobby::cLobby()
{
}


cLobby::~cLobby()
{
}

bool cLobby::JoinLobby(iUser* user)
{
	for (unsigned int i = 0; i < m_users.size(); ++i)
	{
		if (m_users[i]->Compaire(user))
		{
			std::cout << "\tERROR: already joined user socket: " << user->SocketID() << std::endl;
			return false;
		}
	}
	m_users.push_back(user);
	return true;
}
bool cLobby::LeaveLobby(iUser* user)
{
	for (unsigned int i = 0; i < m_users.size(); ++i)
	{
		if (m_users[i] == user)
		{
			m_users.erase(m_users.begin() + i);
			break;
		}
	}

	return true;
}

void cLobby::GetLobbyUsers(sProtocolLobbyUsers& lobbyUsers)
{
	std::cout << "GetLobbyUsers: " << m_users.size() << std::endl;

	lobbyUsers.numberOfUsers = (short)m_users.size();
	lobbyUsers.users.clear();
	for (int i = 0; i < (int)m_users.size(); ++i)
	{
		sProtocolUser user;
		user.username = m_users[i]->Email();
		user.username_length = (short)user.username.size();
		lobbyUsers.users.push_back(user);
	}
}


void cLobby::CollectLobbyUsers(std::set<SOCKET>& outSet)
{
	for (unsigned int i = 0; i < m_users.size(); ++i)
	{
		iUser* user = m_users[i];
		outSet.insert(user->SocketID());
	}
}
