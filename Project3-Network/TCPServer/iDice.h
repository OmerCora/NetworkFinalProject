#pragma once

class iDice
{
public:
	virtual ~iDice() {}

	virtual int Throw() = 0;
	virtual bool DoubleThrow(float &diceValue) = 0;
};

