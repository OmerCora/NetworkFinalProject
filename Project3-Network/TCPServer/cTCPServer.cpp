

#include <stdio.h>
#include <iostream>
#include <set>

// for windows socket link
#pragma comment(lib, "Ws2_32.lib")

// need to link corenetwork.lib
#pragma comment (lib, "CoreNetwork.lib")


#include "cTCPServer.h"
#include "cErrorReport.h"
#include "cRoomManager.h"
#include "cUserManager.h"
#include "cTCPAuthenticationClient.h"

const char* cTCPServer::mc_DEFAULT_PORT = "27015";

cTCPServer::cTCPServer()
	: m_isStopped(false)
	, m_listeningSocket(INVALID_SOCKET)
	, m_lockAuthenticationTaskBuffer(false)
{
}


cTCPServer::~cTCPServer()
{
}

cTCPServer* cTCPServer::m_instance = 0;
cTCPServer& cTCPServer::GetInstance()
{
	if (m_instance == 0)
		m_instance = new cTCPServer();
	return *m_instance;
}
void cTCPServer::DeleteInstance()
{
	cRoomManager::DeleteInstance();
	cUserManager::DeleteInstance();
	
	if (m_instance)
		delete m_instance;
	m_instance = 0;
}
bool cTCPServer::StartServer()
{
	//if (SetConsoleCtrlHandler(NULL, FALSE) == FALSE)
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE)
	{
		// unable to install handler... 
		// display message to the user
		printf("Unable to install handler!\n");
		return false;
	}

	//Step 1: Initial winsock
	if (!this->Initialize())
	{
		this->ShutDown();
		return false;
	}

	// This will be used for the Step2 and Step3
	addrinfo *addressInfo = 0;
	if (!this->GetAddressInfo(addressInfo))
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
#if 0
#include <signal.h>
#ifdef _WIN32
int const sigClosed = SIGBREAK;
#else
int const sigClosed = SIGHUP;
#endif

volatile sig_atomic_t windowClosed = 0;
void cTCPServer::SignalHandler()
{
	cTCPServer::GetInstance().m_isStopped = true;
	//cTCPServer::GetInstance().ShutDown();
}
#endif

bool cTCPServer::RunServer()
{

	//Set up file descriptors for select statement

	//Clear the master
	FD_ZERO(&m_masterFileDiscriptor);
	FD_ZERO(&m_readFileDiscriptors);

	//Add the listener to the master set
	FD_SET(m_listeningSocket, &m_masterFileDiscriptor);

	//Keep track of the biggest file descriptor
	m_maximumFileDiscriptor = m_listeningSocket; //Maximum file descriptor number (to start it's the listener)
	std::cout << "maximumFileDiscriptor " << m_maximumFileDiscriptor << std::endl;

	//Set timeout time
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 500 * 1000; // 500 ms, 0.5 sec

	//signal(sigClosed, SignalHandler);
	//GetMessage(
	while (!m_isStopped)
	{
		Sleep(1);

		//Select function checks to see if any socket has activity
		m_readFileDiscriptors = m_masterFileDiscriptor; // copy the master to a temp
		int result = select(m_maximumFileDiscriptor + 1, &m_readFileDiscriptors, NULL, NULL, &tv);
		if(result == SOCKET_ERROR)
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
			if (iClient == m_listeningSocket)
			{
				//Step 5: Accept a client's connection
				this->AcceptConnection();


				continue;
			}


			//Step 6: Receiveing and sending data
			if (!this->ReceiveData(iClient))
			{
				// TODO: don't close socket. it should wait for next packet
				//if (!this->Disconnect(iClient))
				//{
				//	this->ShutDown();
				//	return false;
				//}
			}
		}

		// process authentication result
		{
			this->ProcessAuthenticationTasks();
		}
		
		// message framing and perform tasks
		{
			this->ProcessReceivedDataFromUsers();
		}


		//if (m_receiveBuffer.getWriteIndex() > 0)
		//	break;
	}

	return true;
}


//Step 7-2: Cleanup
bool cTCPServer::ShutDown()
{
	std::cout << "Step 7: Disconnect Server" << std::endl;

	m_isStopped = true;


	// shutdown the send half of the connection since no more data will be sent
	// TODO: when server socket is shutdown, all the times, error occur. client sockets are not tested yet
	int iResult = shutdown(m_listeningSocket, SD_SEND);//SD_SEND/SD_RECEIVE/SD_BOTH
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


	if (!Disconnect(m_listeningSocket))
	{
		return false;
	}

	//Final clean up
	WSACleanup();

	return true;
}

//Step 1: Initial winsock
bool cTCPServer::Initialize()
{
	std::cout << "Step 1: Initalize WinSock" << std::endl;

	//WSADATA m_windowsSocketApplicationData; //Windows Socket Application Data Object

	//Create a Windows Socket Application Data Object
	int resultInt = WSAStartup(MAKEWORD(2, 2), &m_windowsSocketApplicationData);
	if (resultInt != 0)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("WinSock Initalization failed:" );
		return false;
	}

	return true;
}

bool cTCPServer::GetAddressInfo(addrinfo*& outAddressInfo)
{
	//Convert hostnames and IP addresses between their string representations and the struct (binary) representation


	//struct addrinfo hints;

	addrinfo m_hintsAddrinfo;
	ZeroMemory(&m_hintsAddrinfo, sizeof(m_hintsAddrinfo));
	m_hintsAddrinfo.ai_family = AF_INET; // internetwork: UDP, TCP, etc.
	m_hintsAddrinfo.ai_socktype = SOCK_STREAM; //Stream must be specified for TCP
	// SOCK_STREAM: intact data delivery, higher overhead, use important game protocol
	// SOCK_DGRAM: not timely delivery, responds many many requests at once very quickly, use less important game protocol
	m_hintsAddrinfo.ai_protocol = IPPROTO_TCP; //Protocol is TCP
	m_hintsAddrinfo.ai_flags = AI_PASSIVE;// Socket address will be used in bind() call

	int resultInt = getaddrinfo(NULL, mc_DEFAULT_PORT, &m_hintsAddrinfo, &outAddressInfo);
	if (resultInt != 0)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("Socket Initalization failed:");
		return false;
	}

	std::cout << "Step 1: WinSock Initalized" << std::endl;
	return true;
}

//Step 2: Create a Socket
bool cTCPServer::CreateSocket(const addrinfo& addressInfo)
{
	//initialize a new socket for a specific protocol

	std::cout << "Step 2: Create a Socket" << std::endl;

	std::cout << "socket(ai_family=" << addressInfo.ai_family << "(PF_XXX)";
	std::cout << ", ai_socktype=" << addressInfo.ai_socktype << "(SOCK_XXX)";
	std::cout << ", ai_protocol=" << addressInfo.ai_protocol << "(IPPROTO_XXX)";
	std::cout << std::endl;

	m_listeningSocket = socket(addressInfo.ai_family, addressInfo.ai_socktype, addressInfo.ai_protocol);

	if (m_listeningSocket == INVALID_SOCKET)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("Socket Initalization failed:");
		return false;
	}

	//switch the socket to be non blocking
	// iMode=0: blocking mode, iMode!=0: non-blocking mode
	u_long iMode = 1;
	ioctlsocket(m_listeningSocket, FIONBIO, &iMode);


	std::cout << "Step 2: Socket Created" << std::endl;
	return true;
}
//Step 3: Bind the Socket
bool cTCPServer::BindSocket(const addrinfo& addressInfo)
{
	// Assigns a socket an address and prepares it for accepting connections

	std::cout << "Step 3: Bind Socket" << std::endl;

	int resultInt = bind(m_listeningSocket, addressInfo.ai_addr, (int)addressInfo.ai_addrlen);

	if (resultInt!=0)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("Socket binding failed:");
		return 1;
	}

	std::cout << "Step 3: Socket Bound" << std::endl;
	return true;
}
//Step 4: Listen for a client connection
bool cTCPServer::ListenConnection()
{
	std::cout << "Step 4: Listen for clients" << std::endl;

	int resultInt = listen(m_listeningSocket, SOMAXCONN);
	if(resultInt== SOCKET_ERROR)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("Socket listening failed:");
		return false;
	}

	std::cout << "Step 4: Listening on Socket" << std::endl;
	return true;
}

//Step 5: Accept a client's connection
bool cTCPServer::AcceptConnection()
{
	std::cout << "Step 5: Accept client's connection" << std::endl;

	// handle new connections
	//sockaddr_in client;
	socklen_t addrlen = sizeof sockaddr_storage; //this represents the client address size

	//Newly accepted socket descriptor
	int newFileDiscriptor = accept(m_listeningSocket, (struct sockaddr *)&m_clientSockAddr, &addrlen);
	std::cout << "\tConnected to: ";

	printf("%d.%d.%d.%d port: %5u\n",
		(uint8_t)(m_clientSockAddr.sin_addr.s_addr >> 0),
		(uint8_t)(m_clientSockAddr.sin_addr.s_addr >> 8),
		(uint8_t)(m_clientSockAddr.sin_addr.s_addr >> 16),
		(uint8_t)(m_clientSockAddr.sin_addr.s_addr >> 24),
		m_clientSockAddr.sin_port);

		//<< std::hex << (uint8_t)(client.sin_addr.s_addr >> 24) << "."
		//<< std::hex << (uint8_t)(client.sin_addr.s_addr >> 16) << "."
		//<< std::hex << (uint8_t)(client.sin_addr.s_addr >> 8) << "."
		//<< std::hex << (uint8_t)(client.sin_addr.s_addr) << std::dec
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

	// create new user
	{
		//sUserInfo userInfo;
		m_userInfoBuff.email = "guest";
		m_userInfoBuff.socket = newFileDiscriptor;
		m_userInfoBuff.connectTime = GetCurrentTime();
		iUser* user = cUserManager::GetInstance().CreateNewUser(m_userInfoBuff);
		if (user == 0)
		{
			user = cUserManager::GetInstance().GetUser(newFileDiscriptor);
			std::cout << "\tWARNING: already connected socket: " << user->SocketID() << " connectionTime: " << user->ConnectionTime() << std::endl;
			this->Disconnect(newFileDiscriptor);
			return false;
		}
	}


	std::cout << "\tNew connection on socket: " << newFileDiscriptor << std::endl;
	return true;
}


//Step 6: Receiveing and sending data
bool cTCPServer::ReceiveData(const SOCKET clientSocket)
{
	//std::cout << "\tReceive data" << std::endl;


	//Receive the message
	int result = recv(clientSocket, m_recvbuf, sizeof m_recvbuf, 0);
	if(result == SOCKET_ERROR)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("\trecv failed: ");
		//this->Disconnect(clientSocket);
		Sleep(1);
		return false;
	}
	else if (result == 0)
	{
		//cErrorReport::GetInstance().PrintWSAGetLastMessage("\tConnection closing...");
		//this->Disconnect(clientSocket);
		return false;
	}

	std::cout << "\tBytes received: " << result << std::endl;

	iUser* user = cUserManager::GetInstance().GetUser(clientSocket);
	if (user)
	{
		user->GetReceiveBuffer().loadBuffer(m_recvbuf, result);
		m_receiveFromUsers.insert(clientSocket);

		std::cout << "\t\tbuffer size: " << user->GetReceiveBuffer().getLength() << std::endl;
	}
	else
	{
		return false;
	}
	//std::cout << "\tnumber of chars: " << m_receiveBuffer.readUInt16BE() << std::endl;
	//std::cout << "\tdata: " << m_receiveBuffer.toCharArray() << std::endl;


	return true;
}
bool cTCPServer::SendSuccess(SOCKET client, sProtocolHeader::ePacketID packetID)
{
	m_headerSendBuff.packet_id = packetID;
	m_headerSendBuff.packet_length = sizeof(sProtocolHeader);

	m_sendBuffer.clear();
	m_sendBuffer.Serialize(m_headerSendBuff);

	std::cout << "\tSUCCESS serialize length: " << m_sendBuffer.getLength() << " original size: " << m_headerSendBuff.packet_length << std::endl;

	if (!this->SendData(client))
	{
		this->Disconnect(client);
		return false;
	}
	return true;
}

bool cTCPServer::SendFailure(SOCKET client, sProtocolHeader::ePacketID packetID, sProtocolResponseFailure::eFailureCode failureCode, const std::string& errormsg)
{
	m_failureSendBuff.errormessage = errormsg;
	m_failureSendBuff.message_length = (short)m_failureSendBuff.errormessage.size();
	m_failureSendBuff.failureCode = failureCode;

	m_headerSendBuff.SetProtocol(m_failureSendBuff);
	m_headerSendBuff.packet_id = packetID;

	m_sendBuffer.clear();
	m_sendBuffer.CheckBufferSize(m_headerSendBuff.packet_length);
	m_sendBuffer.Serialize(m_headerSendBuff);
	m_sendBuffer.Serialize(m_failureSendBuff);

	std::cout << "\tFAILURE serialize length: " << m_sendBuffer.getLength() << " original size: " << m_headerSendBuff.packet_length << std::endl;

	if (!this->SendData(client))
	{
		this->Disconnect(client);
		return false;
	}
	return true;
}

void cTCPServer::AddAuthenticationTask(sProtocolHeader::ePacketID id, iUser* user)
{
	while (m_lockAuthenticationTaskBuffer)
	{
		Sleep(1);
	}
	m_lockAuthenticationTaskBuffer = true;
	m_authenticationTasks.push(std::pair<sProtocolHeader::ePacketID, iUser*>(id, user));
	m_lockAuthenticationTaskBuffer = false;
}

bool cTCPServer::ProcessAuthenticationTasks()
{
	while (m_lockAuthenticationTaskBuffer)
	{
		Sleep(1);
	}
	m_lockAuthenticationTaskBuffer = true;


	while (!m_authenticationTasks.empty())
	{
		std::pair<sProtocolHeader::ePacketID, iUser*>& data = m_authenticationTasks.front();
		iUser* user = data.second;
		if (data.first == sProtocolHeader::e_ResponseSigninSuccess)
		{
			std::cout << "ProcessAuthenticationTasks::e_ResponseSigninSuccess" << std::endl;

			iLobby* lobby = cRoomManager::GetInstance().GetLobby();
			if (!lobby->JoinLobby(user))
			{
				this->SendFailure(user->SocketID(), sProtocolHeader::ePacketID::e_ResponseSigninFailure, sProtocolResponseFailure::e_JustMessage, "Failed sign-in");
				user->Logout();
			}
			else
			{
				this->SendSuccess(user->SocketID(), data.first);

				// response
				m_responsesPacketIDs[sProtocolHeader::ePacketID::e_ResponseRoomList].insert(user->SocketID());

				// broadcast 
				cUserManager::GetInstance().CollectLoggedInUsers(m_responsesPacketIDs[sProtocolHeader::ePacketID::e_ResponseLobbyUsers]);
			}
		}
		else if (data.first == sProtocolHeader::e_ResponseLoginSuccess)
		{
			std::cout << "ProcessAuthenticationTasks::e_ResponseLoginSuccess" << std::endl;

			iLobby* lobby = cRoomManager::GetInstance().GetLobby();
			if (!lobby->JoinLobby(user))
			{
				this->SendFailure(user->SocketID(), sProtocolHeader::ePacketID::e_ResponseLoginFailure, sProtocolResponseFailure::e_JustMessage, "Failed log-in");
				user->Logout();
			}
			else
			{
				// response requesting user
				this->SendSuccess(user->SocketID(), sProtocolHeader::ePacketID::e_ResponseLoginSuccess);

				// response
				m_responsesPacketIDs[sProtocolHeader::ePacketID::e_ResponseRoomList].insert(user->SocketID());

				// broadcast 
				cUserManager::GetInstance().CollectLoggedInUsers(m_responsesPacketIDs[sProtocolHeader::ePacketID::e_ResponseLobbyUsers]);
			}
		}

		m_authenticationTasks.pop();
	}
	m_lockAuthenticationTaskBuffer = false;
	return true;
}
bool cTCPServer::ProcessReceivedDataFromUsers()
{

	std::set<SOCKET>::iterator iter = m_receiveFromUsers.begin();
	for (; iter != m_receiveFromUsers.end(); ++iter)
	{
		iUser* user = cUserManager::GetInstance().GetUser(*iter);
		if (user == 0)
			continue;

		cBuffer& receiveBuffer = user->GetReceiveBuffer();

		if (receiveBuffer.isEmpty())
			continue;

		// check message framing
		{
			// check the complition of packet_length value
			if (sizeof(m_headerRecvBuff.packet_length) > receiveBuffer.getLength())
				continue;

			// check the total packet length
			m_headerRecvBuff.packet_length = receiveBuffer.readInt32BE(0);
			if (m_headerRecvBuff.packet_length > (int)(receiveBuffer.getLength() + sizeof(m_headerRecvBuff.packet_length)))
				continue;
		}

		
		receiveBuffer.Deserialize(m_headerRecvBuff);

		switch (m_headerRecvBuff.packet_id)
		{
		case sProtocolHeader::ePacketID::e_RequestSignin:
		{
			std::cout << "e_RequestSignin" << std::endl;

			receiveBuffer.Deserialize(m_loginInfoRecvBuff);

			// check already logged-in
			if (cUserManager::GetInstance().UserExist(m_loginInfoRecvBuff.username))
			{
				this->SendFailure(user->SocketID(), sProtocolHeader::ePacketID::e_ResponseSigninFailure, sProtocolResponseFailure::e_JustMessage, "Failed sign-in");
				break;
			}

			// request authentication to add new account
			cTCPAuthenticationClient::GetInstance().StartSigninThread(user, m_loginInfoRecvBuff);

			break;
		}
		case sProtocolHeader::ePacketID::e_RequestLogin:
		{
			std::cout << "e_RequestLogin" << std::endl;

			receiveBuffer.Deserialize(m_loginInfoRecvBuff);

			// check already logged-in
			if (cUserManager::GetInstance().UserExist(m_loginInfoRecvBuff.username))
			{
				this->SendFailure(user->SocketID(), sProtocolHeader::ePacketID::e_ResponseLoginFailure, sProtocolResponseFailure::e_JustMessage, "Failed log-in");
				break;
			}

			// request authentication to authentication server
			cTCPAuthenticationClient::GetInstance().StartLoginThread(user, m_loginInfoRecvBuff);

			break;
		}
		case sProtocolHeader::ePacketID::e_RequestLogout:
		{
			std::cout << "e_RequestLogout" << std::endl;

			user->Logout();
			cRoomManager::GetInstance().GetLobby()->LeaveLobby(user);

			this->SendSuccess(user->SocketID(), sProtocolHeader::ePacketID::e_ResponseLogoutSuccess);

			cUserManager::GetInstance().CollectLoggedInUsers(m_responsesPacketIDs[sProtocolHeader::ePacketID::e_ResponseLobbyUsers]);

			break;
		}
		case sProtocolHeader::ePacketID::e_RequestRoomList:
		{
			std::cout << "e_RequestRoomList" << std::endl;

			m_responsesPacketIDs[sProtocolHeader::ePacketID::e_ResponseRoomList].insert(user->SocketID());
			break;
		}
		case sProtocolHeader::ePacketID::e_RequestNewRoom:
		{
			std::cout << "e_RequestNewRoom" << std::endl;
			// create new room
			receiveBuffer.Deserialize(m_newRoomRecvBuff);

			iRoom* room = cRoomManager::GetInstance().CreateRoom(m_newRoomRecvBuff.roomname);
			if (room == 0)
			{
				this->SendFailure(user->SocketID(), sProtocolHeader::e_ResponseNewRoomFailure, sProtocolResponseFailure::e_JustMessage, "Failed creating a room: " + m_newRoomRecvBuff.roomname);
				break;
			}

			if (!room->Join(user, m_newRoomRecvBuff.username))
			{
				this->SendFailure(user->SocketID(), sProtocolHeader::e_ResponseNewRoomFailure, sProtocolResponseFailure::e_JustMessage, "Failed joining a new room: " + m_newRoomRecvBuff.roomname);
				break;
			}

			// leave lobby
			iLobby* lobby = cRoomManager::GetInstance().GetLobby();
			lobby->LeaveLobby(user);

			// response
			this->SendSuccess(user->SocketID(), sProtocolHeader::ePacketID::e_ResponseNewRoomSuccess);

			// broadcast room list
			lobby->CollectLobbyUsers(m_responsesPacketIDs[sProtocolHeader::ePacketID::e_ResponseRoomList]);

			// broadcast leave user
			cUserManager::GetInstance().CollectLoggedInUsers(m_responsesPacketIDs[sProtocolHeader::ePacketID::e_ResponseLobbyUsers]);
			break;
		}
		case sProtocolHeader::ePacketID::e_RequestJoinRoom:
		{
			std::cout << "e_RequestJoinRoom" << std::endl;

			receiveBuffer.Deserialize(m_joinRoomRecvBuff);

			iRoom* room = cRoomManager::GetInstance().GetRoom(m_joinRoomRecvBuff.roomname);
			if (room == 0)
			{
				this->SendFailure(user->SocketID(), sProtocolHeader::ePacketID::e_ResponseJoinRoomFailure, sProtocolResponseFailure::e_JustMessage, "Failed joining a room: " + m_joinRoomRecvBuff.roomname);
				break;
			}

			if (!room->Join(user, m_joinRoomRecvBuff.username))
			{
				this->SendFailure(user->SocketID(), sProtocolHeader::ePacketID::e_ResponseJoinRoomFailure, sProtocolResponseFailure::e_JustMessage, "Failed joining a room: " + m_joinRoomRecvBuff.roomname);
				break;
			}

			// leave lobby
			iLobby* lobby = cRoomManager::GetInstance().GetLobby();
			lobby->LeaveLobby(user);

			// response
			this->SendSuccess(user->SocketID(), sProtocolHeader::ePacketID::e_ResponseJoinRoomSuccess);

			// broadcast room list for users change
			lobby->CollectLobbyUsers(m_responsesPacketIDs[sProtocolHeader::ePacketID::e_ResponseRoomList]);

			// broadcast leave user
			cUserManager::GetInstance().CollectLoggedInUsers(m_responsesPacketIDs[sProtocolHeader::ePacketID::e_ResponseLobbyUsers]);

			break;
		}
		case sProtocolHeader::ePacketID::e_SendChatMessage:
		{
			std::cout << "e_SendChatMessage" << std::endl;
			// receive message from a client

			receiveBuffer.Deserialize(m_sendMsgRecvBuff);

			iRoom* room = cRoomManager::GetInstance().GetRoom(m_sendMsgRecvBuff.roomname);
			if (room == 0)
				room = cRoomManager::GetInstance().GetRoomByUser(user);

			if (room == 0)
			{
				this->SendFailure(user->SocketID(), sProtocolHeader::ePacketID::e_SendChatMessageFailure, sProtocolResponseFailure::e_JustMessage, "Failed joining a room: " + m_sendMsgRecvBuff.roomname);
				break;
			}

			// add chat
			room->AddChat(m_sendMsgRecvBuff.username, m_sendMsgRecvBuff.message);

			break;
		}
		case sProtocolHeader::ePacketID::e_RequestLeaveRoom:
		{
			std::cout << "e_RequestLeaveRoom" << std::endl;

			receiveBuffer.Deserialize(m_reaveRoomRecvBuff);

			iRoom* room = cRoomManager::GetInstance().GetRoom(m_reaveRoomRecvBuff.roomname);
			if (room == 0)
			{
				room = cRoomManager::GetInstance().GetRoomByUser(user);
				if(room != 0)
					room->Leave(user);

				this->SendFailure(user->SocketID(), sProtocolHeader::ePacketID::e_ResponseLeaveRoomFailure, sProtocolResponseFailure::e_JustMessage, "Failed leaving a room: " + m_reaveRoomRecvBuff.roomname);
			}
			else
			{
				room->Leave(user);

				this->SendSuccess(user->SocketID(), sProtocolHeader::ePacketID::e_ResponseLeaveRoomSuccess);
			}

			// join lobby
			iLobby* lobby = cRoomManager::GetInstance().GetLobby();
			lobby->JoinLobby(user);

			// response
			m_responsesPacketIDs[sProtocolHeader::ePacketID::e_ResponseRoomList].insert(user->SocketID());

			// broadcast room list for users change
			lobby->CollectLobbyUsers(m_responsesPacketIDs[sProtocolHeader::ePacketID::e_ResponseRoomList]);

			// broadcast leave user
			cUserManager::GetInstance().CollectLoggedInUsers(m_responsesPacketIDs[sProtocolHeader::ePacketID::e_ResponseLobbyUsers]);
			break;
		}
		case sProtocolHeader::ePacketID::e_RequestDisconnect:
		{
			std::cout << "e_RequestDisconnect" << std::endl;

			// leave room or lobby if it's in
			iRoom* room = cRoomManager::GetInstance().GetRoomByUser(user);
			if (room)
			{
				room->Leave(user);

				// broadcast room list for users change
				iLobby* lobby = cRoomManager::GetInstance().GetLobby();
				lobby->CollectLobbyUsers(m_responsesPacketIDs[sProtocolHeader::ePacketID::e_ResponseRoomList]);
			}

			this->SendSuccess(user->SocketID(), sProtocolHeader::e_ResponseDisconnect);

			this->Disconnect(user->SocketID());

			// broadcast leave user
			cUserManager::GetInstance().CollectLoggedInUsers(m_responsesPacketIDs[sProtocolHeader::ePacketID::e_ResponseLobbyUsers]);

			break;
		}
		default:
			break;
		}


		receiveBuffer.clear();
	}

	cRoomManager::GetInstance().BroadcastData();

	return this->RespondData(m_responsesPacketIDs);
}


// Message framing and determin what task performed by packet header
bool cTCPServer::RespondData(std::map<sProtocolHeader::ePacketID, std::set<SOCKET>>& responsesPacketIDs)
{
	if (responsesPacketIDs.size() == 0)
		return true;
	
	std::map<sProtocolHeader::ePacketID, std::set<SOCKET>>::iterator iter = responsesPacketIDs.begin();
	for (; iter != responsesPacketIDs.end(); ++iter)
	{
		if (iter->first == sProtocolHeader::ePacketID::e_ResponseRoomList)
		{
			cRoomManager::GetInstance().SerializeRoomList(sProtocolHeader::ePacketID::e_ResponseRoomList, m_sendBuffer);
			this->BroadcastData(iter->second);
		}
		else if (iter->first == sProtocolHeader::ePacketID::e_ResponseLobbyUsers)
		{
			iLobby* lobby = cRoomManager::GetInstance().GetLobby();
			lobby->GetLobbyUsers(m_lobbyUsersSendBuff);

			m_headerSendBuff.SetProtocol(m_lobbyUsersSendBuff);
			// override
			m_headerSendBuff.packet_id = sProtocolHeader::ePacketID::e_ResponseLobbyUsers;

			m_sendBuffer.clear();
			m_sendBuffer.CheckBufferSize(m_headerSendBuff.packet_length);
			m_sendBuffer.Serialize(m_headerSendBuff);
			m_sendBuffer.Serialize(m_lobbyUsersSendBuff);

			std::cout << "\tserialize length: " << m_sendBuffer.getLength() << " original size: " << m_headerSendBuff.packet_length << std::endl;

			this->BroadcastData(iter->second);
		}
	}

	m_responsesPacketIDs.clear();

	return true;
}
bool cTCPServer::BroadcastData(std::set<SOCKET>& clients)
{
	std::set<SOCKET>::iterator socketiter = clients.begin();
	for (; socketiter != clients.end(); ++socketiter)
	{
		SOCKET client = *socketiter;

		// may the user disconnected by whatever
		iUser* user = cUserManager::GetInstance().GetUser(client);
		if (user == 0)
			continue;

		if (!this->SendData(client))
		{
			this->Disconnect(client);
		}

		Sleep(1);
	}
	return true;
}


bool cTCPServer::SendData(const SOCKET client)
{
	const char* data = m_sendBuffer.toCharArray();
	int length = (int)m_sendBuffer.getLength();
	int result = send(client, data, length, 0);
	if (result == SOCKET_ERROR)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("\tSend failed: ");
		return false;
	}

	std::cout << "\t Socket: " << client << " Bytes sent: " << result << std::endl;

	return true;
}

//Step 7-1: Disconnect
bool cTCPServer::Disconnect(const SOCKET socket)
{
	std::cout << "\tDisconnect socket: " << socket << std::endl;

	closesocket(socket);
	for (u_int iClient = 0; iClient < m_masterFileDiscriptor.fd_count; ++iClient)
	{
		SOCKET clientPeer = m_masterFileDiscriptor.fd_array[iClient];
		std::cout << "\t\t" << clientPeer << std::endl;
	}
	FD_CLR(socket, &m_masterFileDiscriptor); // remove from master set

	//m_maximumFileDiscriptor = m_masterFileDiscriptor.fd_array[m_masterFileDiscriptor.fd_count - 1];

	std::cout << "\tSocket disconnected" << std::endl;
	for (u_int iClient = 0; iClient < m_masterFileDiscriptor.fd_count; ++iClient)
	{
		SOCKET clientPeer = m_masterFileDiscriptor.fd_array[iClient];
		std::cout << "\t\t" << clientPeer << std::endl;
	}

	// delete user
	cUserManager::GetInstance().DeleteUser(socket);

	Sleep(1);
	return true;
}

// when console close event occur, it make sure the clean up every thing before console shutdown
BOOL WINAPI cTCPServer::ConsoleHandler(DWORD CEvent)
{

	switch (CEvent)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
		system("pause");
		return TRUE;

	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:

		cTCPServer::GetInstance().m_isStopped = true;
		while (cTCPServer::m_instance != 0)
		{
			Sleep(1);
		}
		// Detach Console
		FreeConsole();
		// Prevent closing
		ExitThread(0);
		return TRUE;
	default:
		break;
	}

	return FALSE;
}
