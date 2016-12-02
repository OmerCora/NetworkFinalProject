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
	std::cout << "cBuildingDistrict::Action()" << std::endl;
	return true;
}
bool cBuildingDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "cBuildingDistrict::Response()" << std::endl;
	return true;
}