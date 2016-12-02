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
	std::cout << "cCardDistrict::Actioon()" << std::endl;
	return true;
}