#include "cBuildingDistrict.h"


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
	std::cout << "cAssetDistrict::Action()" << std::endl;
	return true;
}