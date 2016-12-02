#include "MonopolyGlobal.h"
#include "cTaxDistrict.h"

#include <conio.h>
#include <iostream>

cTaxDistrict::cTaxDistrict(int districtID, eTaxDistrictType taxtype)
	:cDistrict(districtID)
	, m_taxtype(taxtype)
{
}


cTaxDistrict::~cTaxDistrict()
{
}
bool cTaxDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{

	if (this->m_taxtype == e_Normal)
	{
		//ask the manager to take 100 from the player
		logic.takeMoneyFrom(100, player);
	}
	else if (this->m_taxtype == e_Luxury)
	{
		//ask the manager to to take 200 from the player
		logic.takeMoneyFrom(200, player);
	}


	std::cout << "\t cTaxDistrict::Action()" << std::endl;
	return true;
}
bool cTaxDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cTaxDistrict::Response()" << std::endl;
	return true;
}
