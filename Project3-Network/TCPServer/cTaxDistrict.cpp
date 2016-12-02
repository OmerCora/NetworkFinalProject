#include "cTaxDistrict.h"

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
	std::cout << "cTaxDistrict::Action()" << std::endl;
	return true;
}
