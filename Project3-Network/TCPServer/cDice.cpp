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
	int diceNumber = rand() % 6;
	std::cout << "\t cDice::Throw() diceNumber: " << diceNumber << std::endl;
	return diceNumber;
}
