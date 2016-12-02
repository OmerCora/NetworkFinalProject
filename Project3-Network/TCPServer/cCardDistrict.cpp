#include "cCardDistrict.h"

#include <iostream>


cCardDistrict::cCardDistrict(eCardDistrictType type)
	:m_type(type)
{

}

cCardDistrict::~cCardDistrict()
{
}

bool cCardDistrict::Actioon()
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