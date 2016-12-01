#pragma once

#include "cDistrict.h"

class cCardDistrict : public cDistrict
{
public:
	cCardDistrict();
	~cCardDistrict();

	// begin of iDistrict
	virtual bool Actioon();
	// end of iDistrict
};

