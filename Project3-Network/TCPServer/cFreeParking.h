#pragma once

#include "cDistrict.h"

class cFreeParking : public cDistrict
{
public:
	cFreeParking();
	~cFreeParking();

	// begin of iDistrict
	virtual bool Actioon();
	// end of iDistrict
};

