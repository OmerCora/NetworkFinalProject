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
	virtual void AppendProtocol(iProtocol& protocol);
	virtual void SendData();

	virtual void ProcessReceiveData(cBuffer& receiveBuffer);
	// end of iPacketProcedureMonopoly

private:
	iLogicMonopolyMediator& m_logic;

	sProtocolMonopolyHeader m_header;

	cBuffer m_sendBuffer;
};

