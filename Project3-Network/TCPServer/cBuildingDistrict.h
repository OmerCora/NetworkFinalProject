#pragma once

#include "cAssetDistrict.h"

class cBuildingDistrict : public cAssetDistrict
{
public:
	cBuildingDistrict(int districtID, int price);
	virtual ~cBuildingDistrict();


	// begin of iDistrict
	virtual sProtocolDistrictInfo::eDistrictType DistrictType();
	virtual bool Action(iPlayer* player, iLogicMonopolyMediator& logic);
	virtual bool ReceiveAnswer(iPlayer* player, iLogicMonopolyMediator& logic);
	virtual bool Response(iPlayer* player, iLogicMonopolyMediator& logic);
	// end of iDistrict
};

