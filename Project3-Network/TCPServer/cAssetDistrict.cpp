#include "MonopolyGlobal.h"
#include "cAssetDistrict.h"

#include <conio.h>
#include <iostream>


cAssetDistrict::cAssetDistrict(int districtID, unsigned int price)
	:cDistrict(districtID)
{
	this->m_price = price;
	this->m_owner = 0;
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
