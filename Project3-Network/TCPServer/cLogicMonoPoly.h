#pragma once

#include "iDistrict.h"
#include "iLogicMonopoly.h"
#include "iDice.h"
#include "iPlayer.h"
#include "iCardStorage.h"

#include <vector>
#include <thread>

class cLogicMonoPoly : public iLogicMonopoly
{
public:
	cLogicMonoPoly();
	virtual ~cLogicMonoPoly();


	virtual bool PlayGame(iUser* userA, iUser* userB);

private:
	std::thread GameLoopThread();
	bool GameLoop();
	bool CleanUp();

	iDice* m_dice;

	iPlayer* m_playerA;
	iPlayer* m_playerB;

	iCardStorage* m_communityStorage;
	iCardStorage* m_chanceStorage;

	const int m_numAssetDistricts = 23;// 6 + 6 +6+5
	const int m_numCommunityDistricts = 3;
	const int m_numChanceDistricts = 3;
	std::vector<iDistrict*> m_districts;

	enum ePlayState
	{
		e_Stop,
		e_Ready,
		e_Start,
		e_Wait,
		e_Finish,
	};

	ePlayState m_playState;

	std::thread m_gameLoopThread;
};

