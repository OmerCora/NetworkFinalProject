#pragma once

#include "cDistrict.h"

class cGotoJailDistrict : public cDistrict
{
public:
	cGotoJailDistrict();
	~cGotoJailDistrict();

	// begin of iDistrict
	virtual bool Actioon();
	// end of iDistrict
};

