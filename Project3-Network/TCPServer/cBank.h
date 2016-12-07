#pragma once
#include "cPlayer.h"

class cBank
{
public:
	cBank();
	~cBank();

	void giveMoney(cPlayer toWhom, int howmuch);
	void takeMoney(cPlayer fromWhom, int howmuch);

private:
	int m_money;

};