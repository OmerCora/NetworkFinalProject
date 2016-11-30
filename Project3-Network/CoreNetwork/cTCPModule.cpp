#include <stdio.h>
#include <iostream>
#include <limits>
#include <conio.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include "cTCPModule.h"
#include "cErrorReport.h"


cTCPModule::cTCPModule()
	: m_socket(INVALID_SOCKET)
	, m_isConnected(false)
	, m_isStopped(false)
	, m_isDebugMode(true)
{
}


cTCPModule::~cTCPModule()
{
}




//Step 1: Initial winsock
bool cTCPModule::Initialize()
{
	if (m_isDebugMode)
		std::cout << "Step 1: Initalize WinSock" << std::endl;

	WSADATA m_windowsSocketApplicationData; //Windows Socket Application Data Object

	//Create a Windows Socket Application Data Object
	int resultInt = WSAStartup(MAKEWORD(2, 2), &m_windowsSocketApplicationData);
	if (resultInt != 0)
	{
		if (m_isDebugMode)
			cErrorReport::GetInstance().PrintWSAGetLastMessage("WinSock Initalization failed:");
		return false;
	}

	return true;
}

//Step 2: Create a Socket
bool cTCPModule::CreateSocket(const addrinfo& addressInfo)
{
	//initialize a new socket for a specific protocol

	if (m_isDebugMode)
	{
		std::cout << "\tsocket(ai_family=" << addressInfo.ai_family << "(PF_XXX)";
		std::cout << ", ai_socktype=" << addressInfo.ai_socktype << "(SOCK_XXX)";
		std::cout << ", ai_protocol=" << addressInfo.ai_protocol << "(IPPROTO_XXX)";
		std::cout << std::endl;
	}

	m_socket = socket(addressInfo.ai_family, addressInfo.ai_socktype, addressInfo.ai_protocol);

	if (m_socket == INVALID_SOCKET)
	{
		if (m_isDebugMode)
			cErrorReport::GetInstance().PrintWSAGetLastMessage("Socket Initalization failed:");
		return false;
	}

	return true;
}
bool cTCPModule::SetNonBlockingMode()
{
	//switch the socket to be non blocking
	// iMode=0: blocking mode, iMode!=0: non-blocking mode
	u_long iMode = 1;
	ioctlsocket(m_socket, FIONBIO, &iMode);

	return true;
}


//Step 4-1: Receiveing data
bool cTCPModule::ReceiveData(const SOCKET from, cBuffer& buffer)
{
	if(m_isDebugMode)
		std::cout << "\tReceive data" << std::endl;


	char recvbuf[cBuffer::mc_DEFAULT_BUFFER_LENGTH];

	//Receive the message
	int result = recv(from, recvbuf, sizeof recvbuf, 0);
	if (result == SOCKET_ERROR)
	{
		if (m_isDebugMode)
			cErrorReport::GetInstance().PrintWSAGetLastMessage("\trecv failed: ");
		return false;
	}
	else if (result == 0)
	{
		//cErrorReport::GetInstance().PrintWSAGetLastMessage("\tConnection closing...");
		return false;
	}

	buffer.loadBuffer(recvbuf, result);


	if (m_isDebugMode)
		std::cout << "\tBytes received: " << result << " buffer size: " << buffer.getLength() << std::endl;

	return true;
}


//Step 4-2: sending data
bool cTCPModule::SendData(const SOCKET to)
{
	if (m_isDebugMode)
		std::cout << "bool cTCPModule::SendData(const SOCKET connectedSocket, cBuffer& buffer)" << std::endl;

	int result = send(to, m_sendBuffer.toCharArray(), m_sendBuffer.getLength(), 0);
	if (result == SOCKET_ERROR)
	{
		if (m_isDebugMode)
			cErrorReport::GetInstance().PrintWSAGetLastMessage("\tSend failed: ");
		return false;
	}

	if (m_isDebugMode)
		std::cout << "\t Socket: " << to << " Bytes sent: " << result << std::endl;

	// complete to send data
	m_sendBuffer.clear();

	return true;
}


