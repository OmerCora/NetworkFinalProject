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
	std::cout << "cStationDistrict::Action()" << std::endl;
	return true;
}
bool cStationDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "cStationDistrict::Response()" << std::endl;
	return true;
}
