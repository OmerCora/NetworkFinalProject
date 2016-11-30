#include "cRoom.h"

#include "cErrorReport.h"

#include <iostream>
#

cRoom::cRoom(const std::string& name)
	: m_roomname(name)
{
	m_users.reserve(m_MAXIMUM_USERS);
	m_chatHistory.reserve(m_MAXIMUM_USERS);
	m_chatHistoryBuff.chatHistory.reserve(m_MAXIMUM_USERS);
	m_roomInfoBuff.users.reserve(m_MAXIMUM_USERS);
}


cRoom::~cRoom()
{
}

const std::string& cRoom::Name()
{
	return m_roomname;
}

short cRoom::GetNumUsers()
{
	return (short)m_users.size();
}


void cRoom::InsertBroadcastPacketID(sProtocolHeader::ePacketID packetID)
{
	m_broadcastPacketIDs.insert(packetID);
}
bool cRoom::BroadcastData()
{
	std::set<sProtocolHeader::ePacketID>::iterator iter = m_broadcastPacketIDs.begin();
	for (; iter != m_broadcastPacketIDs.end(); ++iter)
	{
		sProtocolHeader::ePacketID packetID = *iter;
		if (packetID == sProtocolHeader::ePacketID::e_ResponseRoomInfo)
		{
			this->BroadcastRoomInfo(packetID);
		}
		else if (packetID == sProtocolHeader::ePacketID::e_ResponseChatHistory)
		{
			this->BroadcastChatHistory();
		}
	}

	m_broadcastPacketIDs.clear();

	return true;
}


bool cRoom::BroadcastRoomInfo(sProtocolHeader::ePacketID packetID)
{
	std::cout << "BroadcastRoomInfo" << std::endl;
	m_roomInfoBuff.roomname = m_roomname;
	m_roomInfoBuff.roomname_length = (short)m_roomname.size();

	m_roomInfoBuff.numberOfUsers = (short)m_users.size();
	m_roomInfoBuff.users.clear();
	for (int i = 0; i < (int)m_users.size(); ++i)
	{
		m_userBuff.username = m_users[i].username;
		m_userBuff.username_length = (short)m_userBuff.username.size();
		m_roomInfoBuff.users.push_back(m_userBuff);
	}

	m_headerBuff.SetProtocol(m_roomInfoBuff);
	// override packet id
	m_headerBuff.packet_id = packetID;

	m_sendBuffer.clear();
	m_sendBuffer.CheckBufferSize(m_headerBuff.packet_length);
	m_sendBuffer.Serialize(m_headerBuff);
	m_sendBuffer.Serialize(m_roomInfoBuff);

	std::cout << "\tserialize length: " << m_sendBuffer.getLength() << " original size: " << m_headerBuff.packet_length << std::endl;

	return this->SendData();
}
bool cRoom::BroadcastChatHistory()
{
	std::cout << "BroadcastChatHistory" << std::endl;

	m_chatHistoryBuff.numberOfMessages = (int)m_chatHistory.size();
	m_chatHistoryBuff.chatHistory.clear();

	std::vector<std::pair<DWORD/*time*/, std::string>>::iterator iter = m_chatHistory.begin();
	for (; iter != m_chatHistory.end(); ++iter)
	{
		sProtocolChatMessage msg;
		msg.message = iter->second;
		msg.message_length = msg.message.size();

		m_chatHistoryBuff.chatHistory.push_back(msg);
	}
	m_chatHistory.clear();

	m_headerBuff.SetProtocol(m_chatHistoryBuff);

	m_sendBuffer.clear();
	m_sendBuffer.CheckBufferSize(m_headerBuff.packet_length);
	m_sendBuffer.Serialize(m_headerBuff);
	m_sendBuffer.Serialize(m_chatHistoryBuff);

	std::cout << "\tserialize length: " << m_sendBuffer.getLength() << " original size: " << m_headerBuff.packet_length << std::endl;

	return this->SendData();
}
//void cRoom::GetChatHistory(const std::string& userName, std::vector<std::string>& chatHistory)
//{
//	const sUserInfo& user = GetUser(userName);
//	for (int i = 0; i < m_chatHistory.size(); ++i)
//	{
//		if (m_chatHistory[i].first > user.connectedTime)
//		{
//			chatHistory.push_back(m_chatHistory[i].second);
//		}
//	}
//}

iUser* cRoom::GetUser(const std::string& username)
{
	for (int i = 0; i < (int)m_users.size(); ++i)
	{
		if (username == m_users[i].username)
			return m_users[i].user;
	}

	return 0;
}

bool cRoom::Join(iUser* user, const std::string& username)
{
	if (GetUser(username) != 0)
		return false;

	m_infoBuff.user = user;
	m_infoBuff.username = username;
	m_infoBuff.connectedTime = GetCurrentTime();

	m_users.push_back(m_infoBuff);

	this->InsertBroadcastPacketID(sProtocolHeader::ePacketID::e_ResponseRoomInfo);

	return true;
}
void cRoom::Leave(iUser* user)
{
	for (int i = 0; i < (int)m_users.size(); ++i)
	{
		if (user == m_users[i].user)
		{
			m_users.erase(m_users.begin() + i);
			break;
		}
	}

	this->InsertBroadcastPacketID(sProtocolHeader::ePacketID::e_ResponseRoomInfo);
}
//const sUserInfo& cRoom::GetUser(const std::string& username)
//{
//	for (int i = 0; i < m_users.size(); ++i)
//	{
//		if (m_users[i].userName.compare(username) == 0)
//			return m_users[i];
//	}
//
//}
//void cRoom::GetUsers(std::vector<std::string>& usernames)
//{
//	for (int i = 0; i < m_users.size(); ++i)
//	{
//		usernames.push_back(m_users[i].userName);
//	}
//}

void cRoom::AddChat(const std::string& username, const std::string& message)
{
	m_chatHistory.push_back(std::pair<DWORD, std::string>(GetCurrentTime(), username + " : " + message));

	this->InsertBroadcastPacketID(sProtocolHeader::ePacketID::e_ResponseChatHistory);
}
bool cRoom::CheckUser(iUser* user)
{
	for (int i = 0; i < (int)m_users.size(); ++i)
	{
		if (user == m_users[i].user)
			return true;
	}
	return false;
}

bool cRoom::SendData()
{
	const char* buffer = m_sendBuffer.toCharArray();
	int sizeOfBuffer = m_sendBuffer.getLength();

	for (int i = 0; i < (int)m_users.size(); ++i)
	{
		Sleep(1);
		SOCKET client = m_users[i].user->SocketID();

		int result = send(client, buffer, sizeOfBuffer, 0);
		if (result == SOCKET_ERROR)
		{
			cErrorReport::GetInstance().PrintWSAGetLastMessage("\tSend failed: ");
			continue;
		}

		std::cout << "\t Socket: " << client << " Bytes sent: " << result << std::endl;

	}

	return true;
}