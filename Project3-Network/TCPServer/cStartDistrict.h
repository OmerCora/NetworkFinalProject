#pragma once

#include "cDistrict.h"

class cStartDistrict : public cDistrict
{
public:
	cStartDistrict(int districtID);
	~cStartDistrict();

	// begin of iDistrict
	virtual bool Action(iPlayer* player, iLogicMonopolyMediator& logic);
	virtual bool Response(iPlayer* player, iLogicMonopolyMediator& logic);
	// end of iDistrict
};

