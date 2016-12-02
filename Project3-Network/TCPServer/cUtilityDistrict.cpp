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
	std::cout << "cAssetDistrict::Action()" << std::endl;
	return true;
}