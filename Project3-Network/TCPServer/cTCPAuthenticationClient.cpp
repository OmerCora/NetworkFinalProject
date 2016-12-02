#include "cTCPAuthenticationClient.h"

#include "cUserManager.h"
#include "cTCPServer.h"
#include <iostream>
#include <AuthenticationDef.pb.h>

const char* cTCPAuthenticationClient::mc_DEFAULT_PORT = "27016";
const char* cTCPAuthenticationClient::mc_DEFAULT_IPADDRESS = "127.0.0.1";
cTCPAuthenticationClient::cTCPAuthenticationClient()
{
}


cTCPAuthenticationClient::~cTCPAuthenticationClient()
{
}

cTCPAuthenticationClient* cTCPAuthenticationClient::m_instance = 0;
cTCPAuthenticationClient& cTCPAuthenticationClient::GetInstance()
{
	if (m_instance == 0)
		m_instance = new cTCPAuthenticationClient();
	return *m_instance;
}
void cTCPAuthenticationClient::DeleteInstance()
{
	if (m_instance)
		delete m_instance;
	m_instance = 0;
}

bool cTCPAuthenticationClient::StartClient()
{
	if (!this->Start(mc_DEFAULT_IPADDRESS, mc_DEFAULT_PORT))
		return false;

	return true;
}
void cTCPAuthenticationClient::StartSigninThread(iUser* user, sProtocolAccount& accountInfo)
{
	//RequestSignin(user, accountInfo);
	m_receiveThread = std::thread(&cTCPAuthenticationClient::RequestSignin, this, user, accountInfo);
}
void cTCPAuthenticationClient::StartLoginThread(iUser* user, sProtocolAccount& accountInfo)
{
	//RequestLogin(user, accountInfo);
	m_receiveThread = std::thread(&cTCPAuthenticationClient::RequestLogin, this, user, accountInfo);
}
void cTCPAuthenticationClient::RequestSignin(iUser* user, sProtocolAccount accountInfo)
{
	std::cout << "start thread: cTCPAuthenticationClient::RequestSignin" << std::endl;

	// google proto
	ProtocolAuthentication::CreateAccountWeb createProtocol;
	createProtocol.set_requestid(user->SocketID());
	createProtocol.set_email(accountInfo.username);
	createProtocol.set_plaintextpassword(accountInfo.password);

	std::string serializedString;
	createProtocol.SerializeToString(&serializedString);

	m_headerSendBuff.SetProtocol(serializedString);
	m_headerSendBuff.packet_id = sProtocolHeader::e_RequestSignin;

	m_sendBuffer.clear();
	m_sendBuffer.CheckBufferSize(m_headerSendBuff.packet_length);
	m_sendBuffer.Serialize(m_headerSendBuff);
	m_sendBuffer.loadBuffer(serializedString.c_str(), serializedString.size());

	this->SendData(m_socket);
	Sleep(1);
	while (true)
	{
		if (!this->ReceiveData(m_socket, m_receiveBuffer))
			return;

		// check message framing
		if (!m_receiveBuffer.isEmpty())
		{
			// check the complition of packet_length value
			if (sizeof(m_headerRecvBuff.packet_length) > m_receiveBuffer.getLength())
				continue;

			// check the total packet length
			m_headerRecvBuff.packet_length = m_receiveBuffer.readInt32BE(0);
			if (m_headerRecvBuff.packet_length > (int)(m_receiveBuffer.getLength() + sizeof(m_headerRecvBuff.packet_length)))
				continue;
		}

		while (!m_receiveBuffer.isEmpty())
		{
			m_receiveBuffer.Deserialize(m_headerRecvBuff);

			switch (m_headerRecvBuff.packet_id)
			{
			case sProtocolHeader::e_ResponseSigninSuccess:
			{
				std::cout << "e_ResponseSigninSuccess" << std::endl;

				ProtocolAuthentication::CreateAccountWebSuccess successProtocol;
				std::string bufferString = m_receiveBuffer.readStringBE(m_receiveBuffer.getLength());
				if (!successProtocol.ParseFromString(bufferString))
				{
					std::cout << "\tFailed to parse google protocol" << std::endl;
					cTCPServer::GetInstance().SendFailure(user->SocketID(), sProtocolHeader::e_ResponseSigninFailure, sProtocolResponseFailure::e_JustMessage, "Failed to parse google protocol");
					break;
				}

				std::cout << "\trequestid: " << successProtocol.requestid() << "userid: " << successProtocol.userid() << std::endl;

				user->AssignLoginInfo(accountInfo);

				cTCPServer::GetInstance().AddAuthenticationTask(m_headerRecvBuff.packet_id, user);
				break;
			}
			case sProtocolHeader::e_ResponseSigninFailure:
			{
				std::cout << "e_ResponseSigninFailure" << std::endl;

				ProtocolAuthentication::CreateAccountWebFailure failureProtocol;
				std::string bufferString = m_receiveBuffer.readStringBE(m_receiveBuffer.getLength());
				if (!failureProtocol.ParseFromString(bufferString))
				{
					std::cout << "\tFailed to parse google protocol" << std::endl;
					cTCPServer::GetInstance().SendFailure(user->SocketID(), sProtocolHeader::e_ResponseSigninFailure, sProtocolResponseFailure::e_JustMessage, "Failed to parse google protocol");
					break;
				}

				std::string failureMessage = ProtocolAuthentication::CreateAccountWebFailure::e_Reason_Name(failureProtocol.reason());
				std::cout << "\trequestid: " << failureProtocol.requestid() << " reason: " << failureMessage << std::endl;


				sProtocolResponseFailure::eFailureCode failureCode;
				if (failureProtocol.reason() == ProtocolAuthentication::CreateAccountWebFailure_e_Reason_ACCOUNT_ALREADY_EXISTS)
				{
					failureCode = sProtocolResponseFailure::e_HasnotSignedin;
				}
				else
				{
					failureCode = sProtocolResponseFailure::e_JustMessage;
				}


				cTCPServer::GetInstance().SendFailure(user->SocketID(), sProtocolHeader::e_ResponseSigninFailure, failureCode, failureMessage);
				break;
			}
			default:
				break;
			}
		}

		break;
	}

	m_receiveBuffer.clear();
	std::cout << "end thread: cTCPAuthenticationClient::RequestSignin" << std::endl;

	m_receiveThread.detach();
}
void cTCPAuthenticationClient::RequestLogin(iUser* user, sProtocolAccount accountInfo)
{
	std::cout << "start thread: cTCPAuthenticationClient::RequestLogin" << std::endl;


	//std::string password = "this.is.a.password";
	//std::string key = sha256(password);

	//std::cout << password << std::endl;
	//std::cout << key << std::endl;

	// my result
	//66a83a1f08f02cad2815b8f46d4921ffe6ef373d5465d4d3980ad491c10ad92e
	//online calulate result
	//66a83a1f08f02cad2815b8f46d4921ffe6ef373d5465d4d3980ad491c10ad92e

	//exit(EXIT_SUCCESS);

	// google proto
	ProtocolAuthentication::AuthenticateWeb authenticateProtocol;
	authenticateProtocol.set_requestid(user->SocketID());
	authenticateProtocol.set_email(accountInfo.username);
	authenticateProtocol.set_plaintextpassword(accountInfo.password);

	std::string serializedString;
	authenticateProtocol.SerializeToString(&serializedString);

	m_headerSendBuff.SetProtocol(serializedString);
	m_headerSendBuff.packet_id = sProtocolHeader::e_RequestLogin;

	m_sendBuffer.clear();
	m_sendBuffer.CheckBufferSize(m_headerSendBuff.packet_length);
	m_sendBuffer.Serialize(m_headerSendBuff);
	m_sendBuffer.loadBuffer(serializedString.c_str(), serializedString.size());


	this->SendData(m_socket);
	Sleep(1);

	while (true)
	{
		if (!this->ReceiveData(m_socket, m_receiveBuffer))
			return;


		// check message framing
		if(!m_receiveBuffer.isEmpty())
		{
			// check the complition of packet_length value
			if (sizeof(m_headerRecvBuff.packet_length) > m_receiveBuffer.getLength())
				continue;

			// check the total packet length
			m_headerRecvBuff.packet_length = m_receiveBuffer.readInt32BE(0);
			if (m_headerRecvBuff.packet_length > (int)(m_receiveBuffer.getLength() + sizeof(m_headerRecvBuff.packet_length)))
				continue;
		}

		while (!m_receiveBuffer.isEmpty())
		{
			m_receiveBuffer.Deserialize(m_headerRecvBuff);

			switch (m_headerRecvBuff.packet_id)
			{
			case sProtocolHeader::e_ResponseLoginSuccess:
			{
				std::cout << "e_ResponseLoginSuccess" << std::endl;

				ProtocolAuthentication::AuthenticateWebSuccess successProtocol;
				std::string bufferString = m_receiveBuffer.readStringBE(m_receiveBuffer.getLength());
				if (!successProtocol.ParseFromString(bufferString))
				{
					std::cout << "\tFailed to parse google protocol" << std::endl;
					cTCPServer::GetInstance().SendFailure(user->SocketID(), sProtocolHeader::e_ResponseLoginFailure, sProtocolResponseFailure::e_JustMessage, "Failed to parse google protocol");
					break;
				}

				std::cout << "\trequestid: " << successProtocol.requestid() << " userid: " << successProtocol.userid() << std::endl;

				user->AssignLoginInfo(accountInfo);

				cTCPServer::GetInstance().AddAuthenticationTask(m_headerRecvBuff.packet_id, user);

				break;
			}
			case sProtocolHeader::e_ResponseLoginFailure:
			{
				std::cout << "e_ResponseLoginFailure" << std::endl;

				ProtocolAuthentication::AuthenticateWebFailure failureProtocol;
				std::string bufferString = m_receiveBuffer.readStringBE(m_receiveBuffer.getLength());
				if (!failureProtocol.ParseFromString(bufferString))
				{
					std::cout << "\tFailed to parse google protocol" << std::endl;
					cTCPServer::GetInstance().SendFailure(user->SocketID(), sProtocolHeader::e_ResponseLoginFailure, sProtocolResponseFailure::e_JustMessage, "Failed to parse google protocol");
					break;
				}

				std::string failureMessage = ProtocolAuthentication::AuthenticateWebFailure::e_Reason_Name(failureProtocol.reason());
				std::cout << "\trequestid: " << failureProtocol.requestid() << " reason: " << failureMessage << std::endl;

				cTCPServer::GetInstance().SendFailure(user->SocketID(), sProtocolHeader::e_ResponseLoginFailure, sProtocolResponseFailure::e_JustMessage, failureMessage);
				break;
			}
			default:
				break;
			}
		}

		break;
	}

	m_receiveBuffer.clear();

	std::cout << "end thread: cTCPAuthenticationClient::RequestLogin" << std::endl;
	m_receiveThread.detach();
}

bool cTCPAuthenticationClient::EndClient()
{
	this->ShutDown();

	return true;
}

bool cTCPAuthenticationClient::ProcessReceivePackets(SOCKET socket, cBuffer& receiverBuffer)
{
#if 0
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
		case sProtocolHeader::e_CreateAccountSuccess:
		{
			std::cout << "e_CreateAccountSuccess" << std::endl;

			//receiverBuffer.Deserialize(m_loginInfoRecvBuff);

			// TODO: access db to compare account information (id & passsword)

			// TODO: record login time

			//this->SendSuccess(socket, sProtocolHeader::ePacketID::e_CreateAccountSuccess);

			break;
		}
		case sProtocolHeader::e_CreateAccountFailure:
		{
			std::cout << "e_CreateAccountFailure" << std::endl;
			break;
		}
		case sProtocolHeader::e_AuthenticateAccountSuccess:
		{
			std::cout << "e_AuthenticateAccountSuccess" << std::endl;

			//receiverBuffer.Deserialize(m_loginInfoRecvBuff);


			//this->SendSuccess(socket, sProtocolHeader::ePacketID::e_AuthenticateAccountSuccess);

			break;
		}
		case sProtocolHeader::e_AuthenticateAccountFailure:
		{
			std::cout << "e_AuthenticateAccountFailure" << std::endl;
			break;
		}
		default:
			break;
		}
	}

	receiverBuffer.clear();
#endif
	return true;
}


bool cTCPAuthenticationClient::Respond()
{
	return true;
}
