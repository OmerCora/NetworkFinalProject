#include "MonopolyGlobal.h"
#include "cAssetDistrict.h"

#include <conio.h>
#include <iostream>


cAssetDistrict::cAssetDistrict(int districtID, unsigned int price)
	:cDistrict(districtID)
	, m_price(price)
	, m_owner(0)
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
	return this->m_price;
}
void cAssetDistrict::GetDistrictInfo(sProtocolDistrictInfo& outInfo)
{
	outInfo.districtType = this->DistrictType();
	outInfo.district_id = this->m_districtID;
	if(m_owner)
		outInfo.owner_id = m_owner->PlayerID();
	outInfo.price = m_price;
}