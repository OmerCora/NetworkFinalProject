#include "MonopolyGlobal.h"
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

#include "cPacketProcedureMonopoly.h"

#include <conio.h>
#include <iostream>


cLogicMonoPoly::cLogicMonoPoly()
	:m_playState(ePlayState::e_Wait)
	, m_dice(0)
	, m_communityStorage(0)
	, m_chanceStorage(0)
	, m_currentPlayerIndex(0)
	, m_nextLocation(0)
	, m_packetProcedure(0)
{
	m_players[0] = 0;
	m_players[1] = 0;
}

cLogicMonoPoly::~cLogicMonoPoly()
{
}

bool cLogicMonoPoly::PlayGame(iUser* userA, iUser* userB)
{
	m_packetProcedure = new cPacketProcedureMonopoly(*this);


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
		m_districts.push_back(new cBuildingDistrict(districtID++, 50));
		m_districts.push_back(new cCardDistrict(districtID++,cCardDistrict::e_Community));
		m_districts.push_back(new cBuildingDistrict(districtID++, 100));
		m_districts.push_back(new cTaxDistrict(districtID++, cTaxDistrict::e_Normal));
		m_districts.push_back(new cStationDistrict(districtID++, 250));
		m_districts.push_back(new cBuildingDistrict(districtID++, 150));
		m_districts.push_back(new cCardDistrict(districtID++, cCardDistrict::e_Chance));
		m_districts.push_back(new cBuildingDistrict(districtID++, 200));
		m_districts.push_back(new cBuildingDistrict(districtID++, 220));
	}
	// left line
	{
		m_districts.push_back(new cJailDistrict(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++, 240));
		m_districts.push_back(new cUtilityDistrict(districtID++, 200));
		m_districts.push_back(new cBuildingDistrict(districtID++, 300));
		m_districts.push_back(new cBuildingDistrict(districtID++, 350));
		m_districts.push_back(new cStationDistrict(districtID++, 250));
		m_districts.push_back(new cBuildingDistrict(districtID++, 400));
		m_districts.push_back(new cCardDistrict(districtID++, cCardDistrict::e_Community));
		m_districts.push_back(new cBuildingDistrict(districtID++, 450));
		m_districts.push_back(new cBuildingDistrict(districtID++, 550));
	}
	// top line
	{
		m_districts.push_back(new cFreeParking(districtID++));
		m_districts.push_back(new cBuildingDistrict(districtID++, 700));
		m_districts.push_back(new cCardDistrict(districtID++, cCardDistrict::e_Chance));
		m_districts.push_back(new cBuildingDistrict(districtID++, 750));
		m_districts.push_back(new cBuildingDistrict(districtID++, 800));
		m_districts.push_back(new cStationDistrict(districtID++, 250));
		m_districts.push_back(new cBuildingDistrict(districtID++, 850));
		m_districts.push_back(new cBuildingDistrict(districtID++, 900));
		m_districts.push_back(new cUtilityDistrict(districtID++, 200));
		m_districts.push_back(new cBuildingDistrict(districtID++, 1050));
	}
	// right line
	{
		m_districts.push_back(new cGotoJailDistrict(districtID++, *m_districts[m_jailDistrictID]));
		m_districts.push_back(new cBuildingDistrict(districtID++, 1200));
		m_districts.push_back(new cBuildingDistrict(districtID++, 1300));
		m_districts.push_back(new cCardDistrict(districtID++, cCardDistrict::e_Community));
		m_districts.push_back(new cBuildingDistrict(districtID++, 1500));
		m_districts.push_back(new cStationDistrict(districtID++, 250));
		m_districts.push_back(new cCardDistrict(districtID++, cCardDistrict::e_Chance));
		m_districts.push_back(new cBuildingDistrict(districtID++, 2000));
		m_districts.push_back(new cTaxDistrict(districtID++, cTaxDistrict::e_Luxury ));
		m_districts.push_back(new cBuildingDistrict(districtID++, 2200));
	}

	this->SetState(ePlayState::e_Start);
	std::cout << "ePlayState::e_Start" << std::endl;

	// add to start district
	m_districts[0]->AddPlayer(m_players[0], *this);
	m_districts[0]->AddPlayer(m_players[1], *this);

	// decide the first player with a dice
	m_currentPlayerIndex = m_dice->Throw() % 2;

	// send a packet to notice starting game
	{
		m_packetProcedure->SetHeader(sProtocolMonopolyHeader::e_ResponseGameStart);
		sProtocolResponseGameStart protocol;
		this->GetPlayersInfo(protocol);
		m_packetProcedure->AppendProtocol(protocol);

		m_packetProcedure->SendData(m_players[0]->User()->SocketID());
		m_packetProcedure->SendData(m_players[1]->User()->SocketID());
	}

	this->SetState(ePlayState::e_Wait);

	return true;
}

void cLogicMonoPoly::GetPlayersInfo(sProtocolResponseGameStart& outInfo)
{
	outInfo.playerA.isMyTurn = m_currentPlayerIndex == 0 ? 1 : 0;
	outInfo.playerB.isMyTurn = m_currentPlayerIndex == 1 ? 1 : 0;

	m_players[0]->GetPlayerInfo(outInfo.playerA);
	m_players[1]->GetPlayerInfo(outInfo.playerB);

}
void cLogicMonoPoly::BringToStart(iPlayer* player)
{
	m_districts[0]->AddPlayer(player, *this);
}



bool cLogicMonoPoly::UpdateGameLoop()
{
	if (this->IsCurrentState(ePlayState::e_Wait))
	{
		std::cout << "ePlayState::e_Wait" << std::endl;

#ifdef _LOGIC_DEBUG_TEST
		std::cout << "(Press Any Key to Continue...)" << std::endl;
		char anyKey = _getch();

		if (this->IsPriorState(ePlayState::e_Start))
		{
			this->SetState(ePlayState::e_ThrowDice);
		}
		else if (this->IsPriorState(ePlayState::e_ThrowDice))
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
#endif
	}
	else if (this->IsCurrentState(ePlayState::e_ThrowDice))
	{
		std::cout << "ePlayState::e_ThrowDice" << std::endl;


		// 1. throw dice
		if(m_players[m_currentPlayerIndex]->getbCanThrowDice())
		{
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
		}

		// TODO: it will vary two different protocol(1. Throw Dice, 2. Pass)
		// send dice result
		{
			m_packetProcedure->SetHeader(sProtocolMonopolyHeader::e_ResponsePlayThrowDice);
			sProtocolResponsePlayThrowDice protocol;
			protocol.nextLocation = m_nextLocation;
			m_packetProcedure->AppendProtocol(protocol);

			m_packetProcedure->SendData(m_players[0]->User()->SocketID());
			m_packetProcedure->SendData(m_players[1]->User()->SocketID());
		}


		this->SetState(ePlayState::e_Wait);
	}
	else if (this->IsCurrentState(ePlayState::e_Action))
	{
		std::cout << "ePlayState::e_Action" << std::endl;

		// response each action state at the each district action
		m_districts[m_players[m_currentPlayerIndex]->CurrentLocation()]->Action(m_players[m_currentPlayerIndex], *this);

		if (m_districts[m_players[m_currentPlayerIndex]->CurrentLocation()]->IsRequiredAnswer())
		{
			this->SetState(ePlayState::e_Wait);
		}
		else
		{
			this->SetState(ePlayState::e_Response);
		}
	}
	else if (this->IsCurrentState(ePlayState::e_ReceiveAnswer))
	{
		std::cout << "ePlayState::e_ReceiveAnswer" << std::endl;

		// response each action state at the each district action
		m_districts[m_players[m_currentPlayerIndex]->CurrentLocation()]->ReceiveAnswer(m_players[m_currentPlayerIndex], *this);

		this->SetState(ePlayState::e_Response);
	}
	else if (this->IsCurrentState(ePlayState::e_Response))
	{
		std::cout << "ePlayState::e_Response" << std::endl;

		m_districts[m_players[m_currentPlayerIndex]->CurrentLocation()]->Response(m_players[m_currentPlayerIndex], *this);
		this->SetState(ePlayState::e_ChangeTurn);
	}
	else if (this->IsCurrentState(ePlayState::e_ChangeTurn))
	{
		std::cout << "ePlayState::e_ChangeTurn" << std::endl;

		if (!m_players[m_currentPlayerIndex]->HasChanceToThrowDice())
		{
			this->SetState(ePlayState::e_Wait);


			// 4. turn end of the player
			m_currentPlayerIndex = m_currentPlayerIndex == 0 ? 1 : 0;

			// send turn change information
			{
				m_packetProcedure->SetHeader(sProtocolMonopolyHeader::e_ResponsePlayTurnChange);
				sProtocolResponsePlayTurnChange protocol;
				m_packetProcedure->AppendProtocol(protocol);

				m_packetProcedure->SendData(m_players[0]->User()->SocketID());
				m_packetProcedure->SendData(m_players[1]->User()->SocketID());
			}
		}
		else
		{
			this->SetState(ePlayState::e_Wait);

			// send turn keep information
			{
				m_packetProcedure->SetHeader(sProtocolMonopolyHeader::e_ResponsePlayTurnKeep);
				sProtocolResponsePlayTurnChange protocol;
				m_packetProcedure->AppendProtocol(protocol);

				m_packetProcedure->SendData(m_players[0]->User()->SocketID());
				m_packetProcedure->SendData(m_players[1]->User()->SocketID());
			}
		}
	}

	// 5. check end of game
	if (!this->IsCurrentState(ePlayState::e_Finish))
	{
		if (m_players[0]->IsBankrupty() || m_players[1]->IsBankrupty())
		{
			std::cout << "ePlayState::e_Finish" << std::endl;

			this->SetState(ePlayState::e_Finish);

			// TODO: record rate of current play
			{
			}

			// send result to the rating server
			{
				m_packetProcedure->SetHeader(sProtocolMonopolyHeader::e_ResponseGameFinish);
				sProtocolResponseGameFinish protocol;
				m_packetProcedure->AppendProtocol(protocol);

				m_packetProcedure->SendData(m_players[0]->User()->SocketID());
				m_packetProcedure->SendData(m_players[1]->User()->SocketID());
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
iPacketProcedureMonopoly& cLogicMonoPoly::PacketProcedure() { return *m_packetProcedure; }
iPlayer& cLogicMonoPoly::PlayerA() { return *m_players[0]; }
iPlayer& cLogicMonoPoly::PlayerB() { return *m_players[1]; }
void cLogicMonoPoly::SetState(ePlayState state)
{
	m_priorState = m_playState;
	m_playState = state;
}

bool cLogicMonoPoly::CleanUp()
{
	delete m_packetProcedure;

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

	m_packetProcedure->SetHeader(sProtocolMonopolyHeader::e_ResponseGameOver);
	sProtocolResponseGameOver protocol;
	m_packetProcedure->AppendProtocol(protocol);

	m_packetProcedure->SendData(m_players[0]->User()->SocketID());
	m_packetProcedure->SendData(m_players[1]->User()->SocketID());

	return true;
}

bool cLogicMonoPoly::IsGameOver()
{
	return this->IsCurrentState(ePlayState::e_GameOver);
}


void cLogicMonoPoly::ProcessReceivedPlayData(cBuffer& receiveBuffer)
{
	std::cout << "cLogicMonoPoly::ProcessReceivedPlayData" << std::endl;

	this->ProcessReceiveData(receiveBuffer);
}

void cLogicMonoPoly::ProcessReceiveData(cBuffer& receiveBuffer)
{
	sProtocolMonopolyHeader header;
	receiveBuffer.Deserialize(header);

	switch (header.packet_id)
	{
	case sProtocolMonopolyHeader::ePacketID::e_RequestPlayThrowDice:
	{
		std::cout << "e_RequestPlayDiceThrow" << std::endl;

		sProtocolRequestPlayThrowDice data;
		receiveBuffer.Deserialize(data);

		// change state
		this->SetState(iLogicMonopoly::e_ThrowDice);

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_RequestPlayAction:
	{
		std::cout << "e_RequestPlayAction" << std::endl;

		sProtocolRequestPlayAction data;
		receiveBuffer.Deserialize(data);

		this->SetState(iLogicMonopoly::e_Action);

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_AnswerAssetAction:
	{
		std::cout << "e_AnswerAssetAction" << std::endl;

		sProtocolAnswerAssetAction data;
		receiveBuffer.Deserialize(data);

		this->SetState(iLogicMonopoly::e_ReceiveAnswer);

		break;
	}
	default:
		break;
	}

}