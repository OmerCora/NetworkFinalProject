#pragma once

#include "iDistrict.h"
#include "iLogicMonopoly.h"
#include "iDice.h"
#include "iPlayer.h"
#include "iCardStorage.h"
#include "iPacketProcedureMonopoly.h"

#include <vector>

#undef UNICODE
// exclude many header files of windows.h
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "cBuffer.h"


class cLogicMonoPoly : public iLogicMonopoly, public iLogicMonopolyMediator
{
public:
	cLogicMonoPoly();
	virtual ~cLogicMonoPoly();

	virtual bool PlayGame(iUser* userA, iUser* userB);
	virtual bool UpdateGameLoop();
	virtual bool IsGameOver();
	virtual void BringToStart(iPlayer* player);
	virtual void openCommunityCardTo(iPlayer* player);

	virtual void ProcessReceivedPlayData(cBuffer& receiveBuffer);

private:
	//std::thread GameLoopThread();
	bool CleanUp();

	iDice* m_dice;

	iPlayer* m_players[2];
	// 0 or 1
	int m_currentPlayerIndex;
	int m_nextLocation;

	iCardStorage* m_communityStorage;
	iCardStorage* m_chanceStorage;

	std::vector<iDistrict*> m_districts;

	enum ePlayState
	{
		e_Wait,
		e_Start,
		e_ThrowDice,
		e_Action,
		e_Response,
		e_ChangeTurn,
		e_Finish,
		e_GameOver,
	};

	bool IsCurrentState(ePlayState state);
	bool IsPriorState(ePlayState state);
	void SetState(ePlayState state);
	ePlayState m_playState;
	ePlayState m_priorState;

	iPacketProcedureMonopoly* m_packetProcedure;
};

