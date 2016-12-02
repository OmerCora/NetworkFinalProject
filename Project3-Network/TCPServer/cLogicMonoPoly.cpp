#include "cLogicMonoPoly.h"

#include "cPlayer.h"
#include "cDice.h"
#include "cCardStorageChance.h"
#include "cCardStorageCommunity.h"

#include "cCardDistrict.h"
#include "cFreeParking.h"
#include "cGotoJailDistrict.h"
#include "cJailDistrict.h"
#include "cStartDistrict.h"
#include "cTaxDistrict.h"

#include "cAssetDistrict.h"
#include "cBuildingDistrict.h"
#include "cStationDistrict.h"
#include "cUtilityDistrict.h"


cLogicMonoPoly::cLogicMonoPoly()
	:m_playState(ePlayState::e_Stop)
{
}


cLogicMonoPoly::~cLogicMonoPoly()
{
}

bool cLogicMonoPoly::CleanUp()
{
	delete m_dice;

	delete m_playerA;
	delete m_playerB;

	delete m_communityStorage;
	delete m_chanceStorage;

	std::vector<iDistrict*>::iterator iter = m_districts.begin();
	for (; iter != m_districts.end(); ++iter)
	{
		iDistrict* p = *iter;
		delete p;
	}
	m_districts.clear();

	return true;
}

bool cLogicMonoPoly::PlayGame(iUser* userA, iUser* userB)
{
	m_dice = new cDice();

	m_playerA = new cPlayer(userA);
	m_playerB = new cPlayer(userB);

	m_communityStorage = new cCardStorageCommunity();
	m_chanceStorage = new cCardStorageChance();

	// create districts

	// bot line
	{
		m_districts.push_back(new cStartDistrict());
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cCardDistrict(cCardDistrict::e_Community));
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cTaxDistrict(cTaxDistrict::e_Normal));
		m_districts.push_back(new cStationDistrict());
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cCardDistrict(cCardDistrict::e_Chance));
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cAssetDistrict());
	}
	// left line
	{
		m_districts.push_back(new cJailDistrict());
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cUtilityDistrict());
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cStationDistrict());
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cCardDistrict(cCardDistrict::e_Community));
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cAssetDistrict());
	}
	// top line
	{
		m_districts.push_back(new cFreeParking());
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cCardDistrict(cCardDistrict::e_Chance));
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cStationDistrict());
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cUtilityDistrict());
		m_districts.push_back(new cAssetDistrict());
	}
	// right line
	{
		m_districts.push_back(new cGotoJailDistrict());
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cCardDistrict(cCardDistrict::e_Community));
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cStationDistrict());
		m_districts.push_back(new cCardDistrict(cCardDistrict::e_Chance));
		m_districts.push_back(new cAssetDistrict());
		m_districts.push_back(new cTaxDistrict(cTaxDistrict::e_Luxury ));
		m_districts.push_back(new cAssetDistrict());
	}

	m_playState = ePlayState::e_Ready;

	m_gameLoopThread = this->GameLoopThread();
	return true;
}
std::thread cLogicMonoPoly::GameLoopThread()
{
	return std::thread([this] { GameLoop(); });
}
bool cLogicMonoPoly::GameLoop()
{
	m_playState = ePlayState::e_Start;
	while (!m_playState != ePlayState::e_Finish)
	{
		//
	}

	this->CleanUp();

	return true;
}