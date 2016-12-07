#pragma once

#include "cDistrict.h"

class cJailDistrict : public cDistrict
{
public:
	cJailDistrict(int districtID);
	virtual ~cJailDistrict();

	// begin of iDistrict
	virtual sProtocolDistrictInfo::eDistrictType DistrictType();
	virtual bool Action(iPlayer* player, iLogicMonopolyMediator& logic);
	virtual bool Response(iPlayer* player, iLogicMonopolyMediator& logic);
	// end of iDistrict
};

