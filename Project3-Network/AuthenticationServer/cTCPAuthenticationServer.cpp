#include <stdio.h>
#include <iostream>
#include <set>

// for windows socket link
#pragma comment(lib, "Ws2_32.lib")

// need to link corenetwork.lib
#pragma comment (lib, "CoreNetwork.lib")


#include "cTCPAuthenticationServer.h"
#include "cErrorReport.h"

#include <AuthenticationDef.pb.h>


const char* cTCPAuthenticationServer::mc_DEFAULT_PORT = "27016";

cTCPAuthenticationServer::cTCPAuthenticationServer()
{
	m_authenticationDB.Open("Authentication.db");
}


cTCPAuthenticationServer::~cTCPAuthenticationServer()
{
	m_authenticationDB.Close();
}

cTCPAuthenticationServer* cTCPAuthenticationServer::m_instance = 0;
cTCPAuthenticationServer& cTCPAuthenticationServer::GetInstance()
{
	if (m_instance == 0)
		m_instance = new cTCPAuthenticationServer();
	return *m_instance;
}
void cTCPAuthenticationServer::DeleteInstance()
{
	if (m_instance)
		delete m_instance;
	m_instance = 0;
}

bool cTCPAuthenticationServer::ExecuteServer()
{

	//if (SetConsoleCtrlHandler(NULL, FALSE) == FALSE)
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE)
	{
		// unable to install handler... 
		// display message to the user
		printf("Unable to install handler!\n");
		return false;
	}

	if (!this->Start(0, mc_DEFAULT_PORT))
		return false;

	if (!this->Run())
		return false;
	
	this->ShutDown();

	return true;
}

bool cTCPAuthenticationServer::ProcessReceivePackets(SOCKET socket, cBuffer& receiverBuffer)
{
	while (!receiverBuffer.isEmpty())
	{
		// check message framing
		{
			// check the complition of packet_length value
			if (sizeof(m_headerRecvBuff.packet_length) > receiverBuffer.getLength())
				return true;

			// check the total packet length
			m_headerRecvBuff.packet_length = receiverBuffer.readInt32BE(0);
			if (m_headerRecvBuff.packet_length > (int)(receiverBuffer.getLength() + sizeof(m_headerRecvBuff.packet_length)))
				return true;
		}

		receiverBuffer.Deserialize(m_headerRecvBuff);

		switch (m_headerRecvBuff.packet_id)
		{
		case sProtocolHeader::e_RequestSignin:
		{
			std::cout << "e_CreateAccount" << std::endl;

			ProtocolAuthentication::CreateAccountWeb signinProtocol;
			std::string bufferString = receiverBuffer.readStringBE(receiverBuffer.getLength());
			if (!signinProtocol.ParseFromString(bufferString))
			{
				std::cout << "\tFailed to parse google protocol" << std::endl;
				this->SendFailure(socket, signinProtocol.requestid(),
					ProtocolAuthentication::CreateAccountWebFailure_e_Reason::CreateAccountWebFailure_e_Reason_FAILED_TO_PARSE_PROTOCOL);
				break;
			}

			std::cout << "\trequestid: " << signinProtocol.requestid() << " email: " << signinProtocol.email()
				<< " password: " << signinProtocol.plaintextpassword() << std::endl;

			// check server status
			if (!m_authenticationDB.IsOpen())
			{
				std::cout << "\tCreateAccountWebFailure_e_Reason_INTERNAL_SERVER_ERROR" << std::endl;
				this->SendFailure(socket, signinProtocol.requestid(),
					ProtocolAuthentication::CreateAccountWebFailure_e_Reason::CreateAccountWebFailure_e_Reason_INTERNAL_SERVER_ERROR);
				break;
			}

			// compare account information (id & passsword)
			if (m_authenticationDB.UserExist(signinProtocol.email()))
			{
				std::cout << "\tCreateAccountWebFailure_e_Reason_ACCOUNT_ALREADY_EXISTS" << std::endl;
				this->SendFailure(socket, signinProtocol.requestid(),
					ProtocolAuthentication::CreateAccountWebFailure_e_Reason::CreateAccountWebFailure_e_Reason_ACCOUNT_ALREADY_EXISTS);
				break;
			}

			if (signinProtocol.plaintextpassword().size() < 8)
			{
				std::cout << "\tCreateAccountWebFailure_e_Reason_INVALID_PASSWORD" << std::endl;
				this->SendFailure(socket, signinProtocol.requestid(),
					ProtocolAuthentication::CreateAccountWebFailure_e_Reason::CreateAccountWebFailure_e_Reason_INVALID_PASSWORD);
				break;
			}

			// sign-in
			if (!m_authenticationDB.Signin(signinProtocol))
			{
				std::cout << "\tCreateAccountWebFailure_e_Reason_UNKNOWN_ERROR" << std::endl;
				this->SendFailure(socket, signinProtocol.requestid(),
					ProtocolAuthentication::CreateAccountWebFailure_e_Reason::CreateAccountWebFailure_e_Reason_UNKNOWN_ERROR);
				break;
			}

			this->SendSuccess(socket, sProtocolHeader::ePacketID::e_ResponseSigninSuccess, signinProtocol.requestid());
			std::cout << "sign-in success" << std::endl;

			break;
		}
		case sProtocolHeader::e_RequestLogin:
		{
			std::cout << "e_AuthenticateAccount" << std::endl;

			ProtocolAuthentication::AuthenticateWeb loginProtocol;
			std::string bufferString = receiverBuffer.readStringBE(receiverBuffer.getLength());
			if (!loginProtocol.ParseFromString(bufferString))
			{
				std::cout << "\tAuthenticateWebFailure_e_Reason_FAILED_TO_PARSE_PROTOCOL" << std::endl;
				this->SendFailure(socket, loginProtocol.requestid(),
					ProtocolAuthentication::AuthenticateWebFailure_e_Reason::AuthenticateWebFailure_e_Reason_FAILED_TO_PARSE_PROTOCOL);
				break;
			}

			std::cout << "\trequestid: " << loginProtocol.requestid() << " email: " << loginProtocol.email()
				<< " password: " << loginProtocol.plaintextpassword() << std::endl;


			// check server status
			if (!m_authenticationDB.IsOpen())
			{
				std::cout << "\tAuthenticateWebFailure_e_Reason_INTERNAL_SERVER_ERROR" << std::endl;
				this->SendFailure(socket, loginProtocol.requestid(),
					ProtocolAuthentication::AuthenticateWebFailure_e_Reason::AuthenticateWebFailure_e_Reason_INTERNAL_SERVER_ERROR);
				break;
			}

			// compare account information (id & passsword)
			if (!m_authenticationDB.UserExist(loginProtocol.email()))
			{
				std::cout << "\tAuthenticateWebFailure_e_Reason_INVALID_CREDENTIALS" << std::endl;
				this->SendFailure(socket, loginProtocol.requestid(),
					ProtocolAuthentication::AuthenticateWebFailure_e_Reason::AuthenticateWebFailure_e_Reason_INVALID_CREDENTIALS);
				break;
			}

			// log-in
			if (!m_authenticationDB.Login(loginProtocol))
			{
				std::cout << "\tAuthenticateWebFailure_e_Reason_INVALID_CREDENTIALS" << std::endl;
				this->SendFailure(socket, loginProtocol.requestid(),
					ProtocolAuthentication::AuthenticateWebFailure_e_Reason::AuthenticateWebFailure_e_Reason_INVALID_CREDENTIALS);
				break;
			}

			this->SendSuccess(socket, sProtocolHeader::ePacketID::e_ResponseLoginSuccess, loginProtocol.requestid());
			std::cout << "log-in success" << std::endl;

			break;
		}
		default:
			break;
		}
	}

	receiverBuffer.clear();

	return true;
}


bool cTCPAuthenticationServer::Respond()
{
	return true;
}



bool cTCPAuthenticationServer::SendSuccess(SOCKET client, sProtocolHeader::ePacketID packetID, google::protobuf::uint64 requestID)
{
	std::string serializedString;

	if (packetID == sProtocolHeader::e_ResponseSigninSuccess)
	{
		ProtocolAuthentication::CreateAccountWebSuccess successProtocol;
		successProtocol.set_requestid(requestID);
		successProtocol.set_userid(0);
		successProtocol.set_creationdate("");
		successProtocol.SerializeToString(&serializedString);
	}
	else if (packetID == sProtocolHeader::e_ResponseLoginSuccess)
	{
		ProtocolAuthentication::AuthenticateWebSuccess successProtocol;
		successProtocol.set_requestid(requestID);
		successProtocol.set_userid(0);
		successProtocol.set_logindate("");
		successProtocol.SerializeToString(&serializedString);
	}

	m_headerSendBuff.SetProtocol(serializedString);
	m_headerSendBuff.packet_id = packetID;

	m_sendBuffer.clear();
	m_sendBuffer.CheckBufferSize(m_headerSendBuff.packet_length);
	m_sendBuffer.Serialize(m_headerSendBuff);
	m_sendBuffer.loadBuffer(serializedString.c_str(), serializedString.size());

	std::cout << "\tSendSuccess serialize length: " << m_sendBuffer.getLength() << " original size: " << m_headerSendBuff.packet_length << std::endl;

	if (!this->SendData(client))
	{
		this->Disconnect(client);
		return false;
	}
	return true;
}

bool cTCPAuthenticationServer::SendFailure(SOCKET client, google::protobuf::uint64 requestID, 
	ProtocolAuthentication::CreateAccountWebFailure_e_Reason reason)
{
	std::string serializedString;

	ProtocolAuthentication::CreateAccountWebFailure failureProtocol;
	failureProtocol.set_requestid(requestID);
	failureProtocol.set_reason(reason);
	failureProtocol.SerializeToString(&serializedString);

	m_headerSendBuff.SetProtocol(serializedString);
	m_headerSendBuff.packet_id = sProtocolHeader::e_ResponseSigninFailure;

	m_sendBuffer.clear();
	m_sendBuffer.CheckBufferSize(m_headerSendBuff.packet_length);
	m_sendBuffer.Serialize(m_headerSendBuff);
	m_sendBuffer.loadBuffer(serializedString.c_str(), serializedString.size());

	std::cout << "\tSendFailure serialize length: " << m_sendBuffer.getLength() << " original size: " << m_headerSendBuff.packet_length << std::endl;

	if (!this->SendData(client))
	{
		this->Disconnect(client);
		return false;
	}
	return true;
}

bool cTCPAuthenticationServer::SendFailure(SOCKET client, google::protobuf::uint64 requestID,
	ProtocolAuthentication::AuthenticateWebFailure_e_Reason reason)
{
	std::string serializedString;

	ProtocolAuthentication::AuthenticateWebFailure failureProtocol;
	failureProtocol.set_requestid(requestID);
	failureProtocol.set_reason(reason);
	failureProtocol.SerializeToString(&serializedString);

	m_headerSendBuff.SetProtocol(serializedString);
	m_headerSendBuff.packet_id = sProtocolHeader::e_ResponseLoginFailure;

	m_sendBuffer.clear();
	m_sendBuffer.CheckBufferSize(m_headerSendBuff.packet_length);
	m_sendBuffer.Serialize(m_headerSendBuff);
	m_sendBuffer.loadBuffer(serializedString.c_str(), serializedString.size());

	std::cout << "\tSendFailure serialize length: " << m_sendBuffer.getLength() << " original size: " << m_headerSendBuff.packet_length << std::endl;

	if (!this->SendData(client))
	{
		this->Disconnect(client);
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
void cTCPAuthenticationServer::SignalHandler()
{
	cTCPAuthenticationServer::GetInstance().m_isStopped = true;
	//cTCPAuthenticationServer::GetInstance().ShutDown();
}
#endif

// when console close event occur, it make sure the clean up every thing before console shutdown
BOOL WINAPI cTCPAuthenticationServer::ConsoleHandler(DWORD CEvent)
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

		cTCPAuthenticationServer::GetInstance().m_isStopped = true;
		while (cTCPAuthenticationServer::m_instance != 0)
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
