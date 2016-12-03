#pragma once

#include "iUser.h"
#include <sProtocolsMonopoly.h>

class iPlayer
{
public:
	virtual ~iPlayer() {}

	virtual iUser* User() = 0;

	virtual bool IsBankrupty() = 0;
	virtual bool Deposit(unsigned int money) = 0;
	virtual bool Withdraw(unsigned int money) = 0;

	virtual void CurrentLocation(int location) = 0;
	virtual int CurrentLocation() = 0;
	virtual bool HasChanceToThrowDice() = 0;
	virtual void TakeChanceToThrowDice() = 0;

	virtual int getEnjailCount() = 0;
	virtual void SetEnjailCount(int numOfTurns) = 0;

	virtual bool getbCanThrowDice() = 0;
	virtual void setbCanThrowDice(bool bChance) = 0;

	virtual void GetPlayerInfo(sProtocolPlayerInfo& outInfo) = 0;
	
};

