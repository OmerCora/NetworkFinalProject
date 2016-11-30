#include "cUser.h"

#include "sProtocols.h"


cUser::cUser(const sUserInfo& userInfo)
	: m_userInfo(userInfo)
	, m_loggedIn(false)
{
}


cUser::~cUser()
{
}
bool cUser::Compaire(iUser* user)
{
	cUser* castUser = dynamic_cast<cUser*>(user);
	return (this->m_userInfo.email == castUser->m_userInfo.email);
}

SOCKET cUser::SocketID()
{
	return m_userInfo.socket;
}
bool cUser::IsLoggedIn()
{
	return m_loggedIn;
}

DWORD cUser::ConnectionTime()
{
	return m_userInfo.connectTime;
}
const std::string& cUser::Email()
{
	return m_userInfo.email;
}
cBuffer& cUser::GetReceiveBuffer()
{
	return m_receiveBuffer;
}
void cUser::AssignLoginInfo(const sProtocolAccount& loginInfo)
{
	m_userInfo.email = loginInfo.username;
	m_loggedIn = true;
}
void cUser::Logout()
{
	m_userInfo.email = "guest";
	m_loggedIn = false;
}
