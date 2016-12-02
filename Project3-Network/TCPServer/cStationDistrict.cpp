#include "cStationDistrict.h"

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
	std::cout << "cAssetDistrict::Action()" << std::endl;
	return true;
}