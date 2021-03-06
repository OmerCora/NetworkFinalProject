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
	virtual sProtocolDistrictInfo::eDistrictType DistrictType();
	virtual bool Action(iPlayer* player, iLogicMonopolyMediator& logic);
	virtual bool Response(iPlayer* player, iLogicMonopolyMediator& logic);
	// end of iDistrict


private:
	eTaxDistrictType m_taxtype;
};

