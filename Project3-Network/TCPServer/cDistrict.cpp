#include "cDistrict.h"

#include <iostream>


cDistrict::cDistrict(int districtID)
	:m_districtID(districtID)
{
}

cDistrict::~cDistrict()
{
}
bool cDistrict::AddPlayer(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "cDistrict::AddPlayer()" << std::endl;

	if (m_currentPlayers)
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
	std::cout << "cDistrict::RemovePlayer()" << std::endl;

	m_currentPlayers = 0;

	return true;
}