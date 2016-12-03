#pragma once

#include "iPacketProcedureMonopoly.h"
#include "iLogicMonopolyMediator.h"

class cPacketProcedureMonopoly : public iPacketProcedureMonopoly
{
public:
	cPacketProcedureMonopoly(iLogicMonopolyMediator& logic);
	~cPacketProcedureMonopoly();

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

private:
	iLogicMonopolyMediator& m_logic;

	sProtocolHeader m_rootHeader;
	sProtocolMonopolyHeader m_header;

	cBuffer m_temporaryBuffer;
	cBuffer m_sendBuffer;
};

