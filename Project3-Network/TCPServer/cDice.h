#pragma once

#include "iDice.h"

class cDice : public iDice
{
public:
	cDice();
	virtual ~cDice();

	virtual int Throw();
	virtual bool DoubleThrow(int &diceValue);
};

