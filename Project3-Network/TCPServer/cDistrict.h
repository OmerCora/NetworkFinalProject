#pragma once

#include "iDistrict.h"
#include <vector>

class cDistrict : public iDistrict
{
public:
	cDistrict();
	~cDistrict();

	// begin of iDistrict
	virtual bool AddPlayer(iUser* user);
	virtual bool RemovePlayer(iUser* user);
	// end of iDistrict

protected:
	std::vector<iUser*> m_currentUser;
};

