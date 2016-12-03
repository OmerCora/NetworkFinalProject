#include "MonopolyGlobal.h"

#include "cPlayer.h"


#include <conio.h>
#include <iostream>

cPlayer::cPlayer(iUser* user, char playerID)
	:m_money(1500)
	, m_isReady(true)
	, m_user(user)
	, m_chanceToThrowDice(false)
	, m_currentLocation(-1)
	, m_enjailedCount(0)
	, m_playerID(playerID)
{
}


cPlayer::~cPlayer()
{
}

iUser* cPlayer::User() { return m_user; }
char cPlayer::PlayerID() { return m_playerID; }


bool cPlayer::IsReadyToPlay()
{
	return m_isReady;
}

bool cPlayer::IsBankrupty() { return (m_money<=0); }
bool cPlayer::Deposit(unsigned int money)
{
	if (IsBankrupty()) return false;
	m_money += money;
	return true;
}
bool cPlayer::Withdraw(unsigned int money)
{
	if (IsBankrupty()) return false;
	m_money -= money;
	return true;
}
void cPlayer::CurrentLocation(int location)
{
	m_currentLocation = location;
}
int cPlayer::CurrentLocation()
{
	return m_currentLocation;
}
bool cPlayer::HasChanceToThrowDice()
{
	if (m_chanceToThrowDice)
	{
		m_chanceToThrowDice = false;
		return true;
	}
	return false;
}
void cPlayer::TakeChanceToThrowDice()
{
	m_chanceToThrowDice = true;
}

int cPlayer::getEnjailCount()
{
	return this->m_enjailedCount;
}

void cPlayer::SetEnjailCount(int numOfTurns)
{
	this->m_enjailedCount = numOfTurns;
}


bool cPlayer::getbCanThrowDice()
{
	return this->m_CanThrowDice;
}		   

void cPlayer::setbCanThrowDice(bool bChance)
{
	this->m_CanThrowDice = bChance;
}

bool cPlayer::getbJustCameInJail()
{
	return this->m_bJustCameInJail;
}

void cPlayer::setbJustCameInJail(bool bFirstTime)
{
	this->m_bJustCameInJail = bFirstTime;
}

void cPlayer::GetPlayerInfo(sProtocolPlayerInfo& outInfo)
{
	outInfo.id = m_user->SocketID();
	outInfo.nick.name = m_user->Email();
	outInfo.money = m_money;
	outInfo.location = m_currentLocation;
}
