#include "cStartDistrict.h"

#include <iostream>

cStartDistrict::cStartDistrict(int districtID)
	:cDistrict(districtID)
{
}


cStartDistrict::~cStartDistrict()
{
}
bool cStartDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "cStartDistrict::Action()" << std::endl;
	return true;
}
bool cStartDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "cStartDistrict::Response()" << std::endl;
	return true;
}
