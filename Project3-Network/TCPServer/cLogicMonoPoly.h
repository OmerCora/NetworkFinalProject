#pragma once

#include "iDistrict.h"
#include "iLogicMonopoly.h"
#include "iDice.h"
#include "iPlayer.h"
#include "iCardStorage.h"
#include "iPacketProcedureMonopoly.h"

#include <vector>


class cLogicMonoPoly : public iLogicMonopoly, public iLogicMonopolyMediator
{
public:
	cLogicMonoPoly();
	virtual ~cLogicMonoPoly();

	virtual iPacketProcedureMonopoly& PacketProcedure();
	virtual iPlayer& PlayerA();
	virtual iPlayer& PlayerB();

	virtual void SetState(ePlayState state);
	virtual bool PlayGame(iUser* userA, iUser* userB);
	virtual bool UpdateGameLoop();
	virtual bool IsGameOver();
	virtual void BringToStart(iPlayer* player);

	virtual void ProcessReceivedPlayData(cBuffer& receiveBuffer);
	virtual void ProcessReceiveData(cBuffer& receiveBuffer);

	virtual void SetLastReceivedProtocol(iProtocol* protocol);
	virtual iProtocol* GetLastReceivedProtocol();

private:
	//std::thread GameLoopThread();
	bool CleanUp();

	void GetBoardInfo(sProtocolBoardInfo& outInfo);
	//void GetPlayersInfo(sProtocolResponseGameStart& outInfo);


	iDice* m_dice;

	iPlayer* m_players[2];
	// 0 or 1
	int m_currentPlayerIndex;
	int m_nextLocation;

	iCardStorage* m_communityStorage;
	iCardStorage* m_chanceStorage;

	const int m_jailDistrictID = 10;
	std::vector<iDistrict*> m_districts;


	bool IsCurrentState(ePlayState state);
	bool IsPriorState(ePlayState state);
	ePlayState m_playState;
	ePlayState m_priorState;

	iPacketProcedureMonopoly* m_packetProcedure;
	iProtocol* m_lastReceivedData;
};

