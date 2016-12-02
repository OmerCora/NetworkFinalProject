#ifndef _sProtocolsMonopoly_HG_
#define _sProtocolsMonopoly_HG_

/************************************************
Protocols for Game Monopoly
************************************************/

#include <string>
#include <vector>

struct iProtocol
{
	virtual ~iProtocol() {}
	virtual unsigned int Size() = 0;
};
struct sProtocolResponseStart : public iProtocol
{
	sProtocolResponseStart()
	{}
	unsigned int Size()
	{
		return 0;
	}
};
struct sProtocolRequestPlayDiceThrow : public iProtocol
{
	sProtocolRequestPlayDiceThrow()
	{}
	unsigned int Size()
	{
		return 0;
	}
};
struct sProtocolResponsePlayDiceThrow : public iProtocol
{
	sProtocolResponsePlayDiceThrow()
	{}
	unsigned int Size()
	{
		return 0;
	}
};
struct sProtocolRequestPlayAction : public iProtocol
{
	sProtocolRequestPlayAction()
	{}
	unsigned int Size()
	{
		return 0;
	}
};
struct sProtocolResponsePlayAction : public iProtocol
{
	sProtocolResponsePlayAction()
	{}
	unsigned int Size()
	{
		return 0;
	}
};
struct sProtocolResponsePlayTurnChange : public iProtocol
{
	sProtocolResponsePlayTurnChange()
	{}
	unsigned int Size()
	{
		return 0;
	}
};
struct sProtocolResponseGameFinish : public iProtocol
{
	sProtocolResponseGameFinish()
	{}
	unsigned int Size()
	{
		return 0;
	}
};

struct sProtocolResponsePlayFailure : public iProtocol
{
	sProtocolResponsePlayFailure()
		:message_length(0)
	{}
	enum eFailureCode : int
	{
		e_UserAlreadyLoggedin,
		e_HasnotSignedin,
		e_JustMessage,
	};
	short message_length;
	std::string errormessage;
	eFailureCode failureCode;
	unsigned int Size()
	{
		message_length = sizeof(message_length) + (short)(errormessage.size() + sizeof(failureCode));
		return message_length;
	}
};

//struct sProtocolUser
//{
//	sProtocolUser()
//		:username_length(0)
//	{}
//	short username_length;
//	std::string username;
//};
//struct sProtocolPassword
//{
//	sProtocolPassword()
//		:password_length(0)
//	{}
//	short password_length;
//	std::string password;
//};
//struct sProtocolAccount : sProtocolUser, sProtocolPassword
//{
//};
//struct sProtocolRequestLeaveRoom : sProtocolUser, sProtocolRoom
//{
//};
//
//struct sProtocolRequestNewRoom : sProtocolUser, sProtocolRoom
//{
//};
//struct sProtocolRequestJoinRoom : sProtocolUser, sProtocolRoom
//{
//};
//struct sProtocolResponseRoomList
//{
//	sProtocolResponseRoomList()
//		: numberOfRooms(0)
//	{}
//	short numberOfRooms;
//	std::vector<sProtocolRoom> rooms;
//	std::vector<short> user_counts;
//};
//
//struct sProtocolResponseRoomInfo : sProtocolRoom
//{
//	sProtocolResponseRoomInfo()
//		: numberOfUsers(0)
//	{}
//	short numberOfUsers;
//	std::vector<sProtocolUser> users;
//};
//struct sProtocolLobbyUsers
//{
//	sProtocolLobbyUsers()
//		: numberOfUsers(0)
//	{}
//	short numberOfUsers;
//	std::vector<sProtocolUser> users;
//};
//struct sProtocolChatMessage
//{
//	sProtocolChatMessage()
//		:message_length(0)
//	{}
//	int message_length;
//	std::string message;
//};
//struct sProtocolSendChatMessage : sProtocolUser, sProtocolRoom, sProtocolChatMessage
//{
//};
//struct sProtocolResponseChatHistory
//{
//	sProtocolResponseChatHistory()
//		:numberOfMessages(0)
//	{}
//	int numberOfMessages;
//	std::vector<sProtocolChatMessage> chatHistory;
//};
//



struct sProtocolMonopolyHeader : public iProtocol
{

	enum ePacketID : unsigned int
	{
		e_None = 0,
		e_ResponseGameStart,

		e_RequestPlayDiceThrow,
		e_ResponsePlayDiceThrow,

		e_RequestPlayAction,
		e_ResponsePlayAction,

		e_ResponsePlayTurnChange,

		e_ResponseGameFinish,
	};

	sProtocolMonopolyHeader()
		: packet_length(sizeof(sProtocolMonopolyHeader))
		, packet_id(ePacketID::e_None)
	{}
	int packet_length;
	ePacketID packet_id;

	unsigned int Size()
	{
		return packet_length;
	}
	void SetProtocol(iProtocol& data)
	{
		packet_id = e_None;
		packet_length += data.Size();
	}

	//void SetProtocol(sProtocolResponseStart& data)
	//{
	//	packet_id = e_ResponseGameStart;
	//	packet_length += data.Size();
	//	//packet_length += sizeof(data.username_length) + sizeof(data.password_length) + sizeof(sProtocolHeader);
	//}
	//void SetProtocol(sProtocolRequestPlayDiceThrow& data)
	//{
	//	packet_id = e_RequestPlayDiceThrow;
	//	packet_length += data.Size();
	//	//packet_length = sizeof(data.numberOfUsers) + sizeof(sProtocolHeader);
	//	//for (unsigned int i = 0; i < data.users.size(); ++i)
	//	//{
	//	//	packet_length += sizeof(data.users[i].username_length);
	//	//}
	//}
	//void SetProtocol(sProtocolResponsePlayDiceThrow& data)
	//{
	//	packet_id = e_ResponsePlayDiceThrow;
	//	packet_length += data.Size();
	//	//packet_length = data.username_length + data.roomname_length;
	//	//packet_length += sizeof(data.username_length) + sizeof(data.roomname_length) + sizeof(sProtocolHeader);
	//}
	//void SetProtocol(sProtocolRequestPlayAction& data)
	//{
	//	packet_id = e_RequestPlayAction;
	//	packet_length += data.Size();
	//	//packet_length = data.roomname_length + data.username_length;
	//	//packet_length += sizeof(data.roomname_length) + sizeof(data.username_length) + sizeof(sProtocolHeader);
	//}
	//void SetProtocol(sProtocolResponsePlayAction& data)
	//{
	//	packet_id = e_ResponsePlayAction;
	//	packet_length += data.Size();
	//	//packet_length = sizeof(data.numberOfRooms) + sizeof(sProtocolHeader);
	//	//for (int i = 0; i < data.numberOfRooms; ++i)
	//	//{
	//	//	packet_length += data.rooms[i].roomname_length;
	//	//	packet_length += sizeof(data.rooms[i].roomname_length);
	//	//	packet_length += sizeof(data.user_counts[i]);
	//	//}
	//}
	//void SetProtocol(sProtocolResponsePlayTurnChange& data)
	//{
	//	packet_id = e_ResponsePlayTurnChange;
	//	packet_length += data.Size();
	//	//packet_length = data.roomname_length;
	//	//packet_length += sizeof(data.roomname_length) + sizeof(data.numberOfUsers) + sizeof(sProtocolHeader);
	//	//for (int i = 0; i < data.numberOfUsers; ++i)
	//	//{
	//	//	packet_length += data.users[i].username_length;
	//	//	packet_length += sizeof(data.users[i].username_length);
	//	//}
	//}
	//void SetProtocol(sProtocolResponseGameFinish& data)
	//{
	//	packet_id = e_ResponseGameFinish;
	//	packet_length += data.Size();
	//	//packet_length = data.roomname_length + data.username_length;
	//	//packet_length += sizeof(data.roomname_length) + sizeof(data.username_length) + sizeof(sProtocolHeader);
	//}

};


#endif