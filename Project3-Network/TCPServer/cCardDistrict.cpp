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
	if (this->m_type == e_Community)
	{
		//ask the manager to let the player pick one card from community chest cards pile

	}
	else if (this->m_type == e_Chance)
	{
		//ask the manager to let the player pick one card from chance cards pile

	}
	
	std::cout << "cCardDistrict::Actioon()" << std::endl;




	return true;
}

bool cCardDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "cCardDistrict::Response()" << std::endl;
	return true;
}

