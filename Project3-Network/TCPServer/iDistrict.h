#ifndef _iDistrict_HG_
#define _iDistrict_HG_

#include "iPlayer.h"
#include <sProtocols.h>
#include <set>

class iDistrict
{
public:
	virtual ~iDistrict() {}

	// begin of iDistrict
	virtual bool AddPlayer(iPlayer* user) = 0;
	virtual bool RemovePlayer(iPlayer* user) = 0;
	virtual bool Actioon() = 0;
	// end of iDistrict

};

#endif