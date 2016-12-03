#pragma once

#include <cBuffer.h>

class iClientPacketProcedure
{
public:
	virtual ~iClientPacketProcedure() {}

	// begin of iPacketProcedureMonopoly
	virtual void SetHeader() = 0;
	virtual void AppendProtocol() = 0;
	virtual void SendData() = 0;

	virtual void ProcessReceiveData(cBuffer& receiveBuffer) = 0;
	// end of iPacketProcedureMonopoly
};
