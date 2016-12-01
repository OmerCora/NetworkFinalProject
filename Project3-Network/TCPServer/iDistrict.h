#ifndef _iDistrict_HG_
#define _iDistrict_HG_

#include "iUser.h"
#include <sProtocols.h>
#include <set>

class iDistrict
{
public:
	virtual ~iDistrict() {}

	// begin of iDistrict
	virtual bool AddPlayer(iUser* user) = 0;
	virtual bool RemovePlayer(iUser* user) = 0;
	virtual bool Actioon() = 0;
	// end of iDistrict

};

#endif