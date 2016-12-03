#include "MonopolyGlobal.h"
#include "cAssetDistrict.h"

#include <conio.h>
#include <iostream>


	this->m_price = price;
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
bool cAssetDistrict::SetOwner(iPlayer* player)
{
	this->m_owner = player;
	return true;
}
unsigned int cAssetDistrict::GetPrice()
{
	return this->price;
}
