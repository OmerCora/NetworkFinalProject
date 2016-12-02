#include "cPlayer.h"



cPlayer::cPlayer(iUser* user)
	:m_money(0)
	, m_isReady(true)
	, m_user(user)
{
}


cPlayer::~cPlayer()
{
}

bool cPlayer::IsReadyToPlay()
{
	return m_isReady;
}

bool cPlayer::IsBankrupty() { return (m_money<=0); }
bool cPlayer::Debit(unsigned int money)
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