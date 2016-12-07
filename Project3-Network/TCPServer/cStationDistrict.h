#pragma once

#include "cAssetDistrict.h"

class cStationDistrict : public cAssetDistrict
{
public:
	cStationDistrict(int districtID, unsigned int price);
	virtual ~cStationDistrict();


	// begin of iDistrict
	virtual sProtocolDistrictInfo::eDistrictType DistrictType();
	virtual bool Action(iPlayer* player, iLogicMonopolyMediator& logic);
	virtual bool ReceiveAnswer(iPlayer* player, iLogicMonopolyMediator& logic);
	virtual bool Response(iPlayer* player, iLogicMonopolyMediator& logic);
	// end of iDistrict
};

