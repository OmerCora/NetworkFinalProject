#include "cDistrict.h"

#include <iostream>


cDistrict::cDistrict()
{
}


cDistrict::~cDistrict()
{
}
bool cDistrict::AddPlayer(iPlayer* user)
{
	std::cout << "cDistrict::AddPlayer()" << std::endl;

	m_currentPlayers.push_back(user);

	return true;
}
bool cDistrict::RemovePlayer(iPlayer* user)
{
	std::cout << "cDistrict::RemovePlayer()" << std::endl;

	std::vector<iPlayer*>::iterator foundIter = std::find(m_currentPlayers.begin(), m_currentPlayers.end(), user);
	if (foundIter != m_currentPlayers.end())
	{
		m_currentPlayers.erase(foundIter);
	}


	return true;
}