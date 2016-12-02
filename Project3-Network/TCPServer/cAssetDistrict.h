#pragma once

#include "cDistrict.h"

class cAssetDistrict : public cDistrict
{
public:
	cAssetDistrict(int districtID);
	virtual ~cAssetDistrict();


	// begin of cAssetDistrict
	virtual bool SetSold();
	virtual bool SetOwner();
	virtual unsigned int GetPrice();
	// begin of cAssetDistrict

protected:
	iPlayer* m_owner;

};

