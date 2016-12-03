#pragma once

#include "cDistrict.h"

class cFreeParking : public cDistrict
{
public:
	cFreeParking(int districtID);
	virtual ~cFreeParking();

	// begin of iDistrict
	virtual sProtocolMonopolyHeader::eDistrictType DistrictType();
	virtual bool Action(iPlayer* player, iLogicMonopolyMediator& logic);
	virtual bool Response(iPlayer* player, iLogicMonopolyMediator& logic);
	// end of iDistrict
};

