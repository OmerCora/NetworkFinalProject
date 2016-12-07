#include "MonopolyGlobal.h"
#include "cDistrict.h"

#include <conio.h>
#include <iostream>


cDistrict::cDistrict(int districtID)
	:m_districtID(districtID)
	, m_currentPlayers(0)
	, m_isRequiredAnswer(false)
{
}

cDistrict::~cDistrict()
{
}
void cDistrict::GetDistrictInfo(sProtocolDistrictInfo& outInfo)
{
	outInfo.districtType = this->DistrictType();
	outInfo.district_id = this->m_districtID;
	outInfo.owner_id = 0;
	outInfo.price = 0;
}

bool cDistrict::IsRequiredAnswer() { return m_isRequiredAnswer; }
bool cDistrict::AddPlayer(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cDistrict::AddPlayer() m_districtID: " << m_districtID << std::endl;

	if (m_currentPlayers && m_districtID!=0)
	{
		logic.BringToStart(m_currentPlayers);
		player->TakeChanceToThrowDice();
	}

	m_currentPlayers = player;
	player->CurrentLocation(m_districtID);

	// TODO: send packet
	{
	}

	return true;
}
bool cDistrict::RemovePlayer(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cDistrict::RemovePlayer() m_districtID: " << m_districtID  << std::endl;

	m_currentPlayers = 0;

	return true;
}
bool cDistrict::ReceiveAnswer(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cDistrict::ReceiveAnswer() m_districtID: " << m_districtID << std::endl;

	

	return true;
}


