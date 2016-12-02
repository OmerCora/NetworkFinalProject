#include "cAssetDistrict.h"

#include <iostream>


cAssetDistrict::cAssetDistrict(int districtID)
	:cDistrict(districtID)
{
}


cAssetDistrict::~cAssetDistrict()
{
}

bool cAssetDistrict::SetSold()
{
	std::cout << "cAssetDistrict::SetSold()" << std::endl;
	return true;
}
bool cAssetDistrict::SetOwner()
{
	std::cout << "cAssetDistrict::SetOwner()" << std::endl;
	return true;
}
unsigned int cAssetDistrict::GetPrice()
{
	std::cout << "cAssetDistrict::GetPrice()" << std::endl;
	return true;
}
