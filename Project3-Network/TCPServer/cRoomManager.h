#ifndef _cRoomManager_HG_
#define _cRoomManager_HG_

#include "iUser.h"
#include "iRoom.h"
#include "iLobby.h"
#include <vector>
#include <sProtocols.h>

class cRoomManager
{
	cRoomManager();
	~cRoomManager();
	static cRoomManager* m_instance;
public:
	static cRoomManager& GetInstance();
	static void DeleteInstance();

	iLobby* GetLobby();

	iRoom* CreateRoom(const std::string& name);
	iRoom* GetRoom(const std::string& name);
	iRoom* cRoomManager::GetRoomByUser(iUser* client);

	void SerializeRoomList(sProtocolHeader::ePacketID packetID, cBuffer& buffer);

	void BroadcastData();

	void ProcessReceivedPlayData(cBuffer& receiveBuffer);
	void UpdateGameLoop();

private:

	iLobby* m_lobby;
	//std::vector<iUser*> m_users;
	const int m_MAXIMUM_ROOMS = 10000;
	std::vector<iRoom*> m_rooms;

	sProtocolHeader m_headerBuff;
	sProtocolResponseRoomList m_roomListBuff;

};

#endif