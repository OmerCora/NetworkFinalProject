#include "cFreeParking.h"

#include <conio.h>
#include <iostream>


cFreeParking::cFreeParking(int districtID)
	:cDistrict(districtID)
{
}


cFreeParking::~cFreeParking()
{
}
bool cFreeParking::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cFreeParking::Action()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
	char anyKey = _getch();

	return true;
}
bool cFreeParking::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cFreeParking::Response()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
	char anyKey = _getch();

	return true;
}

