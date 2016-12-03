#pragma once


#undef UNICODE
// exclude many header files of windows.h
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <sProtocolsMonopoly.h>
#include "cBuffer.h"


class iClientPacketProcedure
{
public:
	virtual ~iClientPacketProcedure() {}

	// begin of iPacketProcedureMonopoly
	virtual void SetHeader(sProtocolMonopolyHeader::ePacketID packetID) = 0;
	virtual void AppendProtocol(sProtocolResponseGameStart& protocol) = 0;
	virtual void AppendProtocol(sProtocolRequestPlayThrowDice& protocol) = 0;
	virtual void AppendProtocol(sProtocolResponsePlayThrowDice& protocol) = 0;
	virtual void AppendProtocol(sProtocolRequestPlayAction& protocol) = 0;
	virtual void AppendProtocol(sProtocolResponsePlayAction& protocol) = 0;
	virtual void AppendProtocol(sProtocolAskAssetAction& protocol) = 0;
	virtual void AppendProtocol(sProtocolAnswerAssetAction& protocol) = 0;
	virtual void AppendProtocol(sProtocolResponsePlayTurnChange& protocol) = 0;
	virtual void AppendProtocol(sProtocolResponsePlayTurnKeep& protocol) = 0;
	virtual void AppendProtocol(sProtocolResponseGameFinish& protocol) = 0;
	virtual void AppendProtocol(sProtocolResponseGameOver& protocol) = 0;
	virtual bool SendData(SOCKET client) = 0;

	virtual void ProcessReceiveData(cBuffer& receiveBuffer) = 0;
	// end of iPacketProcedureMonopoly
};
