#pragma once

#include "iDistrict.h"
#include "iLogicMonopoly.h"
#include "iDice.h"
#include "iPlayer.h"
#include "iCardStorage.h"

#include <vector>
#include <thread>

class cLogicMonoPoly : public iLogicMonopoly, public iLogicMonopolyMediator
{
public:
	cLogicMonoPoly();
	virtual ~cLogicMonoPoly();

	virtual bool PlayGame(iUser* userA, iUser* userB);
	virtual bool UpdateGameLoop();
	virtual bool IsGameOver();
	virtual void BringToStart(iPlayer* player);

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
};

