#include "MonopolyGlobal.h"
#include "cUtilityDistrict.h"

#include <conio.h>
#include <iostream>

cUtilityDistrict::cUtilityDistrict(int districtID)
	:cAssetDistrict(districtID)
{
}


cUtilityDistrict::~cUtilityDistrict()
{
}
bool cUtilityDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cUtilityDistrict::Action()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif

	return true;
}
bool cUtilityDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cUtilityDistrict::Response()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif
	return true;
}

