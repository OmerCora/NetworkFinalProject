#include <stdio.h>
#include <iostream>
#include "cTCPModuleServer.h"

#include "cErrorReport.h"


cTCPModuleServer::cTCPModuleServer()
{
	//Clear the master
	FD_ZERO(&m_masterFileDiscriptor);
	FD_ZERO(&m_readFileDiscriptors);
}


cTCPModuleServer::~cTCPModuleServer()
{
}

bool cTCPModuleServer::Start(const char* ipaddr, const char* port)
{
	//Step 1: Initial winsock
	if (!this->Initialize())
	{
		this->ShutDown();
		return false;
	}

	// This will be used for the Step2 and Step3
	addrinfo *addressInfo = 0;
	if (!this->GetAddressInfo(addressInfo, ipaddr, port))
	{
		this->ShutDown();
		return false;
	}

	//Step 2: Create a Socket
	if (!this->CreateSocket(*addressInfo))
	{
		freeaddrinfo(addressInfo); //free memory allocated to provent memory leaks
		this->ShutDown();
		return false;
	}

	this->SetNonBlockingMode();

	//Step 3: Bind the Socket
	if (!this->BindSocket(*addressInfo))
	{
		freeaddrinfo(addressInfo); //free memory allocated to provent memory leaks
		this->ShutDown();
		return false;
	}

	freeaddrinfo(addressInfo); //Once bind is called the address info is no longer needed so free the memory allocated


	//Step 4: Listen for a client connection
	if (!this->ListenConnection())
	{
		this->ShutDown();
		return false;
	}


	return true;
}


bool cTCPModuleServer::Run()
{
	//Set up file descriptors for select statement

	//Clear the master
	FD_ZERO(&m_masterFileDiscriptor);
	FD_ZERO(&m_readFileDiscriptors);

	//Add the listener to the master set
	FD_SET(m_socket, &m_masterFileDiscriptor);

	//Keep track of the biggest file descriptor
	m_maximumFileDiscriptor = m_socket; //Maximum file descriptor number (to start it's the listener)
	std::cout << "maximumFileDiscriptor " << m_maximumFileDiscriptor << std::endl;

	//Set timeout time
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 500 * 1000; // 500 ms, 0.5 sec

	while (!m_isStopped)
	{
		Sleep(1);

		//Select function checks to see if any socket has activity
		m_readFileDiscriptors = m_masterFileDiscriptor; // copy the master to a temp
		int result = select(m_maximumFileDiscriptor + 1, &m_readFileDiscriptors, NULL, NULL, &tv);
		if (result == SOCKET_ERROR)
		{
			cErrorReport::GetInstance().PrintWSAGetLastMessage("Select Error: ");
			this->ShutDown();
			return false;
		}

		//Loop through existing connections looking for data to read
		for (int iClient = 0; iClient <= m_maximumFileDiscriptor; iClient++)
		{
			//If no flag is raised keep looping
			if (!FD_ISSET(iClient, &m_readFileDiscriptors))
			{
				continue;
			}

			//If the raised flag is on the listening socket accept a new connection then keep looping
			if (iClient == m_socket)
			{
				//Step 5: Accept a client's connection
				this->AcceptConnection();

				continue;
			}


			//Step 6: Receiveing and sending data
			if (!this->ReceiveData(iClient, m_receiveBuffers[iClient]))
			{
				// TODO: don't close socket. it should wait for next packet
				//if (!this->Disconnect(iClient))
				//{
				//	this->ShutDown();
				//	return false;
				//}
				continue;
			}


			this->ProcessReceivePackets(iClient, m_receiveBuffers[iClient]);
		}

		this->Respond();

		//if (m_receiveBuffer.getWriteIndex() > 0)
		//	break;
	}

	return true;
}

//Step 7-2: Cleanup
bool cTCPModuleServer::ShutDown()
{
	std::cout << "Step 7: Disconnect Server" << std::endl;

	m_isStopped = true;


	// shutdown the send half of the connection since no more data will be sent
	// TODO: when server socket is shutdown, all the times, error occur. client sockets are not tested yet
	int iResult = shutdown(m_socket, SD_SEND);//SD_SEND/SD_RECEIVE/SD_BOTH
													   //if (iResult == SOCKET_ERROR)
													   //{
													   //	cErrorReport::GetInstance().PrintWSAGetLastMessage("shutdown failed: ");
													   //	return false;
													   //}


	m_readFileDiscriptors = m_masterFileDiscriptor;

	for (u_int iClient = 0; iClient < m_readFileDiscriptors.fd_count; ++iClient)
	{
		SOCKET client = m_readFileDiscriptors.fd_array[iClient];

		Disconnect(client);
	}


	if (!Disconnect(m_socket))
	{
		return false;
	}

	//Final clean up
	WSACleanup();

	return true;
}


bool cTCPModuleServer::GetAddressInfo(addrinfo*& outAddressInfo, const char* ipaddr, const char* port)
{
	//Convert hostnames and IP addresses between their string representations and the struct (binary) representation

	addrinfo hintsAddrinfo;
	ZeroMemory(&hintsAddrinfo, sizeof(hintsAddrinfo));
	hintsAddrinfo.ai_family = AF_INET; // internetwork: UDP, TCP, etc.
	hintsAddrinfo.ai_socktype = SOCK_STREAM; //Stream must be specified for TCP
											   // SOCK_STREAM: intact data delivery, higher overhead, use important game protocol
											   // SOCK_DGRAM: not timely delivery, responds many many requests at once very quickly, use less important game protocol
	hintsAddrinfo.ai_protocol = IPPROTO_TCP; //Protocol is TCP
	hintsAddrinfo.ai_flags = AI_PASSIVE;// Socket address will be used in bind() call

	int resultInt = getaddrinfo(NULL/*ipaddr*/, port, &hintsAddrinfo, &outAddressInfo);
	if (resultInt != 0)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("Socket Initalization failed:");
		return false;
	}

	std::cout << "Step 1: WinSock Initalized" << std::endl;
	return true;
}

//Step 3: Bind the Socket
bool cTCPModuleServer::BindSocket(const addrinfo& addressInfo)
{
	// Assigns a socket an address and prepares it for accepting connections

	std::cout << "Step 3: Bind Socket" << std::endl;

	int resultInt = bind(m_socket, addressInfo.ai_addr, (int)addressInfo.ai_addrlen);

	if (resultInt != 0)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("Socket binding failed:");
		return 1;
	}

	std::cout << "Step 3: Socket Bound" << std::endl;
	return true;
}

//Step 4: Listen for a client connection
bool cTCPModuleServer::ListenConnection()
{
	std::cout << "Step 4: Listen for clients" << std::endl;

	int resultInt = listen(m_socket, SOMAXCONN);
	if (resultInt == SOCKET_ERROR)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("Socket listening failed:");
		return false;
	}

	std::cout << "Step 4: Listening on Socket" << std::endl;
	return true;
}

//Step 5: Accept a client's connection
bool cTCPModuleServer::AcceptConnection()
{
	std::cout << "Step 5: Accept client's connection" << std::endl;

	// handle new connections
	//sockaddr_in client;
	socklen_t addrlen = sizeof sockaddr_storage; //this represents the client address size

	sockaddr_in clientSockAddr;
	//Newly accepted socket descriptor
	int newFileDiscriptor = accept(m_socket, (struct sockaddr *)&clientSockAddr, &addrlen);
	std::cout << "\tConnected to: ";

	printf("%d.%d.%d.%d port: %5u\n",
		(uint8_t)(clientSockAddr.sin_addr.s_addr >> 0),
		(uint8_t)(clientSockAddr.sin_addr.s_addr >> 8),
		(uint8_t)(clientSockAddr.sin_addr.s_addr >> 16),
		(uint8_t)(clientSockAddr.sin_addr.s_addr >> 24), clientSockAddr.sin_port);

	std::cout << "addrlen: " << addrlen;
	std::cout << std::endl;

	std::cout << "\tnewFileDiscriptor: " << newFileDiscriptor << std::endl;
	if (newFileDiscriptor == SOCKET_ERROR)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("\tAccept Error: ");
		return false;
	}

	FD_SET(newFileDiscriptor, &m_masterFileDiscriptor); // add to master set

														//Keep track of max fd
	if (newFileDiscriptor > m_maximumFileDiscriptor)
		m_maximumFileDiscriptor = newFileDiscriptor;

	m_receiveBuffers[newFileDiscriptor] = cBuffer();

	std::cout << "\tNew connection on socket: " << newFileDiscriptor << std::endl;
	return true;
}
//Step 7-1: Disconnect
bool cTCPModuleServer::Disconnect(const SOCKET socket)
{
	std::cout << "\tDisconnect socket: " << socket << std::endl;

	closesocket(socket);
	for (u_int iClient = 0; iClient < m_masterFileDiscriptor.fd_count; ++iClient)
	{
		SOCKET clientPeer = m_masterFileDiscriptor.fd_array[iClient];
		std::cout << "\t\t" << clientPeer << std::endl;
	}
	FD_CLR(socket, &m_masterFileDiscriptor); // remove from master set

	std::map<SOCKET, cBuffer>::iterator iter = m_receiveBuffers.find(socket);
	if(iter!= m_receiveBuffers.end())
		m_receiveBuffers.erase(iter);

	std::cout << "\tSocket disconnected" << std::endl;
	for (u_int iClient = 0; iClient < m_masterFileDiscriptor.fd_count; ++iClient)
	{
		SOCKET clientPeer = m_masterFileDiscriptor.fd_array[iClient];
		std::cout << "\t\t" << clientPeer << std::endl;
	}

	return true;
}

