#ifndef _sProtocols_HG_
#define _sProtocols_HG_

#include <string>
#include <vector>

#include "sProtocolsMonopoly.h"

struct sProtocolRoom
{
	sProtocolRoom()
		:roomname_length(0)
	{}
	short roomname_length;
	std::string roomname;
};
struct sProtocolUser
{
	sProtocolUser()
		:username_length(0)
	{}
	short username_length;
	std::string username;
};
struct sProtocolPassword
{
	sProtocolPassword()
		:password_length(0)
	{}
	short password_length;
	std::string password;
};
struct sProtocolAccount : sProtocolUser, sProtocolPassword
{
};
struct sProtocolResponseFailure
{
	sProtocolResponseFailure()
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
};
struct sProtocolRequestLeaveRoom : sProtocolUser, sProtocolRoom
{
};

struct sProtocolRequestNewRoom : sProtocolUser, sProtocolRoom
{
};
struct sProtocolRequestJoinRoom : sProtocolUser, sProtocolRoom
{
};
struct sProtocolResponseRoomList
{
	sProtocolResponseRoomList()
		: numberOfRooms(0)
	{}
	short numberOfRooms;
	std::vector<sProtocolRoom> rooms;
	std::vector<short> user_counts;
};

struct sProtocolResponseRoomInfo : sProtocolRoom
{
	sProtocolResponseRoomInfo()
		: numberOfUsers(0)
	{}
	short numberOfUsers;
	std::vector<sProtocolUser> users;
};
struct sProtocolLobbyUsers
{
	sProtocolLobbyUsers()
		: numberOfUsers(0)
	{}
	short numberOfUsers;
	std::vector<sProtocolUser> users;
};
struct sProtocolChatMessage
{
	sProtocolChatMessage()
		:message_length(0)
	{}
	int message_length;
	std::string message;
};
struct sProtocolSendChatMessage : sProtocolUser, sProtocolRoom, sProtocolChatMessage
{
};

struct sProtocolResponseChatHistory
{
	sProtocolResponseChatHistory()
		:numberOfMessages(0)
	{}
	int numberOfMessages;
	std::vector<sProtocolChatMessage> chatHistory;
};


struct sProtocolHeader
{
	enum ePacketID : unsigned int
	{
		e_None = 0,
		e_PlainText,

		e_RequestSignin,
		e_ResponseSigninSuccess,
		e_ResponseSigninFailure,
		e_RequestLogin,
		e_ResponseLoginSuccess,
		e_ResponseLoginFailure,
		e_RequestLogout,
		e_ResponseLogoutSuccess,

		e_RequestNewRoom,
		e_ResponseNewRoomSuccess,
		e_ResponseNewRoomFailure,

		e_RequestJoinRoom,
		e_ResponseJoinRoomSuccess,
		e_ResponseJoinRoomFailure,

		e_SendChatMessage,
		e_SendChatMessageFailure,

		e_RequestChatHistory,
		e_ResponseChatHistory,

		e_RequestLeaveRoom,
		e_ResponseLeaveRoomSuccess,
		e_ResponseLeaveRoomFailure,

		e_RequestDisconnect,

		e_RequestRoomList,
		e_ResponseRoomList,

		e_ResponseRoomInfo,
		e_ResponseLobbyUsers,

		e_ResponseDisconnect,

		/************************************************
		// Protocols for Game Monopoly
		************************************************/
		e_PlayMonopoly,
	};


	sProtocolHeader()
		: packet_length(0)
		, packet_id(ePacketID::e_None)
	{}
	int packet_length;
	ePacketID packet_id;

	void SetProtocol(const std::string& data)
	{
		packet_id = e_PlainText;
		packet_length = data.size();
		packet_length += sizeof(sProtocolHeader);
	}
	void SetProtocol(const sProtocolAccount& data)
	{
		packet_id = e_RequestSignin;
		packet_length = data.username_length + data.password_length;
		packet_length += sizeof(data.username_length) + sizeof(data.password_length) + sizeof(sProtocolHeader);
	}
	void SetProtocol(const sProtocolLobbyUsers& data)
	{
		packet_id = e_ResponseLobbyUsers;
		packet_length = sizeof(data.numberOfUsers) + sizeof(sProtocolHeader);
		for (unsigned int i = 0; i < data.users.size(); ++i)
		{
			packet_length += sizeof(data.users[i].username_length);
		}
	}
	void SetProtocol(const sProtocolRequestNewRoom& data)
	{
		packet_id = e_RequestNewRoom;
		packet_length = data.username_length + data.roomname_length;
		packet_length += sizeof(data.username_length) + sizeof(data.roomname_length) + sizeof(sProtocolHeader);
	}
	void SetProtocol(const sProtocolRequestJoinRoom& data)
	{
		packet_id = e_RequestJoinRoom;
		packet_length = data.roomname_length + data.username_length;
		packet_length += sizeof(data.roomname_length) + sizeof(data.username_length) + sizeof(sProtocolHeader);
	}
	void SetProtocol(const sProtocolResponseRoomList& data)
	{
		packet_id = e_ResponseRoomList;
		packet_length = sizeof(data.numberOfRooms) + sizeof(sProtocolHeader);
		for (int i = 0; i < data.numberOfRooms; ++i)
		{
			packet_length += data.rooms[i].roomname_length;
			packet_length += sizeof(data.rooms[i].roomname_length);
			packet_length += sizeof(data.user_counts[i]);
		}
	}
	void SetProtocol(const sProtocolResponseRoomInfo& data)
	{
		packet_id = e_ResponseRoomInfo;
		packet_length = data.roomname_length;
		packet_length += sizeof(data.roomname_length) + sizeof(data.numberOfUsers) + sizeof(sProtocolHeader);
		for (int i = 0; i < data.numberOfUsers; ++i)
		{
			packet_length += data.users[i].username_length;
			packet_length += sizeof(data.users[i].username_length);
		}
	}
	void SetProtocol(const sProtocolRequestLeaveRoom& data)
	{
		packet_id = e_RequestLeaveRoom;
		packet_length = data.roomname_length + data.username_length;
		packet_length += sizeof(data.roomname_length) + sizeof(data.username_length) + sizeof(sProtocolHeader);
	}
	void SetProtocol(const sProtocolSendChatMessage& data)
	{
		packet_id = e_SendChatMessage;
		packet_length = data.username_length + data.roomname_length + data.message_length;
		packet_length += sizeof(data.username_length) + sizeof(data.roomname_length) + sizeof(data.message_length) + sizeof(sProtocolHeader);
	}
	void SetProtocol(const sProtocolResponseChatHistory& data)
	{
		packet_id = e_ResponseChatHistory;

		packet_length = sizeof(data.numberOfMessages) + sizeof(sProtocolHeader);
		for (int i = 0; i < data.numberOfMessages; ++i)
		{
			packet_length += data.chatHistory[i].message_length;
			packet_length += sizeof(data.chatHistory[i].message_length);
		}
	}
	void SetProtocol(const sProtocolResponseFailure& data)
	{
		packet_id = e_None;
		packet_length = data.message_length;
		packet_length += sizeof(data.message_length) + sizeof(data.failureCode) + sizeof(sProtocolHeader);
	}
	void SetProtocol(sProtocolMonopolyHeader& data)
	{
		packet_id = e_PlayMonopoly;
		packet_length = data.Size();
	}
};


#endif