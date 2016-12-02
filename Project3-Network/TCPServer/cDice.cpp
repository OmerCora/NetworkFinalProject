#include "cDice.h"

#include <iostream>

cDice::cDice()
{
}

cDice::~cDice()
{
}

int cDice::Throw()
{
	return rand() % 6;
}
