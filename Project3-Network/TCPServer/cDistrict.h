#pragma once

#include "iDistrict.h"
#include <vector>

class cDistrict : public iDistrict
{
public:
	cDistrict(int districtID);
	virtual ~cDistrict();

	// begin of iDistrict
	virtual bool IsRequiredAnswer();
	virtual bool AddPlayer(iPlayer* player, iLogicMonopolyMediator& logic);
	virtual bool RemovePlayer(iPlayer* player, iLogicMonopolyMediator& logic);
	virtual bool ReceiveAnswer(iPlayer* player, iLogicMonopolyMediator& logic);
	// end of iDistrict

protected:
	int m_districtID;
	iPlayer* m_currentPlayers;

	bool m_isRequiredAnswer;
};

