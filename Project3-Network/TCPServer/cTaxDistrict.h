#pragma once

#include "cDistrict.h"

class cTaxDistrict : public cDistrict
{
public:
	enum eTaxDistrictType
	{
		e_Normal,
		e_Luxury,
	};

	cTaxDistrict(eTaxDistrictType taxtype);
	~cTaxDistrict();

	// begin of iDistrict
	virtual bool Actioon();
	// end of iDistrict


private:
	eTaxDistrictType m_taxtype;
};

