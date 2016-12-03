#include "MonopolyGlobal.h"

#include "cPlayer.h"


#include <conio.h>
#include <iostream>

cPlayer::cPlayer(iUser* user)
	:m_money(1500)
	, m_isReady(true)
	, m_user(user)
	, m_chanceToThrowDice(false)
	, m_currentLocation(-1)
	, m_enjailedCount(0)
{
}


cPlayer::~cPlayer()
{
}

iUser* cPlayer::User() { return m_user; }


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

void cPlayer::SetEnjailCount(int numOfTurns)
{
	this->m_enjailedCount = numOfTurns;
}

int cPlayer::getEnjailCount()
{
	return this->m_enjailedCount;
}

bool cPlayer::getbCanThrowDice()
{
	return false;
}

void cPlayer::setbCanThrowDice(bool bChance)
{
}

