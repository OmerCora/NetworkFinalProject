#pragma once

#include "cDistrict.h"

class cAssetDistrict : public cDistrict
{
public:
	cAssetDistrict();
	virtual ~cAssetDistrict();

	// begin of iDistrict
	virtual bool Actioon();
	// end of iDistrict

	// begin of cAssetDistrict
	virtual bool SetSold();
	virtual bool SetOwner();
	virtual unsigned int GetPrice();
	// begin of cAssetDistrict

protected:
	iUser* m_owner;

};

