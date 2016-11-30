#ifndef _cTCPAuthenticationServer_HG_
#define _cTCPAuthenticationServer_HG_


#include <cTCPModuleServer.h>
#include "AuthenticationDef.pb.h"
#include "cDBAccessor.h"

class cTCPAuthenticationServer : public cTCPModuleServer
{
	cTCPAuthenticationServer();
	~cTCPAuthenticationServer();
	static cTCPAuthenticationServer* m_instance;
public:
	static cTCPAuthenticationServer& GetInstance();
	static void DeleteInstance();

	bool ExecuteServer();

private:
	// begin of cTCPModule
	virtual bool ProcessReceivePackets(SOCKET socket, cBuffer& receiverBuffer);
	virtual bool Respond();
	// end of cTCPModule

	bool SendSuccess(SOCKET client, sProtocolHeader::ePacketID packetID, google::protobuf::uint64 requestID);
	bool SendFailure(SOCKET client, google::protobuf::uint64 requestID,
		ProtocolAuthentication::CreateAccountWebFailure_e_Reason reason);
	bool SendFailure(SOCKET client, google::protobuf::uint64 requestID,
		ProtocolAuthentication::AuthenticateWebFailure_e_Reason reason);

	// when console close event occur, it make sure the clean up every thing before console shutdown
	static BOOL WINAPI ConsoleHandler(DWORD CEvent);

	sProtocolHeader m_headerSendBuff;
	//sProtocolResponseFailure m_failureSendBuff;

	sProtocolHeader m_headerRecvBuff;
	sProtocolAccount m_accountInfoRecvBuff;

	static const char* mc_DEFAULT_PORT;

	cDBAccessor m_authenticationDB;
};

#endif