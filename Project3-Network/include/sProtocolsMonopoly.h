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

struct sProtocolNameInfo : public iProtocol
{
	sProtocolNameInfo()
		: name_length(0)
	{}
	short name_length;
	std::string name;
	unsigned int Size()
	{
		name_length = (short)name.size();
		return name_length;
	}
};
struct sProtocolPlayerInfo : public iProtocol
{
	sProtocolPlayerInfo()
		: isMyTurn(0)
		, chanceToThrowDice(0)
		, money(0)
		, moneyVariation(0)
		, location(0)
	{}
	int id;
	sProtocolNameInfo nick;
	char isMyTurn;
	char chanceToThrowDice;
	int money;
	int moneyVariation;
	short location;
	unsigned int Size()
	{
		return sizeof(id) + nick.Size() + sizeof(isMyTurn) + sizeof(chanceToThrowDice) + sizeof(money) + sizeof(moneyVariation) + sizeof(location);
	}
};
struct sProtocolDistrictInfo : public iProtocol
{
	enum eDistrictType : char
	{
		e_None = 0,
		e_Card,
		e_FreeParking,
		e_GotoJail,
		e_Jail,
		e_Start,
		e_Tax,
		e_Building,
		e_Station,
		e_Utility,
	};

	sProtocolDistrictInfo()
		: price(0)
		, districtType(sProtocolDistrictInfo::e_None)
		, district_id(0)
		, owner_id(0)
	{}
	int price;
	sProtocolDistrictInfo::eDistrictType districtType;
	char district_id;
	char owner_id;
	unsigned int Size()
	{
		return sizeof(price) + sizeof(districtType) + sizeof(district_id) + sizeof(owner_id);
	}
};
struct sProtocolBoardInfo : public iProtocol
{
	sProtocolBoardInfo()
	{}
	sProtocolPlayerInfo playerA;
	sProtocolPlayerInfo playerB;

	sProtocolDistrictInfo districts[40];
	unsigned int Size()
	{
		size_t data_size = playerA.Size() + playerB.Size();
		for (int i = 0; i < 40; ++i)
		{
			data_size += districts[i].Size();
		}
		return data_size;
	}
};
struct sProtocolResponseGameStart : public iProtocol
{
	sProtocolResponseGameStart()
	{}
	sProtocolPlayerInfo player;
	//sProtocolPlayerInfo playerB;
	unsigned int Size()
	{
		return player.Size();// +playerB.Size();
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
	{}
	sProtocolPlayerInfo player;
	unsigned int Size()
	{
		return player.Size();
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
		: districtType(sProtocolDistrictInfo::e_None)
	{}
	sProtocolDistrictInfo::eDistrictType districtType;
	sProtocolPlayerInfo player;
	sProtocolDistrictInfo district;
	unsigned int Size()
	{
		return sizeof(districtType) + player.Size() + district.Size();
	}
};
struct sProtocolAskAssetAction : public iProtocol
{
	sProtocolAskAssetAction()
	{}
	sProtocolDistrictInfo districtInfo;
	sProtocolPlayerInfo player;
	unsigned int Size()
	{
		return districtInfo.Size() + player.Size();
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
	sProtocolPlayerInfo player;
	sProtocolBoardInfo board;
	unsigned int Size()
	{
		return player.Size() + board.Size();
	}
};
struct sProtocolResponsePlayTurnKeep : public iProtocol
{
	sProtocolResponsePlayTurnKeep()
	{}
	sProtocolPlayerInfo player;
	sProtocolBoardInfo board;
	unsigned int Size()
	{
		return player.Size() + board.Size();
	}
};
struct sProtocolResponseGameFinish : public iProtocol
{
	sProtocolResponseGameFinish()
	{}
	sProtocolPlayerInfo player;//winner
	unsigned int Size()
	{
		return player.Size();
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


	sProtocolMonopolyHeader()
		: packet_length(0)
		, packet_id(ePacketID::e_None)
	{}
	int packet_length;
	ePacketID packet_id;

	unsigned int Size()
	{
		return sizeof(packet_length) + sizeof(packet_id);
	}
	void SetProtocol(sProtocolBoardInfo& data)
	{
		packet_length = data.Size() + this->Size();
	}
	void SetProtocol(ePacketID packetID, sProtocolResponsePlayFailure& data)
	{
		packet_id = packetID;
		packet_length = data.Size() + this->Size();
	}
	void SetProtocol(ePacketID packetID, sProtocolPlayerInfo& data)
	{
		packet_id = packetID;
		packet_length = data.Size() + this->Size();
	}
	void SetProtocol(ePacketID packetID, sProtocolDistrictInfo& data)
	{
		packet_id = packetID;
		packet_length = data.Size() + this->Size();
	}
	void SetProtocol(ePacketID packetID, sProtocolBoardInfo& data)
	{
		packet_id = packetID;
		packet_length = data.Size() + this->Size();
	}
	void SetProtocol(sProtocolResponseGameStart& data)
	{
		packet_id = e_ResponseGameStart;
		packet_length = data.Size() + this->Size();
	}
	void SetProtocol(sProtocolRequestPlayThrowDice& data)
	{
		packet_id = e_RequestPlayThrowDice;
		packet_length = data.Size() + this->Size();
	}
	void SetProtocol(sProtocolResponsePlayThrowDice& data)
	{
		packet_id = e_ResponsePlayThrowDice;
		packet_length = data.Size() + this->Size();
	}
	void SetProtocol(sProtocolRequestPlayAction& data)
	{
		packet_id = e_RequestPlayAction;
		packet_length = data.Size() + this->Size();
	}
	void SetProtocol(sProtocolResponsePlayAction& data)
	{
		packet_id = e_ResponsePlayAction;
		packet_length = data.Size() + this->Size();
	}
	void SetProtocol(sProtocolAskAssetAction& data)
	{
		packet_id = e_AskAssetAction;
		packet_length = data.Size() + this->Size();
	}
	void SetProtocol(sProtocolAnswerAssetAction& data)
	{
		packet_id = e_AnswerAssetAction;
		packet_length = data.Size() + this->Size();
	}
	void SetProtocol(sProtocolResponsePlayTurnChange& data)
	{
		packet_id = e_ResponsePlayTurnChange;
		packet_length = data.Size() + this->Size();
	}
	void SetProtocol(sProtocolResponsePlayTurnKeep& data)
	{
		packet_id = e_ResponsePlayTurnKeep;
		packet_length = data.Size() + this->Size();
	}
	void SetProtocol(sProtocolResponseGameFinish& data)
	{
		packet_id = e_ResponseGameFinish;
		packet_length = data.Size() + this->Size();
	}
	void SetProtocol(sProtocolResponseGameOver& data)
	{
		packet_id = e_ResponseGameOver;
		packet_length = data.Size() + this->Size();
	}
};


#endif