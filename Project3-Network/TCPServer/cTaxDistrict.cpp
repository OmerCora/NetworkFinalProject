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
	std::cout << "cTaxDistrict::Actioon()" << std::endl;
	return true;
}
