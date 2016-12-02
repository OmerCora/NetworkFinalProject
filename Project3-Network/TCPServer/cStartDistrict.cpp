#include "cStartDistrict.h"

#include <conio.h>
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
	std::cout << "\t cStartDistrict::Action()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
	char anyKey = _getch();

	return true;
}
bool cStartDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cStartDistrict::Response()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
	char anyKey = _getch();

	return true;
}
