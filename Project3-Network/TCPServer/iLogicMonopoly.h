#pragma once

#include <cBuffer.h>

class iLogicMonopoly
{
public:
	virtual ~iLogicMonopoly() {}

	virtual bool PlayGame(iUser* userA, iUser* userB) = 0;
	virtual bool UpdateGameLoop() = 0;
	virtual bool IsGameOver() = 0;

	virtual void ProcessReceivedPlayData(cBuffer& receiveBuffer)= 0;

};

