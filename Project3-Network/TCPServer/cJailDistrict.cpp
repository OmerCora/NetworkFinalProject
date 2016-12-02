#include "cJailDistrict.h"

#include <iostream>


cJailDistrict::cJailDistrict(int districtID)
	:cDistrict(districtID)
{
}


cJailDistrict::~cJailDistrict()
{
}
bool cJailDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "cJailDistrict::Action()" << std::endl;
	return true;
}
