#include "cFreeParking.h"

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
	std::cout << "cFreeParking::Action()" << std::endl;
	return true;
}
bool cFreeParking::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "cFreeParking::Response()" << std::endl;
	return true;
}

