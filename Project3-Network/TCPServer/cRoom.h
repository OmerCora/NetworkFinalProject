#ifndef _cRoom_HG_
#define _cRoom_HG_

#include <string>
#include <vector>
#include <set>

#undef UNICODE
// exclude many header files of windows.h
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "cBuffer.h"

#include "iUser.h"
#include "iRoom.h"

struct sRoomUserInfo
{
	iUser* user;
	std::string username;
	DWORD connectedTime;
};
class cRoom : public iRoom
{
public:
	cRoom(const std::string& name);
	~cRoom();

	virtual const std::string& Name();
	virtual bool CheckUser(iUser* user);
	virtual short GetNumUsers();

	virtual bool BroadcastData();

	virtual bool Join(iUser* user, const std::string& username);
	virtual void Leave(iUser* user);

	virtual void AddChat(const std::string& username, const std::string& message);
	//void GetUsers(std::vector<std::string>& usernames);
	//void GetChatHistory(const std::string& userName, std::vector<std::string>& chatHistory);


private:
	void InsertBroadcastPacketID(sProtocolHeader::ePacketID packetID);
	bool BroadcastRoomInfo(sProtocolHeader::ePacketID packetID);
	bool BroadcastChatHistory();
	bool SendData();

	iUser* GetUser(const std::string& username);

	cBuffer m_sendBuffer;
	std::set<sProtocolHeader::ePacketID> m_broadcastPacketIDs;

	std::string m_roomname;
	const int m_MAXIMUM_USERS = 128;
	std::vector<sRoomUserInfo> m_users;
	std::vector<std::pair<DWORD/*time*/, std::string>> m_chatHistory;

	sProtocolHeader m_headerBuff;
	sProtocolUser m_userBuff;
	sProtocolResponseRoomInfo m_roomInfoBuff;
	sProtocolResponseChatHistory m_chatHistoryBuff;
	sRoomUserInfo m_infoBuff;
};

#endif