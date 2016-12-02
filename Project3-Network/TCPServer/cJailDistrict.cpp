#include "cJailDistrict.h"

#include <conio.h>
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
	std::cout << "\t cJailDistrict::Action()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
	char anyKey = _getch();

	return true;
}
bool cJailDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cJailDistrict::Response()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
	char anyKey = _getch();

	return true;
}
