#include "MonopolyGlobal.h"
#include "cGotoJailDistrict.h"

#include <conio.h>
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
	std::cout << "\t cGotoJailDistrict::Action()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif
	return true;
}
bool cGotoJailDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cGotoJailDistrict::Response()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif
	return true;
}
