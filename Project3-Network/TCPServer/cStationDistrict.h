#pragma once

#include "cAssetDistrict.h"

class cStationDistrict : public cAssetDistrict
{
public:
	cStationDistrict(int districtID);
	virtual ~cStationDistrict();


	// begin of iDistrict
	virtual bool Action(iPlayer* player, iLogicMonopolyMediator& logic);
	virtual bool Response(iPlayer* player, iLogicMonopolyMediator& logic);
	// end of iDistrict
};

