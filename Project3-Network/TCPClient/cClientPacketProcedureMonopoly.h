#pragma once

#include "iClientPacketProcedure.h"
#include <cBuffer.h>

class cClientPacketProcedureMonopoly : public iClientPacketProcedure
{
public:
	cClientPacketProcedureMonopoly();
	~cClientPacketProcedureMonopoly();

	// begin of iPacketProcedureMonopoly
	virtual void SetHeader();
	virtual void AppendProtocol();
	virtual void SendData();

	virtual void ProcessReceiveData(cBuffer& receiveBuffer);
	// end of iPacketProcedureMonopoly


private:
	cBuffer m_sendBuffer;
};

