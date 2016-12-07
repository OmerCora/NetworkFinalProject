#pragma once

#include "iPlayer.h"


class cPlayer : public iPlayer
{
public:
	cPlayer(iUser* user, char playerID);
	virtual ~cPlayer();

	virtual iUser* User();
	virtual char PlayerID();

	virtual bool IsReadyToPlay();

	virtual bool IsBankrupty();
	virtual int GetMoneyVariation();
	virtual bool Deposit(int money);
	virtual bool Withdraw(int money);
	virtual void CurrentLocation(int location);
	virtual int CurrentLocation();
	virtual bool HasChanceToThrowDice();
	virtual void TakeChanceToThrowDice();

	virtual int getEnjailCount(); 
	virtual void SetEnjailCount(int numOfTurns);

	virtual bool getbCanThrowDice();
	virtual void setbCanThrowDice(bool bChance);

	bool getbJustCameInJail();
	void setbJustCameInJail(bool bFirstTime);

	virtual void GetPlayerInfo(sProtocolPlayerInfo& outInfo, int currentPlayerID);


private:
	char m_playerID;

	int m_money;
	int m_previousMoney;
	int m_enjailedCount;

	int m_currentLocation;
	iUser* m_user;

	bool m_isReady;

	bool m_chanceToThrowDice;

	bool m_CanThrowDice;
	bool m_bJustCameInJail;

};

