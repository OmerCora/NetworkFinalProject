#pragma once

#include "iPlayer.h"


class cPlayer : public iPlayer
{
public:
	cPlayer(iUser* user);
	virtual ~cPlayer();

	virtual bool IsReadyToPlay();

	virtual bool IsBankrupty();
	virtual bool Deposit(unsigned int money);
	virtual bool Withdraw(unsigned int money);
	virtual void CurrentLocation(int location);
	virtual int CurrentLocation();
	virtual bool HasChanceToThrowDice();
	virtual void TakeChanceToThrowDice();

private:
	unsigned int m_money;
	bool m_isReady;

	bool m_chanceToThrowDice;
	int m_currentLocation;
	iUser* m_user;
};

