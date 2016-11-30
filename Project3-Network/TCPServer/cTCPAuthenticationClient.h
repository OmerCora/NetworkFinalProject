#ifndef _cTCPAuthenticationClient_HG_
#define _cTCPAuthenticationClient_HG_


#include <cTCPModuleClient.h>
#include <AuthenticationDef.pb.h>
#include "iUser.h"
#include <thread>

class cTCPAuthenticationClient : public cTCPModuleClient
{
	cTCPAuthenticationClient();
	~cTCPAuthenticationClient();

	static cTCPAuthenticationClient* m_instance;
public:
	static cTCPAuthenticationClient& GetInstance();
	static void DeleteInstance();

	bool StartClient();
	bool EndClient();

	void StartSigninThread(iUser* user, sProtocolAccount& accountInfo);
	void StartLoginThread(iUser* user, sProtocolAccount& accountInfo);
	void RequestSignin(iUser* user, sProtocolAccount accountInfo);
	void RequestLogin(iUser* user, sProtocolAccount accountInfo);

private:
	// begin of cTCPModule
	virtual bool ProcessReceivePackets(SOCKET socket, cBuffer& receiverBuffer);
	virtual bool Respond();
	// end of cTCPModule

	sProtocolHeader m_headerSendBuff;
	sProtocolHeader m_headerRecvBuff;

	//sProtocolResponseFailure m_failureRecvBuff;

	std::thread m_receiveThread;

	static const char* mc_DEFAULT_IPADDRESS;
	static const char* mc_DEFAULT_PORT;
};

#endif