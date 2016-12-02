#include "cGotoJailDistrict.h"

#include <iostream>


cGotoJailDistrict::cGotoJailDistrict(int districtID)
	:cDistrict(districtID)
{
}


cGotoJailDistrict::~cGotoJailDistrict()
{
}
bool cGotoJailDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "cGotoJailDistrict::Action()" << std::endl;
	return true;
}
bool cGotoJailDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "cGotoJailDistrict::Response()" << std::endl;
	return true;
}
