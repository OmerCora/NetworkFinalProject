#include "MonopolyGlobal.h"
#include "cStationDistrict.h"

#include <conio.h>
#include <iostream>


cStationDistrict::cStationDistrict(int districtID)
	:cAssetDistrict(districtID)
{
}


cStationDistrict::~cStationDistrict()
{
}
bool cStationDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cStationDistrict::Action()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif

	return true;
}
bool cStationDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cStationDistrict::Response()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif

	return true;
}
