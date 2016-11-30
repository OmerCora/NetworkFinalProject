#ifndef _cTCPModuleClient_HG_
#define _cTCPModuleClient_HG_

#include "cBuffer.h"
#include "cTCPModule.h"

class cTCPModuleClient : public cTCPModule
{
protected:
	cTCPModuleClient();
public:
	virtual ~cTCPModuleClient();

protected:
	// Attempt to connect to an address until one succeeds
	bool ConnectToServer(const char* ipaddr, const char* port);

	// begin of cTCPModule
	virtual bool Start(const char* ipaddr, const char* port);
	virtual bool Run();
	virtual bool ShutDown();

	virtual bool GetAddressInfo(addrinfo*& outAddressInfo, const char* ipaddr, const char* port);
	virtual bool Disconnect(const SOCKET connectedSocket);
	// end of cTCPModule



	cBuffer m_receiveBuffer;
};

#endif