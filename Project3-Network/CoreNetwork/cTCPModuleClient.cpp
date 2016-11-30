#include <stdio.h>
#include <iostream>
#include "cTCPModuleClient.h"

#include "cErrorReport.h"


cTCPModuleClient::cTCPModuleClient()
{
}


cTCPModuleClient::~cTCPModuleClient()
{
}


bool cTCPModuleClient::Start(const char* ipaddr, const char* port)
{
	//Step 1: Initial winsock
	if (!this->Initialize())
	{
		this->ShutDown();
		return false;
	}


	while (!m_isConnected && !m_isStopped)
	{
		if (!this->ConnectToServer(ipaddr, port))
			Sleep(100);
	}


	return true;
}

bool cTCPModuleClient::Run()
{
	while (!m_isStopped)
	{
		// 100 times per second for chance of message processing
		Sleep(1);

		//Step 4-1: Receiveing data
		if (!this->ReceiveData(m_socket, m_receiveBuffer))
		{
			// TODO: don't close socket. it should wait for next packet
			//if (!this->Disconnect(iClient))
			//{
			//	this->ShutDown();
			//	return false;
			//}
			continue;
		}


		// message framing and perform tasks
		this->ProcessReceivePackets(m_socket, m_receiveBuffer);

	}


	return true;
}


//Step 5-2: Cleanup
bool cTCPModuleClient::ShutDown()
{
	if (m_isDebugMode)
		std::cout << "Step 5: Disconnect Server" << std::endl;

	m_isStopped = true;

	// shutdown the send half of the connection since no more data will be sent
	// TODO: when server socket is shutdown, all the times, error occur. client sockets are not tested yet
	int iResult = shutdown(m_socket, SD_BOTH);//SD_SEND/SD_RECEIVE/SD_BOTH
	if (iResult == SOCKET_ERROR)
	{
		if (m_isDebugMode)
			cErrorReport::GetInstance().PrintWSAGetLastMessage("shutdown failed: ");
		return false;
	}


	if (!Disconnect(m_socket))
	{
		return false;
	}

	//Final clean up
	WSACleanup();

	return true;
}

bool cTCPModuleClient::GetAddressInfo(addrinfo*& outAddressInfo, const char* ipaddr, const char* port)
{
	//Convert hostnames and IP addresses between their string representations and the struct (binary) representation


	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // unspecified
	hints.ai_socktype = SOCK_STREAM; //Stream must be specified for TCP
									 // SOCK_STREAM: intact data delivery, higher overhead, use important game protocol
									 // SOCK_DGRAM: not timely delivery, responds many many requests at once very quickly, use less important game protocol
	hints.ai_protocol = IPPROTO_TCP; //Protocol is TCP

									 // This is for listening socket
									 //hints.ai_flags = AI_PASSIVE;// Socket address will be used in bind() call

	int resultInt = getaddrinfo(ipaddr, port, &hints, &outAddressInfo);
	if (resultInt != 0)
	{
		if (m_isDebugMode)
			cErrorReport::GetInstance().PrintWSAGetLastMessage("Socket Initalization failed:");
		return false;
	}

	if (m_isDebugMode)
		std::cout << "Step 1: WinSock Initalized" << std::endl;
	return true;
}


// Attempt to connect to an address
bool cTCPModuleClient::ConnectToServer(const char* ipaddr, const char* port)
{

	// This will be used for the Step2 and Step3
	addrinfo *addressInfo = 0;
	if (!this->GetAddressInfo(addressInfo, ipaddr, port))
	{
		this->ShutDown();
		return false;
	}

	//Step 2: Create a Socket

	// Attempt to connect to an address until one succeeds
	std::cout << "Step 2 & 3: Create a Socket & Connect to server" << std::endl;
	struct addrinfo *ptr = NULL;
	for (ptr = addressInfo; ptr != NULL; ptr = ptr->ai_next)
	{
		if (!this->CreateSocket(*addressInfo))
		{
			//this->ShutDown();
			m_socket = INVALID_SOCKET;
			break;
		}

		// Connect to server.
		int result = connect(m_socket, addressInfo->ai_addr, (int)addressInfo->ai_addrlen);

		if (result == SOCKET_ERROR)
		{
			//closesocket(connectedSocket);
			m_socket = INVALID_SOCKET;
			break;
		}
	}

	freeaddrinfo(addressInfo); //Once bind is called the address info is no longer needed so free the memory allocated

	if (m_socket == INVALID_SOCKET)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("Unable to connect to server!: ");
		return false;
	}

	std::cout << "Step 2 & 3: Socket created & Server connected" << std::endl;

	m_isConnected = true;

	return true;
}

//Step 5-1: Disconnect
bool cTCPModuleClient::Disconnect(const SOCKET connectedSocket)
{
	if (m_isDebugMode)
		std::cout << "\tDisconnect socket: " << connectedSocket << std::endl;

	closesocket(connectedSocket);

	if (m_isDebugMode)
		std::cout << "\tSocket disconnected" << std::endl;

	m_isConnected = false;
	return true;
}
