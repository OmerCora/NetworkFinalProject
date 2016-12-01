#include "cDistrict.h"

#include <iostream>


cDistrict::cDistrict()
{
}


cDistrict::~cDistrict()
{
}
bool cDistrict::AddPlayer(iUser* user)
{
	std::cout << "cDistrict::AddPlayer()" << std::endl;

	m_currentUser.push_back(user);

	return true;
}
bool cDistrict::RemovePlayer(iUser* user)
{
	std::cout << "cDistrict::RemovePlayer()" << std::endl;

	std::vector<iUser*>::iterator foundIter = std::find(m_currentUser.begin(), m_currentUser.end(), user);
	if (foundIter != m_currentUser.end())
	{
		m_currentUser.erase(foundIter);
	}


	return true;
}