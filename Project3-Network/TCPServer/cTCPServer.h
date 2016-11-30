#ifndef _cTCPServer_HG_
#define _cTCPServer_HG_


#include <map>
#include <set>
#include <queue>

#undef UNICODE
// exclude many header files of windows.h
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "cBuffer.h"
#include "sProtocols.h"
#include "iUser.h"


class cTCPServer
{
	cTCPServer();
	~cTCPServer();
	static cTCPServer* m_instance;
public:
	static cTCPServer& GetInstance();
	static void DeleteInstance();

	// Initialize server
	bool StartServer();
	// message loop
	bool RunServer();
	//Cleanup
	bool ShutDown();

	void AddAuthenticationTask(sProtocolHeader::ePacketID id, iUser* user);
	bool SendSuccess(SOCKET client, sProtocolHeader::ePacketID packetID);
	bool SendFailure(SOCKET client, sProtocolHeader::ePacketID packetID, sProtocolResponseFailure::eFailureCode failureCode, const std::string& errormsg);

private:

	//Step 1: Initial winsock
	bool Initialize();
	// Resolve the server address and port
	bool GetAddressInfo(addrinfo*& outAddressInfo);
	//Step 2: Create a Socket
	bool CreateSocket(const addrinfo& addressInfo);
	//Step 3: Bind the Socket
	bool BindSocket(const addrinfo& addressInfo);
	//Step 4: Listen for a client connection
	bool ListenConnection();

	//Step 5: Accept a client's connection
	bool AcceptConnection();
	//Step 6: Receiveing and sending data
	// reads from the incoming steam
	bool ReceiveData(const SOCKET client);

	bool ProcessAuthenticationTasks();
	bool ProcessReceivedDataFromUsers();
	bool RespondData(std::map<sProtocolHeader::ePacketID, std::set<SOCKET>>& responsesPacketIDs);
	// writes to the outgoing stream
	bool BroadcastData(std::set<SOCKET>& clients);
	bool SendData(const SOCKET client);

	// broadcast to peers
	///bool BroadcastLobbyUsers();
	//bool BroadcastData(cBuffer& buffer);

	//Step 7: Disconnect
	bool Disconnect(const SOCKET client);

	// when console close event occur, it make sure the clean up every thing before console shutdown
	static BOOL WINAPI ConsoleHandler(DWORD CEvent);

	bool m_lockAuthenticationTaskBuffer;
	std::queue<std::pair<sProtocolHeader::ePacketID, iUser*>> m_authenticationTasks;

	WSADATA m_windowsSocketApplicationData; //Windows Socket Application Data Object
	sockaddr_in m_clientSockAddr;

	//cPacketSender& m_sender;
	SOCKET m_listeningSocket;
	//cBuffer m_receiveBuffer;
	std::set<SOCKET> m_receiveFromUsers;
	std::map<sProtocolHeader::ePacketID, std::set<SOCKET>> m_responsesPacketIDs;

	char m_recvbuf[cBuffer::mc_DEFAULT_BUFFER_LENGTH];
	cBuffer m_sendBuffer;

	fd_set m_masterFileDiscriptor;    //Master list: // a set of file descriptors or in our cases sockets
	fd_set m_readFileDiscriptors;  //Temp list for select() statement

	int m_maximumFileDiscriptor;
	//int m_newFileDiscriptor;

	bool m_isStopped;

	static const char* mc_DEFAULT_PORT;

	sUserInfo m_userInfoBuff;

	sProtocolHeader m_headerSendBuff;
	sProtocolResponseFailure m_failureSendBuff;
	sProtocolLobbyUsers m_lobbyUsersSendBuff;

	sProtocolHeader m_headerRecvBuff;
	sProtocolAccount m_loginInfoRecvBuff;
	sProtocolRequestNewRoom m_newRoomRecvBuff;
	sProtocolRequestJoinRoom m_joinRoomRecvBuff;
	sProtocolSendChatMessage m_sendMsgRecvBuff;
	sProtocolRequestLeaveRoom m_reaveRoomRecvBuff;
};

#endif