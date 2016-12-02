#include "cCardDistrict.h"

#include <iostream>


cCardDistrict::cCardDistrict(int districtID, eCardDistrictType type)
	:cDistrict(districtID)
	,m_type(type)
{
}


cCardDistrict::~cCardDistrict()
{
}
bool cCardDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "cCardDistrict::Action()" << std::endl;
	return true;
}