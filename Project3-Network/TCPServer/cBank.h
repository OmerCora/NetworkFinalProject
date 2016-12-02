#pragma once
#include "cPlayer.h"

class cBank
{
public:
	cBank();
	~cBank();

	void giveMoney(cPlayer toWhom, unsigned int howmuch);
	void takeMoney(cPlayer fromWhom, unsigned int howmuch);

private:
	unsigned int m_money;

};