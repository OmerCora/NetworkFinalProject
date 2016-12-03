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
		player->Withdraw(100);
	}
	else if (this->m_taxtype == e_Luxury)
	{
		player->Withdraw(200);
	}


	std::cout << "\t cTaxDistrict::Action()" << std::endl;
	return true;
}
bool cTaxDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cTaxDistrict::Response()" << std::endl;
	return true;
}
