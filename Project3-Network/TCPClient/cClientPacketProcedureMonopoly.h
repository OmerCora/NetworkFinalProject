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


private:
	sProtocolMonopolyHeader m_header;

	cBuffer m_sendBuffer;
	iTCPClient& m_client;
};

