#include "MonopolyGlobal.h"
#include "cGotoJailDistrict.h"
#include "cPlayer.h"

#include <conio.h>
#include <iostream>


cGotoJailDistrict::cGotoJailDistrict(int districtID, iDistrict& jail)
	:cDistrict(districtID)
	, m_jail(jail)
{
}


cGotoJailDistrict::~cGotoJailDistrict()
{
}

bool cGotoJailDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	this->RemovePlayer(player, logic);
	dynamic_cast<cPlayer*>(player)->setbJustCameInJail(true);
	m_jail.AddPlayer(player, logic);

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
