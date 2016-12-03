#include "MonopolyGlobal.h"
#include "cDice.h"

#include <conio.h>
#include <iostream>

cDice::cDice()
{
}

cDice::~cDice()
{
}

int cDice::Throw()
{
	int diceNumber = rand() % 6 +1;
	std::cout << "\t cDice::Throw() diceNumber: " << diceNumber << std::endl;
	return diceNumber;
}

bool cDice::DoubleThrow(int & diceValue)
{
	int diceNumber1 = rand() % 6 +1; //1..6
	int diceNumber2 = rand() % 6 +1; //1..6

	diceValue = diceNumber1 + diceNumber2;

	if (diceNumber1 == diceNumber2)
	{
		return true;
	}
	else
	{
		return false;
	}
}