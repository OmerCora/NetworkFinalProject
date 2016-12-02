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

	cTaxDistrict(int districtID, eTaxDistrictType taxtype);
	~cTaxDistrict();

	// begin of iDistrict
	virtual bool Action(iPlayer* player, iLogicMonopolyMediator& logic);
	// end of iDistrict


private:
	eTaxDistrictType m_taxtype;
};

