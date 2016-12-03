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
struct sProtocolResponseGameStart : public iProtocol
{
	sProtocolResponseGameStart()
		:isMyTurn(0)
	{}
	char isMyTurn;
	unsigned int Size()
	{
		return sizeof(isMyTurn);
	}
};
struct sProtocolRequestPlayThrowDice : public iProtocol
{
	sProtocolRequestPlayThrowDice()
	{}
	unsigned int Size()
	{
		return 0;
	}
};
struct sProtocolResponsePlayThrowDice : public iProtocol
{
	sProtocolResponsePlayThrowDice()
		:nextLocation(0)
	{}
	short nextLocation;
	unsigned int Size()
	{
		return sizeof(nextLocation);
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
		: districtType(-1)
	{}
	short districtType;
	unsigned int Size()
	{
		return sizeof(districtType);
	}
};
struct sProtocolAskAssetAction : public iProtocol
{
	sProtocolAskAssetAction()
		: districtType(-1)
	{}
	short districtType;
	int districtID;
	int price;
	unsigned int Size()
	{
		return sizeof(districtType) + sizeof(districtID) + sizeof(price);
	}
};
struct sProtocolAnswerAssetAction : public iProtocol
{
	sProtocolAnswerAssetAction()
		: yesOrNo(1)
	{}
	char yesOrNo;
	int districtID;
	unsigned int Size()
	{
		return sizeof(yesOrNo) + sizeof(districtID);
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
struct sProtocolResponsePlayTurnKeep : public iProtocol
{
	sProtocolResponsePlayTurnKeep()
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
struct sProtocolResponseGameOver : public iProtocol
{
	sProtocolResponseGameOver()
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

		e_RequestPlayThrowDice,
		e_ResponsePlayThrowDice,

		e_RequestPlayAction,
		e_ResponsePlayAction,

		e_AskAssetAction,
		e_AnswerAssetAction,

		e_ResponsePlayTurnChange,
		e_ResponsePlayTurnKeep,

		e_ResponseGameFinish,
		e_ResponseGameOver,
	};


	enum eDistrictType : int
	{
		e_Card = 0,
		e_FreeParking,
		e_GotoJail,
		e_Jail,
		e_Start,
		e_Tax,
		e_Building,
		e_Station,
		e_Utility,
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
	void SetProtocol(ePacketID packetID, sProtocolResponsePlayFailure& data)
	{
		packet_id = packetID;
		packet_length += data.Size();
	}

	void SetProtocol(sProtocolResponseGameStart& data)
	{
		packet_id = e_ResponseGameStart;
		packet_length += data.Size();
	}
	void SetProtocol(sProtocolRequestPlayThrowDice& data)
	{
		packet_id = e_RequestPlayThrowDice;
		packet_length += data.Size();
	}
	void SetProtocol(sProtocolResponsePlayThrowDice& data)
	{
		packet_id = e_ResponsePlayThrowDice;
		packet_length += data.Size();
	}
	void SetProtocol(sProtocolRequestPlayAction& data)
	{
		packet_id = e_RequestPlayAction;
		packet_length += data.Size();
	}
	void SetProtocol(sProtocolResponsePlayAction& data)
	{
		packet_id = e_ResponsePlayAction;
		packet_length += data.Size();
	}
	void SetProtocol(sProtocolAskAssetAction& data)
	{
		packet_id = e_AskAssetAction;
		packet_length += data.Size();
	}
	void SetProtocol(sProtocolAnswerAssetAction& data)
	{
		packet_id = e_AnswerAssetAction;
		packet_length += data.Size();
	}
	void SetProtocol(sProtocolResponsePlayTurnChange& data)
	{
		packet_id = e_ResponsePlayTurnChange;
		packet_length += data.Size();
	}
	void SetProtocol(sProtocolResponsePlayTurnKeep& data)
	{
		packet_id = e_ResponsePlayTurnChange;
		packet_length += data.Size();
	}
	void SetProtocol(sProtocolResponseGameFinish& data)
	{
		packet_id = e_ResponseGameFinish;
		packet_length += data.Size();
	}
	void SetProtocol(sProtocolResponseGameOver& data)
	{
		packet_id = e_ResponseGameOver;
		packet_length += data.Size();
	}
};


#endif