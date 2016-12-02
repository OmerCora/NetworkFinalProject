#pragma once

#include <thread>

class iLogicMonopoly
{
public:
	virtual ~iLogicMonopoly() {}

	virtual bool PlayGame(iUser* userA, iUser* userB) = 0;
	virtual bool UpdateGameLoop() = 0;
	virtual bool IsGameOver() = 0;
};

