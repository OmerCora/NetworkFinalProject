#pragma once

#include "iUser.h"

class iPlayer
{
public:
	virtual ~iPlayer() {}

	virtual bool IsBankrupty() = 0;
	virtual bool Debit(unsigned int money) = 0;
	virtual bool Withdraw(unsigned int money) = 0;

};

