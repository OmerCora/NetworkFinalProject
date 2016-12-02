#pragma once

#include "cAssetDistrict.h"

class cBuildingDistrict : public cAssetDistrict
{
public:
	cBuildingDistrict(int districtID);
	virtual ~cBuildingDistrict();


	// begin of iDistrict
	virtual bool Action(iPlayer* player, iLogicMonopolyMediator& logic);
	virtual bool Response(iPlayer* player, iLogicMonopolyMediator& logic);
	// end of iDistrict
};

