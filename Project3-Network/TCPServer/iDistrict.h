#ifndef _iDistrict_HG_
#define _iDistrict_HG_

#include "iPlayer.h"
#include "iLogicMonopolyMediator.h"
#include <sProtocols.h>
#include <sProtocolsMonopoly.h>
#include <set>

class iDistrict
{
public:
	virtual ~iDistrict() {}


	// begin of iDistrict
	virtual sProtocolMonopolyHeader::eDistrictType DistrictType() = 0;
	virtual void GetDistrictInfo(sProtocolDistrictInfo& outInfo) = 0;
	virtual bool IsRequiredAnswer() = 0;
	virtual bool AddPlayer(iPlayer* player, iLogicMonopolyMediator& logic) = 0;
	virtual bool RemovePlayer(iPlayer* player, iLogicMonopolyMediator& logic) = 0;
	virtual bool Action(iPlayer* player, iLogicMonopolyMediator& logic) = 0;
	virtual bool ReceiveAnswer(iPlayer* player, iLogicMonopolyMediator& logic) = 0;
	virtual bool Response(iPlayer* player, iLogicMonopolyMediator& logic) = 0;
	// end of iDistrict

};

#endif