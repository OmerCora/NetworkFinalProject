#ifndef _iDistrict_HG_
#define _iDistrict_HG_

#include "iPlayer.h"
#include "iLogicMonopolyMediator.h"
#include <sProtocols.h>
#include <set>

class iDistrict
{
public:
	virtual ~iDistrict() {}

	// begin of iDistrict
	virtual bool AddPlayer(iPlayer* player, iLogicMonopolyMediator& logic) = 0;
	virtual bool RemovePlayer(iPlayer* player, iLogicMonopolyMediator& logic) = 0;
	virtual bool Action(iPlayer* player, iLogicMonopolyMediator& logic) = 0;
	// end of iDistrict

};

#endif