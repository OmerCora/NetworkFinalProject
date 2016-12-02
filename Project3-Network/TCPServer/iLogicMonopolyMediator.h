#pragma once

#include "iPlayer.h"

class iLogicMonopolyMediator
{
public:
	virtual ~iLogicMonopolyMediator() {}

	virtual void BringToStart(iPlayer* player) = 0;

	virtual void openCommunityCardTo(iPlayer* player) = 0;
	virtual void openChanceCardTo(iPlayer* player) = 0;
	virtual void takeMoneyFrom(int moneyAmount, iPlayer* player) = 0;

};

