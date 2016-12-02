#pragma once

#include "cDistrict.h"

class cCardDistrict : public cDistrict
{
public:
	enum eCardDistrictType
	{
		e_Community,
		e_Chance,
	};

	cCardDistrict(eCardDistrictType type);
	virtual ~cCardDistrict();

	// begin of iDistrict
	virtual bool Actioon();
	// end of iDistrict

private:
	eCardDistrictType m_type;
};

