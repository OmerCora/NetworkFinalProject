#include "cUtilityDistrict.h"

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
	std::cout << "cUtilityDistrict::Action()" << std::endl;
	return true;
}
bool cUtilityDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "cUtilityDistrict::Response()" << std::endl;
	return true;
}

