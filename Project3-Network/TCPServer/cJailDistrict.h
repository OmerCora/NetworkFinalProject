#pragma once

#include "cDistrict.h"

class cJailDistrict : public cDistrict
{
public:
	cJailDistrict(int districtID);
	virtual ~cJailDistrict();

	// begin of iDistrict
	virtual bool Action(iPlayer* player, iLogicMonopolyMediator& logic);
	// end of iDistrict
};

