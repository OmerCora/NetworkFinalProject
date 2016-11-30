#include "cRoomManager.h"

#include "cErrorReport.h"

#include <iostream>

#include "cRoom.h"
#include "cLobby.h"

cRoomManager::cRoomManager()
	:m_lobby(new cLobby())
{
	m_rooms.reserve(m_MAXIMUM_ROOMS);
}


cRoomManager::~cRoomManager()
{
	std::vector<iRoom*>::iterator iter = m_rooms.begin();
	for (; iter != m_rooms.end(); ++iter)
	{
		iRoom* p = *iter;
		delete p;
	}
	m_rooms.clear();

	if (m_lobby)
		delete m_lobby;
}

cRoomManager* cRoomManager::m_instance = 0;
cRoomManager& cRoomManager::GetInstance()
{
	if (m_instance == 0)
		m_instance = new cRoomManager();
	return *m_instance;
}
void cRoomManager::DeleteInstance()
{
	if (m_instance)
		delete m_instance;
	m_instance = 0;
}

void cRoomManager::BroadcastData()
{
	for (int i = 0; i < (int)m_rooms.size(); ++i)
	{
		m_rooms[i]->BroadcastData();
	}
}


iLobby* cRoomManager::GetLobby()
{
	return m_lobby;
}


iRoom* cRoomManager::CreateRoom(const std::string& name)
{
	std::cout << "CreateRoom: " << name << std::endl;
	iRoom* room = this->GetRoom(name);
	if (room != 0)
	{
		std::cout << "Error: already existed room: " << name << std::endl;
		return 0;
	}
	room = new cRoom(name);
	m_rooms.push_back(room);
	return room;
}
iRoom* cRoomManager::GetRoom(const std::string& name)
{
	for (int i = 0; i < (int)m_rooms.size(); ++i)
	{
		if (m_rooms[i]->Name().compare(name) == 0)
			return m_rooms[i];
	}

	return 0;
}
iRoom* cRoomManager::GetRoomByUser(iUser* client)
{
	for (int i = 0; i < (int)m_rooms.size(); ++i)
	{
		if (m_rooms[i]->CheckUser(client))
			return m_rooms[i];
	}
	return 0;
}
void cRoomManager::SerializeRoomList(sProtocolHeader::ePacketID packetID, cBuffer& buffer)
{
	std::cout << "SerializeRoomList: " << std::endl;

	m_roomListBuff.numberOfRooms = (short)m_rooms.size();
	m_roomListBuff.rooms.clear();
	m_roomListBuff.user_counts.clear();
	for (int i = 0; i < (int)m_rooms.size(); ++i)
	{
		sProtocolRoom room;
		room.roomname = m_rooms[i]->Name();
		room.roomname_length = (short)room.roomname.size();
		m_roomListBuff.rooms.push_back(room);
		m_roomListBuff.user_counts.push_back(m_rooms[i]->GetNumUsers());
		std::cout << "\t" << room.roomname << std::endl;
	}

	m_headerBuff.SetProtocol(m_roomListBuff);
	// override
	m_headerBuff.packet_id = packetID;

	buffer.clear();
	buffer.CheckBufferSize(m_headerBuff.packet_length);
	buffer.Serialize(m_headerBuff);
	buffer.Serialize(m_roomListBuff);

	std::cout << "\tserialize length: " << buffer.getLength() << " original size: " << m_headerBuff.packet_length << std::endl;
}
