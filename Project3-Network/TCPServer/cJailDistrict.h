#pragma once

#include "cDistrict.h"

class cJailDistrict : public cDistrict
{
public:
	cJailDistrict();
	~cJailDistrict();

	// begin of iDistrict
	virtual bool Actioon();
	// end of iDistrict
};

