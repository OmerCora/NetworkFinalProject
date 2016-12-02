#pragma once

#include "iPlayer.h"


class cPlayer : public iPlayer
{
public:
	cPlayer(iUser* user);
	virtual ~cPlayer();

	virtual bool IsReadyToPlay();

	virtual bool IsBankrupty();
	virtual bool Debit(unsigned int money);
	virtual bool Withdraw(unsigned int money);

private:
	unsigned int m_money;
	bool m_isReady;

	iUser* m_user;
};

