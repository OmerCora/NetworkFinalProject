#pragma once

#include "iPlayer.h"

class iLogicMonopolyMediator
{
public:
	virtual ~iLogicMonopolyMediator() {}

	virtual void BringToStart(iPlayer* player) = 0;
};

