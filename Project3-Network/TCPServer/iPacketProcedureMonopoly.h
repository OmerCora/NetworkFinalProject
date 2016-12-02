#pragma once

#include <cBuffer.h>
#include <sProtocolsMonopoly.h>

class iPacketProcedureMonopoly
{
public:
	virtual ~iPacketProcedureMonopoly() {}

	// begin of iPacketProcedureMonopoly
	virtual void SetHeader(sProtocolMonopolyHeader::ePacketID packetID) = 0;
	virtual void AppendProtocol(iProtocol& protocol) = 0;
	virtual void SendData() = 0;

	virtual void ProcessReceiveData(cBuffer& receiveBuffer) = 0;
	// end of iPacketProcedureMonopoly
};

