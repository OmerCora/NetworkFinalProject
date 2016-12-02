#pragma once

#include "cAssetDistrict.h"

class cUtilityDistrict : public cAssetDistrict
{
public:
	cUtilityDistrict(int districtID);
	virtual ~cUtilityDistrict();


	// begin of iDistrict
	virtual bool Action(iPlayer* player, iLogicMonopolyMediator& logic);
	// end of iDistrict

};

