#pragma once

#include "iPlayer.h"
#include "iPacketProcedureMonopoly.h"

class iLogicMonopolyMediator
{
public:
	virtual ~iLogicMonopolyMediator() {}

	virtual iPacketProcedureMonopoly& PacketProcedure() = 0;
	virtual iPlayer& PlayerA() = 0;
	virtual iPlayer& PlayerB() = 0;

	virtual void BringToStart(iPlayer* player) = 0;


};

