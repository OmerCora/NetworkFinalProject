#pragma once

#include "iPlayer.h"


class cPlayer : public iPlayer
{
public:
	cPlayer(iUser* user);
	virtual ~cPlayer();

	virtual iUser* User();

	virtual bool IsReadyToPlay();

	virtual bool IsBankrupty();
	virtual bool Deposit(unsigned int money);
	virtual bool Withdraw(unsigned int money);
	virtual void CurrentLocation(int location);
	virtual int CurrentLocation();
	virtual bool HasChanceToThrowDice();
	virtual void TakeChanceToThrowDice();
	virtual void SetEnjailCount(int numOfTurns);
	virtual int getEnjailCount(); 
	virtual bool getbCanThrowDice();
	virtual void setbCanThrowDice(bool bChance);

private:
	unsigned int m_money;
	unsigned int m_enjailedCount;

	bool m_isReady;

	bool m_chanceToThrowDice;
	bool m_CanThrowDice;
	bool m_bJustCameInJail;
	int m_currentLocation;
	iUser* m_user;
};

