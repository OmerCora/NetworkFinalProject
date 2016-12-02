#include "cTaxDistrict.h"

#include <iostream>

cTaxDistrict::cTaxDistrict(eTaxDistrictType taxtype)
	:m_taxtype(taxtype)
{
}


cTaxDistrict::~cTaxDistrict()
{
}
bool cTaxDistrict::Actioon()
{

	if (this->m_taxtype == e_Normal)
	{
		//ask the manager to let the player pay $100 to the bank
		//                or just -100 from the playe

	}
	else if (this->m_taxtype == e_Luxury)
	{
		//ask the manager to to let the player pay $200 to the bank

	}


	std::cout << "cTaxDistrict::Actioon()" << std::endl;
	return true;
}
