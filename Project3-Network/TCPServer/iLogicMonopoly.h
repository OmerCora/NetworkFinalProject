#pragma once

#include <cBuffer.h>

class iLogicMonopoly
{
public:
	virtual ~iLogicMonopoly() {}

	enum ePlayState
	{
		e_Wait,
		e_Start,
		e_ThrowDice,
		e_Action,
		e_ReceiveAnswer,
		e_Response,
		e_ChangeTurn,
		e_Finish,
		e_GameOver,
	};

	virtual void SetState(ePlayState state) = 0;
	virtual bool PlayGame(iUser* userA, iUser* userB) = 0;
	virtual bool UpdateGameLoop() = 0;
	virtual bool IsGameOver() = 0;

	virtual void ProcessReceivedPlayData(cBuffer& receiveBuffer)= 0;

};

