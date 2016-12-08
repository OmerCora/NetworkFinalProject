#pragma once

#include "iClientPacketProcedure.h"
#include <cBuffer.h>
#include "iTCPClient.h"


class cClientPacketProcedureMonopoly : public iClientPacketProcedure
{
public:
	cClientPacketProcedureMonopoly(iTCPClient& client);
	~cClientPacketProcedureMonopoly();


	// begin of iPacketProcedureMonopoly
	virtual void SetHeader(sProtocolMonopolyHeader::ePacketID packetID);
	virtual void AppendProtocol(sProtocolResponseGameStart& protocol);
	virtual void AppendProtocol(sProtocolRequestPlayThrowDice& protocol);
	virtual void AppendProtocol(sProtocolResponsePlayThrowDice& protocol);
	virtual void AppendProtocol(sProtocolRequestPlayAction& protocol);
	virtual void AppendProtocol(sProtocolResponsePlayAction& protocol);
	virtual void AppendProtocol(sProtocolAskAssetAction& protocol);
	virtual void AppendProtocol(sProtocolAnswerAssetAction& protocol);
	virtual void AppendProtocol(sProtocolResponsePlayTurnChange& protocol);
	virtual void AppendProtocol(sProtocolResponsePlayTurnKeep& protocol);
	virtual void AppendProtocol(sProtocolResponseGameFinish& protocol);
	virtual void AppendProtocol(sProtocolResponseGameOver& protocol);
	virtual bool SendData(SOCKET client);

	virtual void ProcessReceiveData(cBuffer& receiveBuffer);
	// end of iPacketProcedureMonopoly

	virtual sProtocolPlayerInfo& MyInfo() { return m_myInfo; }
	virtual sProtocolPlayerInfo& OpponentInfo() { return m_opponentInfo; }
	virtual sProtocolPlayerInfo& CurrentPlayerInfo() { return m_currentPlayerInfo; }
	virtual bool IsMyTurn() { return (m_myInfo.id== m_currentPlayerInfo.id); }
	virtual sProtocolDistrictInfo& CurrentDistrictInfo() { return m_currentDistrictInfo; }
	virtual sProtocolBoardInfo& CurrentBoardInfo() { return m_currentBoardInfo; }
private:
	void SetPlayerInfo(sProtocolPlayerInfo& playerInfo);

	sProtocolHeader m_rootHeader;
	sProtocolMonopolyHeader m_header;

	sProtocolPlayerInfo m_myInfo;
	sProtocolPlayerInfo m_opponentInfo;
	sProtocolPlayerInfo m_currentPlayerInfo;
	sProtocolDistrictInfo m_currentDistrictInfo;
	sProtocolBoardInfo m_currentBoardInfo;

	cBuffer m_temporaryBuffer;
	cBuffer m_sendBuffer;
	iTCPClient& m_client;
};

