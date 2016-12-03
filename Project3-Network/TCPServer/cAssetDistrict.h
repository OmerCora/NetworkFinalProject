#pragma once

#include "cDistrict.h"

class cAssetDistrict : public cDistrict
{
public:
	cAssetDistrict(int districtID, unsigned int price);
	virtual ~cAssetDistrict();


	// begin of cAssetDistrict
	virtual bool SetSold();
	virtual bool SetOwner(iPlayer* player);
	virtual unsigned int GetPrice();
	virtual void GetDistrictInfo(sProtocolDistrictInfo& outInfo);
	// begin of cAssetDistrict

protected:
	iPlayer* m_owner;
	unsigned int m_price;
};

