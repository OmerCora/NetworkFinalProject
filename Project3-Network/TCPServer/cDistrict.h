#pragma once

#include "iDistrict.h"
#include <vector>

class cDistrict : public iDistrict
{
public:
	cDistrict();
	virtual ~cDistrict();

	// begin of iDistrict
	virtual bool AddPlayer(iPlayer* user);
	virtual bool RemovePlayer(iPlayer* user);
	// end of iDistrict

protected:
	std::vector<iPlayer*> m_currentPlayers;
};

