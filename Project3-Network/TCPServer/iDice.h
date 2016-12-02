#pragma once

class iDice
{
public:
	virtual ~iDice() {}

	virtual int Throw() = 0;
};

