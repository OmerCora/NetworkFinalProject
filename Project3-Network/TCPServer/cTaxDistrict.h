#pragma once

#include "cDistrict.h"

class cTaxDistrict : public cDistrict
{
public:
	cTaxDistrict();
	~cTaxDistrict();

	// begin of iDistrict
	virtual bool Actioon();
	// end of iDistrict

};

