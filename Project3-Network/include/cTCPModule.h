#ifndef _cTCPModule_HG_
#define _cTCPModule_HG_


#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include "cBuffer.h"

class cTCPModule
{
protected:
	cTCPModule();
public:
	virtual ~cTCPModule();

protected:

	// begin of cTCPModule
	virtual bool Start(const char* ipaddr, const char* port) = 0;
	virtual bool Run() = 0;
	virtual bool ShutDown() = 0;

	virtual bool ProcessReceivePackets(SOCKET socket, cBuffer& receiverBuffer) = 0;
	virtual bool Respond() = 0;
	virtual bool GetAddressInfo(addrinfo*& outAddressInfo, const char* ipaddr, const char* port) = 0;
	virtual bool Disconnect(const SOCKET connectedSocket) = 0;
	// end of cTCPModule

	//Step 1
	// Initialize Winsock
	bool Initialize();

	//Step 2: Create a Socket
	bool CreateSocket(const addrinfo& addressInfo);
	bool SetNonBlockingMode();

	//Step 6: Receiveing and sending data
	// reads from the incoming steam
	// Receive until the peer closes the connection
	bool ReceiveData(const SOCKET from, cBuffer& buffer);
	// writes to the outgoing stream
	bool SendData(const SOCKET to);



	// one is only sender, the other is only receiver
	SOCKET m_socket;
	//cBuffer m_receiveBuffer;
	cBuffer m_sendBuffer;

	bool m_isConnected;
	bool m_isStopped;
	bool m_isDebugMode;
};

#endif