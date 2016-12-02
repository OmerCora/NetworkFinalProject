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

#include "cBuildingDistrict.h"
#include "cStationDistrict.h"
#include "cUtilityDistrict.h"

#include <conio.h>
#include <iostream>

cLogicMonoPoly::cLogicMonoPoly()
	:m_playState(ePlayState::e_Wait)
	, m_dice(0)
	, m_communityStorage(0)
	, m_chanceStorage(0)
	, m_currentPlayerIndex(0)
	, m_nextLocation(0)
{
	m_players[0] = 0;
	m_players[1] = 0;
}

cLogicMonoPoly::~cLogicMonoPoly()
{
}

bool cLogicMonoPoly::PlayGame(iUser* userA, iUser* userB)
{
	m_dice = new cDice();

	m_players[0] = new cPlayer(userA);
	m_players[1] = new cPlayer(userB);

	m_communityStorage = new cCardStorageCommunity();
	m_chanceStorage = new cCardStorageChance();

	// create districts

	int districtID = 0;
	// bot line
	{
		m_districts.push_back(new cStartDistrict(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cCardDistrict(districtID++,cCardDistrict::e_Community));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cTaxDistrict(districtID++, cTaxDistrict::e_Normal));
		m_districts.push_back(new cStationDistrict(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cCardDistrict(districtID++, cCardDistrict::e_Chance));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++));
	}
	// left line
	{
		m_districts.push_back(new cJailDistrict(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cUtilityDistrict(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cStationDistrict(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cCardDistrict(districtID++, cCardDistrict::e_Community));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++));
	}
	// top line
	{
		m_districts.push_back(new cFreeParking(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cCardDistrict(districtID++, cCardDistrict::e_Chance));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cStationDistrict(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cUtilityDistrict(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++));
	}
	// right line
	{
		m_districts.push_back(new cGotoJailDistrict(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cCardDistrict(districtID++, cCardDistrict::e_Community));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cStationDistrict(districtID++));
		m_districts.push_back(new cCardDistrict(districtID++, cCardDistrict::e_Chance));
		m_districts.push_back(new cBuildingDistrict(districtID++));
		m_districts.push_back(new cTaxDistrict(districtID++, cTaxDistrict::e_Luxury ));
		m_districts.push_back(new cBuildingDistrict(districtID++));
	}

	this->SetState(ePlayState::e_Start);
	std::cout << "ePlayState::e_Start" << std::endl;

	// add to start district
	m_districts[0]->AddPlayer(m_players[0], *this);
	m_districts[0]->AddPlayer(m_players[1], *this);

	// 0. trun start of the player
	//m_playerLocations[0] = 0;
	//m_playerLocations[1] = 0;

	// decide the first player with a dice
	m_currentPlayerIndex = m_dice->Throw() % 2;

	// TODO: send a packet
	{
		// confirm to throw a dice
	}

	this->SetState(ePlayState::e_ThrowDice);

	return true;
}

void cLogicMonoPoly::BringToStart(iPlayer* player)
{
	m_districts[0]->AddPlayer(player, *this);
}

bool cLogicMonoPoly::UpdateGameLoop()
{
#if 1 // this is hard coding for test
	if (this->IsCurrentState(ePlayState::e_Wait))
	{
		std::cout << "ePlayState::e_Wait (Press Any Key to Continue...)" << std::endl;
		char anyKey = _getch();

		if (this->IsPriorState(ePlayState::e_ThrowDice))
		{
			this->SetState(ePlayState::e_Action);
		}
		else if (this->IsPriorState(ePlayState::e_Action))
		{
			this->SetState(ePlayState::e_Response);
		}
		else if (this->IsPriorState(ePlayState::e_Response))
		{
			this->SetState(ePlayState::e_ChangeTurn);
		}
		else if (this->IsPriorState(ePlayState::e_ChangeTurn))
		{
			this->SetState(ePlayState::e_ThrowDice);
		}
	}
	else
#endif
		if (this->IsCurrentState(ePlayState::e_ThrowDice))
	{
		std::cout << "ePlayState::e_ThrowDice" << std::endl;

		// 1. throw dice
		m_nextLocation = m_dice->Throw() + m_players[m_currentPlayerIndex]->CurrentLocation();
		if (m_nextLocation >= (int)m_districts.size())
			m_nextLocation = 0;

		// TODO: send packet of dice number
		{
		}

		// 2. remove the player from prior district
		// in the remove and add methods, packet will sended
		m_districts[m_players[m_currentPlayerIndex]->CurrentLocation()]->RemovePlayer(m_players[m_currentPlayerIndex], *this);
		// 3. add into a distict
		m_districts[m_nextLocation]->AddPlayer(m_players[m_currentPlayerIndex], *this);

		this->SetState(ePlayState::e_Wait);
	}
	else if (this->IsCurrentState(ePlayState::e_Action))
	{
		std::cout << "ePlayState::e_Action" << std::endl;

		m_districts[m_players[m_currentPlayerIndex]->CurrentLocation()]->Action(m_players[m_currentPlayerIndex], *this);
		this->SetState(ePlayState::e_Wait);
	}
	else if (this->IsCurrentState(ePlayState::e_Response))
	{
		std::cout << "ePlayState::e_Response" << std::endl;

		m_districts[m_players[m_currentPlayerIndex]->CurrentLocation()]->Response(m_players[m_currentPlayerIndex], *this);
		this->SetState(ePlayState::e_Wait);
	}
	else if (this->IsCurrentState(ePlayState::e_ChangeTurn))
	{
		std::cout << "ePlayState::e_ChangeTurn" << std::endl;

		if (!m_players[m_currentPlayerIndex]->HasChanceToThrowDice())
		{
			// 4. turn end of the player
			m_currentPlayerIndex = m_currentPlayerIndex == 0 ? 1 : 0;

			this->SetState(ePlayState::e_Wait);
		}
		else
		{
			this->SetState(ePlayState::e_ThrowDice);
		}
	}

	// 5. check end of game
	if (!this->IsCurrentState(ePlayState::e_Finish))
	{
		if (m_players[0]->IsBankrupty() || m_players[1]->IsBankrupty())
		{
			std::cout << "ePlayState::e_Finish" << std::endl;

			this->SetState(ePlayState::e_Finish);

			// TODO send result to the rating server
			{
			}

			this->CleanUp();
		}
	}

	return true;
}

bool cLogicMonoPoly::IsCurrentState(ePlayState state)
{
	return (m_playState== state);
}
bool cLogicMonoPoly::IsPriorState(ePlayState state)
{
	return (m_priorState == state);
}
void cLogicMonoPoly::SetState(ePlayState state)
{
	m_priorState = m_playState;
	m_playState = state;
}

bool cLogicMonoPoly::CleanUp()
{
	delete m_dice;

	delete m_players[0];
	delete m_players[1];

	delete m_communityStorage;
	delete m_chanceStorage;

	std::vector<iDistrict*>::iterator iter = m_districts.begin();
	for (; iter != m_districts.end(); ++iter)
	{
		iDistrict* p = *iter;
		delete p;
	}
	m_districts.clear();

	this->SetState(ePlayState::e_GameOver);
	std::cout << "ePlayState::e_GameOver" << std::endl;

	return true;
}

bool cLogicMonoPoly::IsGameOver()
{
	return this->IsCurrentState(ePlayState::e_GameOver);
}
