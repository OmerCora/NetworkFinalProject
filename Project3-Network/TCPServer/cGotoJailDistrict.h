#pragma once

#include "cDistrict.h"

class cGotoJailDistrict : public cDistrict
{
public:
	cGotoJailDistrict(int districtID);
	virtual ~cGotoJailDistrict();

	// begin of iDistrict
	virtual bool Action(iPlayer* player, iLogicMonopolyMediator& logic);
	virtual bool Response(iPlayer* player, iLogicMonopolyMediator& logic);
	// end of iDistrict
};

