#ifndef _cTCPModuleServer_HG_
#define _cTCPModuleServer_HG_


#include "cTCPModule.h"
#include "cBuffer.h"
#include <map>

class cTCPModuleServer : public cTCPModule
{
protected:
	cTCPModuleServer();
public:
	virtual ~cTCPModuleServer();

protected:
	// begin of cTCPModule
	virtual bool Start(const char* ipaddr, const char* port);
	virtual bool Run();
	virtual bool ShutDown();

	virtual bool GetAddressInfo(addrinfo*& outAddressInfo, const char* ipaddr, const char* port);
	virtual bool Disconnect(const SOCKET connectedSocket);
	// end of cTCPModule

	//Step 3: Bind the Socket
	bool BindSocket(const addrinfo& addressInfo);
	//Step 4: Listen for a client connection
	bool ListenConnection();
	//Step 5: Accept a client's connection
	bool AcceptConnection();

	fd_set m_masterFileDiscriptor;    //Master list: // a set of file descriptors or in our cases sockets
	fd_set m_readFileDiscriptors;  //Temp list for select() statement

	int m_maximumFileDiscriptor;

	std::map<SOCKET, cBuffer> m_receiveBuffers;
};

#endif