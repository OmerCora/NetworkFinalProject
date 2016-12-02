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

	cCardDistrict(int districtID, eCardDistrictType type);
	virtual ~cCardDistrict();

	// begin of iDistrict
	virtual bool Action(iPlayer* player, iLogicMonopolyMediator& logic);
	// end of iDistrict

private:
	eCardDistrictType m_type;
};

