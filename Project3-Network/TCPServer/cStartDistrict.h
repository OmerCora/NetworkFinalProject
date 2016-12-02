#pragma once

#include "cDistrict.h"

class cStartDistrict : public cDistrict
{
public:
	cStartDistrict();
	~cStartDistrict();

	// begin of iDistrict
	virtual bool Actioon();
	// end of iDistrict
};

