#include "MonopolyGlobal.h"
#include "cCardDistrict.h"

#include <conio.h>
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
	if (this->m_type == e_Community)
	{
		//ask the manager to let the player pick one card from community chest cards pile
		logic.openCommunityCardTo(player);


	}
	else if (this->m_type == e_Chance)
	{
		//ask the manager to let the player pick one card from chance cards pile

	}
	
	std::cout << "\t cCardDistrict::Actioon()" << std::endl;

	std::cout << "\t Press Any Key to Continue" << std::endl;
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif


	return true;
}

bool cCardDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cCardDistrict::Response()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif
	return true;
}

