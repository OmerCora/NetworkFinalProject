#include "MonopolyGlobal.h"
#include "cBuildingDistrict.h"


#include <conio.h>
#include <iostream>

cBuildingDistrict::cBuildingDistrict(int districtID)
	:cAssetDistrict(districtID)
{
}


cBuildingDistrict::~cBuildingDistrict()
{
}
bool cBuildingDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cBuildingDistrict::Action()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif

	return true;
}
bool cBuildingDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cBuildingDistrict::Response()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif

	return true;
}