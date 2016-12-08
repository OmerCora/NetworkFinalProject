

#include <stdio.h>
#include <iostream>
#include <limits>
#include <conio.h>
#include <sstream>
#include <set>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

// need to link corenetwork.lib
#pragma comment (lib, "CoreNetwork.lib")

#include "cTCPClient.h"
#include "cErrorReport.h"
#include "cConsolTool.h"
#include "cClientPacketProcedureMonopoly.h"

const char* cTCPClient::mc_DEFAULT_PORT = "27015";
const char* cTCPClient::mc_DEFAULT_IPADDRESS = "127.0.0.1";

cTCPClient::cTCPClient()
	: m_isStopped(false)
	, m_isConnected(false)
	, m_connectedSocket(INVALID_SOCKET)
	, m_gameMonopolyState(eGameMonopolyState::e_GM_Wait)
	, m_isDebug(false)

	, m_menuState(eChatMenuState::e_Connect)
	, m_gameStarted(false)
{
	m_instance = this;
}


cTCPClient::~cTCPClient()
{
}


cTCPClient* cTCPClient::m_instance = 0;
cTCPClient& cTCPClient::GetInstance()
{
	//if (m_instance == 0)
	//	m_instance = new cTCPClient();
	return *m_instance;
}
//void cTCPClient::DeleteInstance()
//{
//	if (m_instance)
//		delete m_instance;
//	m_instance = 0;
//}



bool cTCPClient::StartClient()
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

	return true;
}


void cTCPClient::PrintTitlePage(const std::string& title)
{
	if (m_gameStarted) return;
	cConsolTool::Reference().StartPrinting();
	cConsolTool::Reference().DrawRectangle(5, 2, 100, 5);
	cConsolTool::Reference().PrintPos((short)(55 - title.size() / 2), 4, title.c_str());
	cConsolTool::Reference().DrawRectangle(5, 7, 100, 20);
	cConsolTool::Reference().EndPrinting(false);
}
void cTCPClient::PrintLobbyUsers()
{
	if (m_gameStarted) return;
	cConsolTool::Reference().StartPrinting();
	// lobby users
	cConsolTool::Reference().DrawRectangle(89, 2, 16, 25);

	cConsolTool::Reference().SetCursorPos(91, 3);
	printf("Lobby Users(%d)", m_lobbyUsers.numberOfUsers);
	for (int i = 0; i < m_lobbyUsers.numberOfUsers; ++i)
	{
		cConsolTool::Reference().SetCursorPos(91, i+4);
		printf("%s", m_lobbyUsers.users[i].username.c_str());
	}
	cConsolTool::Reference().EndPrinting(false);
}
void cTCPClient::PrintRoomList()
{
	if (m_gameStarted) return;
	cConsolTool::Reference().StartPrinting();
	// room list
	cConsolTool::Reference().DrawRectangle(5, 10, 82, 17);

	cConsolTool::Reference().SetCursorPos(7, 11);
	std::cout << "[Room List]";
	for (int i = 0; i < m_roomList.numberOfRooms; ++i)
	{
		SetCursorPos(7, 12+i);
		std::cout << (i+1) << ": " << m_roomList.rooms[i].roomname << " (" << m_roomList.user_counts[i] << " Users)";
	}
	cConsolTool::Reference().EndPrinting(false);
}
void cTCPClient::PrintCurrentRoomUsers()
{
	if (m_gameStarted) return;
	cConsolTool::Reference().StartPrinting();

	// room users
	cConsolTool::Reference().DrawRectangle(5, 10, 82, 4);

	cConsolTool::Reference().SetCursorPos(7, 11);
	cConsolTool::Reference().PrintPos(7, 11, "[Users] ");
	if (m_currentRoomInfo.users.size() > 0)
	{
		cConsolTool::Reference().PrintPos(7, 12, m_currentRoomInfo.users[0].username.c_str());
		for (int i = 1; i < m_currentRoomInfo.numberOfUsers; ++i)
		{
			std::cout << ", " << m_currentRoomInfo.users[i].username;
		}
	}
	cConsolTool::Reference().EndPrinting(false);
}
void cTCPClient::PrintChatHistory()
{
	if (m_gameStarted) return;
	cConsolTool::Reference().StartPrinting();

	// chat history
	cConsolTool::Reference().DrawRectangle(5, 14, 82, 13);

	cConsolTool::Reference().SetCursorPos(7, 15);
	std::cout << "\t[Chat History]\n";
	int startIndex = 0;
	if (m_chatHistory.numberOfMessages > 10)
	{
		startIndex = m_chatHistory.numberOfMessages - 10;
	}
	for (int i = 0; i < 10; ++i)
	{
		if (startIndex + i >= m_chatHistory.numberOfMessages)
		{
			std::cout << "\t\n";
		}
		else
		{
			std::cout << "\t" << m_chatHistory.chatHistory[startIndex + i].message << "\n";
		}
	}
	std::cout << "\n\n";
	cConsolTool::Reference().EndPrinting(false);
}


void cTCPClient::RequestLogout()
{
	// request leave room
	sProtocolHeader header;
	header.packet_id = sProtocolHeader::e_RequestLogout;
	header.packet_length = sizeof(sProtocolHeader);

	m_sendBuffer.CheckBufferSize(header.packet_length);
	m_sendBuffer.Serialize(header);

	this->SendData(m_connectedSocket, m_sendBuffer);
}
void cTCPClient::RequestDisconnect()
{
	// request leave room
	sProtocolHeader header;
	header.packet_id = sProtocolHeader::e_RequestDisconnect;
	header.packet_length = sizeof(sProtocolHeader);

	m_sendBuffer.CheckBufferSize(header.packet_length);
	m_sendBuffer.Serialize(header);

	if (!this->SendData(m_connectedSocket, m_sendBuffer))
	{
		m_isStopped = true;
	}
}

void cTCPClient::SendLeaveRoom()
{
	// clear chat history
	m_chatHistory.numberOfMessages = 0;
	m_chatHistory.chatHistory.clear();

	// request leave room
	sProtocolRequestLeaveRoom reaveRoom;

	reaveRoom.username = this->m_userName;
	reaveRoom.username_length = (short)this->m_userName.size();
	reaveRoom.roomname = this->m_currentRoomInfo.roomname;
	reaveRoom.roomname_length = (short)this->m_currentRoomInfo.roomname.size();
	sProtocolHeader header;
	header.SetProtocol(reaveRoom);

	m_sendBuffer.CheckBufferSize(header.packet_length);
	m_sendBuffer.Serialize(header);
	m_sendBuffer.Serialize(reaveRoom);

	this->SendData(m_connectedSocket, m_sendBuffer);
}

void cTCPClient::UserSendThread()
{
	while (!m_isStopped)
	{
		//if(!m_isDebug)
			//system("cls");

		// always print lobby users
		//this->PrintLobbyUsers();

		if (m_menuState == eChatMenuState::e_PlayGame)
		{
			std::cout << "eChatMenuState::e_PlayGame" << std::endl;
			// do nothing
			while (m_gameStarted)
			{
				Sleep(1000);
			}

			system("cls");
		}
		else if (m_menuState == eChatMenuState::e_Connect)
		{
			cConsolTool::Reference().StartPrinting();
			cConsolTool::Reference().DrawRectangle(5, 2, 100, 25);
			std::string notice = "[Connecting Server ...]";
			cConsolTool::Reference().PrintPos((short)(55 - notice.size()/2), 12, notice.c_str());
			cConsolTool::Reference().EndPrinting(false);

			int waitForSecound = 0;
			while (!m_isConnected && !m_isStopped)
			{
				std::stringstream msg;
				msg << notice << "(" << waitForSecound << ")";
				cConsolTool::Reference().StartPrinting();
				cConsolTool::Reference().PrintPos((short)(55 - msg.str().size() / 2), 12, msg.str().c_str());
				cConsolTool::Reference().EndPrinting(false);
				Sleep(1000);
				waitForSecound++;
			}
		}
		else if (m_menuState == eChatMenuState::e_StartMenu)
		{
			PrintTitlePage("[Sign-in / Log-in]");

			int selectedItem = -1;
			cConsolTool::Reference().StartPrinting();
			cConsolTool::Reference().PrintPos(40, 10, "ESC. Quit");
			cConsolTool::Reference().PrintPos(40, 11, "1. Sign-in");
			cConsolTool::Reference().PrintPos(40, 12, "2. Log-in");
			cConsolTool::Reference().PrintPos(40, 14, "Select:                        ");
			cConsolTool::Reference().SetCursorPos(49, 14);
			cConsolTool::Reference().EndPrinting(true);

			if (!cConsolTool::Reference().InputNumber(selectedItem))
			{
				this->RequestDisconnect();
				while (m_menuState == eChatMenuState::e_StartMenu)
				{
					Sleep(10);
				}
			}
			else if (selectedItem == 1)
			{
				m_menuState = eChatMenuState::e_Signin;
			}
			else if (selectedItem == 2)
			{
				m_menuState = eChatMenuState::e_Login;
			}
			else
			{
				// do nothing
			}
		}
		else if (m_menuState == eChatMenuState::e_Signin)
		{
			// request signin
			PrintTitlePage("[Sign-in]");
			cConsolTool::Reference().StartPrinting();

			// input new room name
			cConsolTool::Reference().PrintPos(40, 11, "Press ESC key to exit");
			cConsolTool::Reference().PrintPos(40, 13, "[User e-mail: ]");
			cConsolTool::Reference().PrintPos(40, 14, "[Password   : ]");
			cConsolTool::Reference().PrintPos(40, 15, "[Password-Re: ]");
			cConsolTool::Reference().SetCursorPos(54, 13);
			cConsolTool::Reference().EndPrinting(true);

			const int MAXIMUM_EMAIL_PASS = 32;
			sProtocolAccount signinInfo;
			if (!cConsolTool::Reference().InputName("User e-mail", MAXIMUM_EMAIL_PASS, signinInfo.username, 40, 13))
			{
				m_menuState = eChatMenuState::e_StartMenu;
			}
			else if (signinInfo.username.empty())
			{
				// wait for input name
				// do nothing
			}
			else
			{
				// password
				if (!cConsolTool::Reference().InputPassword("Password   ", MAXIMUM_EMAIL_PASS, signinInfo.password, 40, 14))
				{
					m_menuState = eChatMenuState::e_StartMenu;
				}
				else if (signinInfo.password.empty())
				{
					// do nothing
				}
				else
				{
					// password-re
					std::string password_re;
					if (!cConsolTool::Reference().InputPassword("Password-Re", MAXIMUM_EMAIL_PASS, password_re, 40, 15))
					{
						m_menuState = eChatMenuState::e_StartMenu;
					}
					else if (password_re != signinInfo.password)
					{
						// do nothing
					}
					else
					{
						signinInfo.username_length = (short)signinInfo.username.size();
						signinInfo.password_length = (short)signinInfo.password.size();

						sProtocolHeader header;
						header.SetProtocol(signinInfo);
						header.packet_id = sProtocolHeader::e_RequestSignin;

						m_sendBuffer.CheckBufferSize(header.packet_length);
						m_sendBuffer.Serialize(header);
						m_sendBuffer.Serialize(signinInfo);

						this->SendData(m_connectedSocket, m_sendBuffer);

						while (m_menuState == eChatMenuState::e_Signin)
						{
							Sleep(10);
						}
					}
				}
			}
		}
		else if (m_menuState == eChatMenuState::e_SigninSuccess)
		{
			PrintTitlePage("[Sign-in]");

			cConsolTool::Reference().StartPrinting();
			cConsolTool::Reference().PrintPos(40, 13, "Sign-in Success.");
			cConsolTool::Reference().EndPrinting(false);

			m_menuState = eChatMenuState::e_MainMenu;
		}
		else if (m_menuState == eChatMenuState::e_SigninFailure)
		{
			PrintTitlePage("[Sign-in]");

			int waitsec = 3;
			while (waitsec != 0)
			{
				waitsec--;
				cConsolTool::Reference().StartPrinting();
				std::stringstream msg;
				msg << "Sign-in failed. Try again...(" << waitsec << ")";
				cConsolTool::Reference().PrintPos(40, 13, msg.str().c_str());
				cConsolTool::Reference().EndPrinting(false);
				Sleep(1000);
			}

			m_menuState = eChatMenuState::e_Signin;
		}
		else if (m_menuState == eChatMenuState::e_Login)
		{
			// TODO: this is for debug
#if 0
			sProtocolHeader header;
			sProtocolAccount loginInfo;
			header.SetProtocol(loginInfo);
			header.packet_id = sProtocolHeader::e_RequestLogin;

			m_sendBuffer.CheckBufferSize(header.packet_length);
			m_sendBuffer.Serialize(header);
			m_sendBuffer.Serialize(loginInfo);

			this->SendData(m_connectedSocket, m_sendBuffer);

			while (m_menuState == eChatMenuState::e_Login)
			{
				Sleep(10);
			}
#else

			// request login
			PrintTitlePage("[Log-in]");

			cConsolTool::Reference().StartPrinting();
			// input new room name
			sProtocolAccount loginInfo;
			cConsolTool::Reference().PrintPos(40, 11, "Press ESC key to exit");
			cConsolTool::Reference().PrintPos(40, 13, "[User e-mail: ]");
			cConsolTool::Reference().PrintPos(40, 14, "[Password   : ]");
			cConsolTool::Reference().SetCursorPos(54, 13);
			cConsolTool::Reference().EndPrinting(true);

			const int MAXIMUM_EMAIL_PASS = 32;
			if (!cConsolTool::Reference().InputName("User e-mail", MAXIMUM_EMAIL_PASS, loginInfo.username, 40, 13))
			{
				m_menuState = eChatMenuState::e_StartMenu;
			}
			else if (loginInfo.username.empty())
			{
				// wait for input name
				// do nothing
			}
			else
			{
				// user name
				if (!cConsolTool::Reference().InputPassword("Password   ", MAXIMUM_EMAIL_PASS, loginInfo.password, 40, 14))
				{
					m_menuState = eChatMenuState::e_StartMenu;
				}
				else if (loginInfo.password.empty())
				{
					// do nothing
				}
				else
				{
					loginInfo.username_length = (short)loginInfo.username.size();
					loginInfo.password_length = (short)loginInfo.password.size();

					sProtocolHeader header;
					header.SetProtocol(loginInfo);
					header.packet_id = sProtocolHeader::e_RequestLogin;

					m_sendBuffer.CheckBufferSize(header.packet_length);
					m_sendBuffer.Serialize(header);
					m_sendBuffer.Serialize(loginInfo);

					this->SendData(m_connectedSocket, m_sendBuffer);

					while (m_menuState == eChatMenuState::e_Login)
					{
						Sleep(10);
					}
				}
			}
#endif
		}
		else if (m_menuState == eChatMenuState::e_LoginSuccess)
		{
			PrintTitlePage("[Log-in]");

			cConsolTool::Reference().StartPrinting();
			cConsolTool::Reference().PrintPos(40, 13, "Log-in Success.");
			cConsolTool::Reference().EndPrinting(false);

			m_menuState = eChatMenuState::e_MainMenu;
		}
		else if (m_menuState == eChatMenuState::e_LoginFailure)
		{
			PrintTitlePage("[Log-in]");

			int waitsec = 3;
			while (waitsec != 0)
			{
				waitsec--;
				cConsolTool::Reference().StartPrinting();
				std::stringstream msg;
				msg << "Log-in failed. Try again...(" << waitsec << ")";
				cConsolTool::Reference().PrintPos(40, 13, msg.str().c_str());
				cConsolTool::Reference().EndPrinting(false);
				Sleep(1000);
			}

			m_menuState = eChatMenuState::e_Login;
		}
		else if (m_menuState == eChatMenuState::e_MainMenu)
		{
			// main menu

			// room list
			this->PrintRoomList();
			this->PrintLobbyUsers();

			cConsolTool::Reference().StartPrinting();
			cConsolTool::Reference().DrawRectangle(5, 2, 82, 8);
			cConsolTool::Reference().PrintPos(7, 3, "[Main Menu]");
			cConsolTool::Reference().PrintPos(7, 4, "ESC. Log-out");
			cConsolTool::Reference().PrintPos(7, 5, "1. Create Room");
			cConsolTool::Reference().PrintPos(7, 6, "2. Join Room");
			cConsolTool::Reference().PrintPos(7, 8, "Select Menu: ");
			cConsolTool::Reference().EndPrinting(true);

			int selectionMenu = -1;
			if (!cConsolTool::Reference().InputNumber(selectionMenu))
			{
				this->RequestLogout();

				while (m_menuState == eChatMenuState::e_MainMenu)
				{
					Sleep(10);
				}
			}
			else if (selectionMenu == 1)
			{
				m_menuState = eChatMenuState::e_CreateRoom;
			}
			else if (selectionMenu == 2)
			{
				m_menuState = eChatMenuState::e_JoinRoom;
			}
		}
		else if (m_menuState == eChatMenuState::e_CreateRoom)
		{
			// create room

			// room list
			this->PrintRoomList();
			this->PrintLobbyUsers();

			cConsolTool::Reference().StartPrinting();
			cConsolTool::Reference().DrawRectangle(5, 2, 82, 8);
			cConsolTool::Reference().PrintPos(7, 3, "[Create Room]");
			cConsolTool::Reference().PrintPos(7, 4, "ESC. Return to Main Menu");
			cConsolTool::Reference().PrintPos(7, 7, "[Room Name: ]");
			cConsolTool::Reference().PrintPos(7, 8, "[Nick Name: ]");
			cConsolTool::Reference().EndPrinting(true);

			// input new room name
			const int MAXIMUM_ROOM_NAME = 32;
			const char SPACE = 32;
			std::string newName;
			if (!cConsolTool::Reference().InputName("Room Name", MAXIMUM_ROOM_NAME, newName, 7, 7, SPACE))
			{
				m_menuState = eChatMenuState::e_MainMenu;
			}
			else if (newName.empty())
			{
				// wait for input name
			}
			else
			{
				bool isExistRoom = false;
				for (int i = 0; i < m_roomList.numberOfRooms; ++i)
				{
					if (m_roomList.rooms[i].roomname.compare(newName) == 0)
					{
						// do nothing
						isExistRoom = true;
					}
				}

				if (!isExistRoom)
				{
					if (!cConsolTool::Reference().InputName("Nick Name", MAXIMUM_ROOM_NAME, m_userName, 7, 8, SPACE))
					{
					}
					else if (m_userName.empty())
					{
						// do nothing
					}
					else
					{
						// request create a room
						sProtocolRequestNewRoom newRoom;
						newRoom.username = m_userName;
						newRoom.username_length = (short)m_userName.size();
						newRoom.roomname = newName;
						newRoom.roomname_length = (short)newName.size();

						sProtocolHeader header;
						header.SetProtocol(newRoom);

						m_sendBuffer.CheckBufferSize(header.packet_length);
						m_sendBuffer.Serialize(header);
						m_sendBuffer.Serialize(newRoom);

						this->SendData(m_connectedSocket, m_sendBuffer);

						while (m_menuState == eChatMenuState::e_CreateRoom)
						{
							Sleep(10);
						}
					}
				}
			}
		}
		else if (m_menuState == eChatMenuState::e_CreateRoomSuccess)
		{
			PrintTitlePage("[Creating a room]");

			cConsolTool::Reference().StartPrinting();
			cConsolTool::Reference().PrintPos(40, 13, "Creating a room Success.");
			cConsolTool::Reference().EndPrinting(false);

			//m_menuState = eChatMenuState::e_ChatRoom;
			//m_menuState = eChatMenuState::e_PlayGame;
		}
		else if (m_menuState == eChatMenuState::e_CreateRoomFailure)
		{
			PrintTitlePage("[Creating a room]");

			int waitsec = 3;
			while (waitsec != 0)
			{
				waitsec--;
				cConsolTool::Reference().StartPrinting();
				std::stringstream msg;
				msg << "Creating a room failed. Try again...(" << waitsec << ")";
				cConsolTool::Reference().PrintPos(40, 13, msg.str().c_str());
				cConsolTool::Reference().EndPrinting(false);
				Sleep(1000);
			}

			m_menuState = eChatMenuState::e_CreateRoom;
		}
		else if (m_menuState == eChatMenuState::e_JoinRoom)
		{
			// join room

			// room list
			this->PrintRoomList();
			this->PrintLobbyUsers();

			cConsolTool::Reference().StartPrinting();
			cConsolTool::Reference().DrawRectangle(5, 2, 82, 8);

			cConsolTool::Reference().PrintPos(7, 3, "[Join Room]");
			cConsolTool::Reference().PrintPos(7, 4, "ESC. Return to Main Menu");


			// select a room
			cConsolTool::Reference().PrintPos(7, 7, "[Select a Room Number:  ]");
			cConsolTool::Reference().PrintPos(7, 8, "[Nick Name: ]");
			cConsolTool::Reference().SetCursorPos(30, 7);
			cConsolTool::Reference().EndPrinting(true);

			int selectIndex = -1;
			if (!cConsolTool::Reference().InputNumber(selectIndex))
			{
				m_menuState = eChatMenuState::e_MainMenu;
			}
			else if (selectIndex<1 || selectIndex > m_roomList.numberOfRooms)
			{
				// do nothing
			}
			else
			{
				const int MAXIMUM_ROOM_NAME = 32;
				const char SPACE = 32;
				// user name
				if (!cConsolTool::Reference().InputName("Nick Name", MAXIMUM_ROOM_NAME, m_userName, 7, 8, SPACE))
				{
				}
				else if (m_userName.empty())
				{
					// do nothing
				}
				else
				{
					// request join a room
					sProtocolRequestJoinRoom joinRoom;
					joinRoom.roomname = m_roomList.rooms[selectIndex - 1].roomname;
					joinRoom.roomname_length = (short)joinRoom.roomname.size();

					joinRoom.username = m_userName;
					joinRoom.username_length = (short)m_userName.size();

					sProtocolHeader header;
					header.SetProtocol(joinRoom);

					m_sendBuffer.CheckBufferSize(header.packet_length);
					m_sendBuffer.Serialize(header);
					m_sendBuffer.Serialize(joinRoom);

					this->SendData(m_connectedSocket, m_sendBuffer);

					while (m_menuState == eChatMenuState::e_JoinRoom)
					{
						Sleep(10);
					}
				}
			}
		}
		else if (m_menuState == eChatMenuState::e_JoinRoomSuccess)
		{
			PrintTitlePage("[Joining a room]");

			cConsolTool::Reference().StartPrinting();
			cConsolTool::Reference().PrintPos(40, 13, "Joining a room Success.");
			cConsolTool::Reference().EndPrinting(false);

			//m_menuState = eChatMenuState::e_ChatRoom;
			//m_menuState = eChatMenuState::e_PlayGame;
		}
		else if (m_menuState == eChatMenuState::e_JoinRoomFailure)
		{
			PrintTitlePage("[Joining a room]");

			int waitsec = 3;
			while (waitsec != 0)
			{
				waitsec--;
				cConsolTool::Reference().StartPrinting();
				std::stringstream msg;
				msg << "Joining a room failed. Try again...(" << waitsec << ")";
				cConsolTool::Reference().PrintPos(40, 13, msg.str().c_str());
				cConsolTool::Reference().EndPrinting(false);
				Sleep(1000);
			}
			m_menuState = eChatMenuState::e_JoinRoom;
		}
		else if (m_menuState == eChatMenuState::e_ChatRoom)
		{
			// enter room

			this->PrintCurrentRoomUsers();
			this->PrintChatHistory();
			this->PrintLobbyUsers();

			cConsolTool::Reference().StartPrinting();
			cConsolTool::Reference().DrawRectangle(5, 2, 82, 8);

			cConsolTool::Reference().PrintPos(7, 3, ("[Room(" + m_currentRoomInfo.roomname+ ") Menu]").c_str());
			cConsolTool::Reference().PrintPos(7, 4, "Press ESC to Exit Room");

			// chat message
			cConsolTool::Reference().PrintPos(7, 7, (m_userName + "'s Message: ").c_str());
			cConsolTool::Reference().EndPrinting(true);
			std::string message;
			//std::cin >> message;
			const int MAXIMUM_MESSAGE_LENGTH = 512;
			const char SPACE = 32;
			if (!cConsolTool::Reference().InputName(m_userName + "'s Message", MAXIMUM_MESSAGE_LENGTH, message, 7, 7, SPACE))
			{
				message.clear();

				this->SendLeaveRoom();
				while (m_menuState == eChatMenuState::e_ChatRoom)
				{
					Sleep(10);
				}
			}
			else if (message.empty())
			{
				// do nothing
			}
			else
			{
				// send message
				sProtocolSendChatMessage chatMessage;

				chatMessage.username = m_userName;
				chatMessage.username_length = (short)m_userName.size();

				chatMessage.roomname = m_currentRoomInfo.roomname;
				chatMessage.roomname_length = (short)m_currentRoomInfo.roomname.size();

				chatMessage.message = message;
				chatMessage.message_length = message.size();

				sProtocolHeader header;
				header.SetProtocol(chatMessage);

				m_sendBuffer.CheckBufferSize(header.packet_length);
				m_sendBuffer.Serialize(header);
				m_sendBuffer.Serialize(chatMessage);

				this->SendData(m_connectedSocket, m_sendBuffer);
			}
		}
		else if (m_menuState == eChatMenuState::e_LeaveRoomSuccess)
		{
			PrintTitlePage("[Leaving a room]");

			cConsolTool::Reference().StartPrinting();
			cConsolTool::Reference().PrintPos(40, 13, "Leaving a room Success.");
			cConsolTool::Reference().EndPrinting(false);

			m_menuState = eChatMenuState::e_JoinRoom;
		}
		else if (m_menuState == eChatMenuState::e_LeaveRoomFailure)
		{
			PrintTitlePage("[Leaving a room]");

			int waitsec = 3;
			while (waitsec != 0)
			{
				waitsec--;
				cConsolTool::Reference().StartPrinting();
				std::stringstream msg;
				msg << "Leaving a room failed. Try again...(" << waitsec << ")";
				cConsolTool::Reference().PrintPos(40, 13, msg.str().c_str());
				cConsolTool::Reference().EndPrinting(false);
				Sleep(1000);
			}
			m_menuState = eChatMenuState::e_JoinRoom;
		}
		Sleep(1);
	}
}

void cTCPClient::ClientReceiveTherad()
{
	while (!m_isConnected && !m_isStopped)
	{
		if (!this->ConnectToServer())
			Sleep(10);
	}

	while (!m_isStopped)
	{
		// 100 times per second for chance of message processing
		Sleep(1);


		//Step 4-1: Receiveing data
		if (!this->ReceiveData(m_connectedSocket))
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
		this->PacketProcedure(m_connectedSocket);
	}

	//std::cout << "try FreeConsole" << std::endl;
	//std::set_terminate(std::cin);
	//std::exit(EXIT_SUCCESS);
	//std::cin.set_rdbuf( std::basic_filebuf '0');
	//system("-1");

	//if (!FreeConsole())
	//{
	//	std::cout << "failed FreeConsole" << std::endl;
	//}
	//std::cout << "end of FreeConsole" << std::endl;
}


SOCKET cTCPClient::GetSocketID() { return m_connectedSocket; }
void cTCPClient::SetState(eGameMonopolyState state) { m_gameMonopolyState = state; }
iTCPClient::eGameMonopolyState cTCPClient::GetState() { return m_gameMonopolyState; }
void cTCPClient::PrintPlayerInfo(sProtocolPlayerInfo& info)
{
	if (m_gameMonopolyPacketProcedure->MyInfo().id == info.id)
	{
		std::cout << "\t MyInfo" << std::endl;
	}
	else
	{
		std::cout << "\t CurrentInfo" << std::endl;
	}
	{
		std::cout << "\t\t ID: " << info.id << "\t Nick: " << info.nick.name << "\t MyTurn: " << (int)info.isMyTurn << "\t Location: " << info.location << std::endl;
		std::cout << "\t\t Money: " << info.money << "(" << info.moneyVariation << ")" << "\t Chance: " << (int)info.chanceToThrowDice << std::endl;
	}
}

void cTCPClient::PlayMonopolySendThread()
{
	std::cout << "cTCPClient::PlayMonopolySendThread()" << std::endl;

	//*****************************************
	// TODO: require a timer for animation
	double deltaTime = 1.0/60;

	float durationOfDiceAnimationDuration = 1.0f;
	float diceAnimation = 0.0f;

	float myPieceLocation = 0.0f;
	float otherPieceLocation = 0.0f;
	float pieceNextLocation = 0.0f;
	float moveSpeed = 2.0f;


	//*****************************************



	while (!m_isStopped)
	{
		switch(m_gameMonopolyState)
		{
		case eGameMonopolyState::e_GM_Wait:
		{
			std::cout << "\t eGameMonopolyState::e_GM_Wait" << std::endl;

			//char anyKey = _getch();

			while (m_gameMonopolyState == eGameMonopolyState::e_GM_Wait)
			{
				// wait 1/20 sec
				Sleep(50);
			}

			break;
		}
		case eGameMonopolyState::e_GM_Start:
		{
			std::cout << "\t eGameMonopolyState::e_GM_Start" << std::endl;

			this->PrintPlayerInfo(m_gameMonopolyPacketProcedure->CurrentPlayerInfo());

			if (m_gameMonopolyPacketProcedure->MyInfo().isMyTurn)
			{
				std::cout << "[ Press any key to throw dice ]" << std::endl;
				char anyKey = _getch();

				// request throw dice
				{
					m_gameMonopolyPacketProcedure->SetHeader(sProtocolMonopolyHeader::e_RequestPlayThrowDice);
					sProtocolRequestPlayThrowDice data;
					m_gameMonopolyPacketProcedure->AppendProtocol(data);

					m_gameMonopolyPacketProcedure->SendData(m_connectedSocket);
				}
			}

			m_gameMonopolyState = eGameMonopolyState::e_GM_Wait;
			break;
		}
		case eGameMonopolyState::e_GM_ThrowDice:
		{
			std::cout << "\t eGameMonopolyState::e_GM_ThrowDice" << std::endl;

			this->PrintPlayerInfo(m_gameMonopolyPacketProcedure->CurrentPlayerInfo());

			diceAnimation = 0.0f;
			m_gameMonopolyState = eGameMonopolyState::e_GM_AnimationThrowDice;

			break;
		}
		case eGameMonopolyState::e_GM_AnimationThrowDice:
		{
			while (diceAnimation < durationOfDiceAnimationDuration)
			{
				diceAnimation += (float)deltaTime;
			}

			if (m_gameMonopolyPacketProcedure->IsMyTurn())
			{
				std::cout << "[ My Current Square: " << m_gameMonopolyPacketProcedure->CurrentPlayerInfo().location << " ]" << std::endl;
			}
			else
			{
				std::cout << "[ Oppenent's Current Square: " << m_gameMonopolyPacketProcedure->CurrentPlayerInfo().location << " ]" << std::endl;
			}

			m_gameMonopolyState = eGameMonopolyState::e_GM_MovePiece;

			break;
		}
		case eGameMonopolyState::e_GM_MovePiece:
		{
			std::cout << "\t eGameMonopolyState::e_GM_MovePiece" << std::endl;

			m_gameMonopolyState = eGameMonopolyState::e_GM_AnimationMovePiece;

			break;
		}
		case eGameMonopolyState::e_GM_AnimationMovePiece:
		{

			float pieceLocation = 0;
			if (m_gameMonopolyPacketProcedure->IsMyTurn())
			{
				pieceLocation = myPieceLocation;
			}
			else
			{
				pieceLocation = otherPieceLocation;
			}

			pieceNextLocation = m_gameMonopolyPacketProcedure->CurrentPlayerInfo().location;
			while (pieceLocation < pieceNextLocation)
			{
				pieceLocation += (float)(deltaTime * moveSpeed);
			}

			// request play action
			if (m_gameMonopolyPacketProcedure->IsMyTurn())
			{
				m_gameMonopolyPacketProcedure->SetHeader(sProtocolMonopolyHeader::e_RequestPlayAction);
				sProtocolRequestPlayAction data;
				m_gameMonopolyPacketProcedure->AppendProtocol(data);

				m_gameMonopolyPacketProcedure->SendData(m_connectedSocket);
			}
				
			m_gameMonopolyState = eGameMonopolyState::e_GM_Wait;

			break;
		}
		case eGameMonopolyState::e_GM_ActionDistrictStart:
		{
			std::cout << "\t eGameMonopolyState::e_GM_ActionDistrictStart" << std::endl;

			std::cout << "[My Money: " << m_gameMonopolyPacketProcedure->MyInfo().money << " ]" << std::endl;
			std::cout << "[Opponent Money: " << m_gameMonopolyPacketProcedure->OpponentInfo().money << " ]" << std::endl;

			m_gameMonopolyState = eGameMonopolyState::e_GM_Wait;
			break;
		}
		case eGameMonopolyState::e_GM_ActionDistrictTax:
		{
			std::cout << "\t eGameMonopolyState::e_GM_ActionDistrictTax" << std::endl;

			std::cout << "[My Money: " << m_gameMonopolyPacketProcedure->MyInfo().money << " ]" << std::endl;
			std::cout << "[Opponent Money: " << m_gameMonopolyPacketProcedure->OpponentInfo().money << " ]" << std::endl;

			m_gameMonopolyState = eGameMonopolyState::e_GM_Wait;
			break;
		}
		case eGameMonopolyState::e_GM_ActionDistrictCard:
		{
			std::cout << "\t eGameMonopolyState::e_GM_ActionDistrictCard" << std::endl;

			//char anyKey = _getch();

			m_gameMonopolyState = eGameMonopolyState::e_GM_Wait;
			break;
		}
		case eGameMonopolyState::e_GM_ActionDistrictFreeParking:
		{
			std::cout << "\t eGameMonopolyState::e_GM_ActionDistrictFreeParking" << std::endl;

			//char anyKey = _getch();

			m_gameMonopolyState = eGameMonopolyState::e_GM_Wait;
			break;
		}
		case eGameMonopolyState::e_GM_ActionDistrictGotoJail:
		{
			std::cout << "\t eGameMonopolyState::e_GM_ActionDistrictGotoJail" << std::endl;

			//char anyKey = _getch();

			m_gameMonopolyState = eGameMonopolyState::e_GM_Wait;
			break;
		}
		case eGameMonopolyState::e_GM_ActionDistrictJail:
		{
			std::cout << "\t eGameMonopolyState::e_GM_ActionDistrictJail" << std::endl;

			//char anyKey = _getch();


			m_gameMonopolyState = eGameMonopolyState::e_GM_Wait;
			break;
		}
		case eGameMonopolyState::e_GM_ActionDistrictBuilding:
		{
			std::cout << "\t eGameMonopolyState::e_GM_ActionDistrictBuilding" << std::endl;

			std::cout << "[My Money: " << m_gameMonopolyPacketProcedure->MyInfo().money << " ]" << std::endl;
			std::cout << "[Opponent Money: " << m_gameMonopolyPacketProcedure->OpponentInfo().money << " ]" << std::endl;

			m_gameMonopolyState = eGameMonopolyState::e_GM_Wait;
			break;
		}
		case eGameMonopolyState::e_GM_ActionDistrictStation:
		{
			std::cout << "\t eGameMonopolyState::e_GM_ActionDistrictStation" << std::endl;

			std::cout << "[My Money: " << m_gameMonopolyPacketProcedure->MyInfo().money << " ]" << std::endl;
			std::cout << "[Opponent Money: " << m_gameMonopolyPacketProcedure->OpponentInfo().money << " ]" << std::endl;


			m_gameMonopolyState = eGameMonopolyState::e_GM_Wait;
			break;
		}
		case eGameMonopolyState::e_GM_ActionDistrictUtility:
		{
			std::cout << "\t eGameMonopolyState::e_GM_ActionDistrictUtility" << std::endl;

			std::cout << "[My Money: " << m_gameMonopolyPacketProcedure->MyInfo().money << " ]" << std::endl;
			std::cout << "[Opponent Money: " << m_gameMonopolyPacketProcedure->OpponentInfo().money << " ]" << std::endl;

			m_gameMonopolyState = eGameMonopolyState::e_GM_Wait;
			break;
		}
		case eGameMonopolyState::e_GM_AskDistrictBuilding:
		{
			std::cout << "\t eGameMonopolyState::e_GM_AskDistrictBuilding" << std::endl;
			std::cout << "[ Would you buy this Building? (Y or N) ]" << std::endl;


			char anyKey = _getch();

			// request asking
			{
				m_gameMonopolyPacketProcedure->SetHeader(sProtocolMonopolyHeader::e_AnswerAssetAction);
				sProtocolAnswerAssetAction data;
				data.yesOrNo = 1;
				//if(anyKey==89 || anyKey==121)
				//	data.yesOrNo = 1;
				if (anyKey == 78 || anyKey == 110)
					data.yesOrNo = 0;

				m_gameMonopolyPacketProcedure->AppendProtocol(data);

				m_gameMonopolyPacketProcedure->SendData(m_connectedSocket);
			}

			m_gameMonopolyState = eGameMonopolyState::e_GM_Wait;
			break;
		}
		case eGameMonopolyState::e_GM_AskDistrictStation:
		{
			std::cout << "\t eGameMonopolyState::e_GM_AskDistrictStation" << std::endl;
			std::cout << "[ Would you buy this Station? (Y or N) ]" << std::endl;

			char anyKey = _getch();

			// request asking
			{
				m_gameMonopolyPacketProcedure->SetHeader(sProtocolMonopolyHeader::e_AnswerAssetAction);
				sProtocolAnswerAssetAction data;
				data.yesOrNo = 1;
				//if(anyKey==89 || anyKey==121)
				//	data.yesOrNo = 1;
				if (anyKey == 78 || anyKey == 110)
					data.yesOrNo = 0;
				m_gameMonopolyPacketProcedure->AppendProtocol(data);

				m_gameMonopolyPacketProcedure->SendData(m_connectedSocket);
			}

			m_gameMonopolyState = eGameMonopolyState::e_GM_Wait;
			break;
		}
		case eGameMonopolyState::e_GM_AskDistrictUtility:
		{
			std::cout << "\t eGameMonopolyState::e_GM_AskDistrictUtility" << std::endl;

			std::cout << "[ Would you buy this Utility? (Y or N) ]" << std::endl;
			char anyKey = _getch();

			// request asking
			{
				m_gameMonopolyPacketProcedure->SetHeader(sProtocolMonopolyHeader::e_AnswerAssetAction);
				sProtocolAnswerAssetAction data;
				data.yesOrNo = 1;
				//if(anyKey==89 || anyKey==121)
				//	data.yesOrNo = 1;
				if (anyKey == 78 || anyKey == 110)
					data.yesOrNo = 0;
				m_gameMonopolyPacketProcedure->AppendProtocol(data);

				m_gameMonopolyPacketProcedure->SendData(m_connectedSocket);
			}

			m_gameMonopolyState = eGameMonopolyState::e_GM_Wait;
			break;
		}
		case eGameMonopolyState::e_GM_TurnChange:
		{
			std::cout << "\t eGameMonopolyState::e_GM_TurnChange" << std::endl;

			//this->PrintPlayerInfo(m_gameMonopolyPacketProcedure->CurrentPlayerInfo());

			m_gameMonopolyState = eGameMonopolyState::e_GM_Start;

			break;
		}
		case eGameMonopolyState::e_GM_TurnKeep:
		{
			std::cout << "\t eGameMonopolyState::e_GM_TurnKeep" << std::endl;

			//this->PrintPlayerInfo(m_gameMonopolyPacketProcedure->CurrentPlayerInfo());
			std::cout << "[ *\\^^/* Chance *\\^^/* ]" << std::endl;
			m_gameMonopolyState = eGameMonopolyState::e_GM_Start;

			break;
		}
		case eGameMonopolyState::e_GM_Finish:
		{
			std::cout << "\t eGameMonopolyState::e_GM_Finish" << std::endl;
			m_gameMonopolyState = eGameMonopolyState::e_GM_Wait;

			if (m_gameMonopolyPacketProcedure->CurrentPlayerInfo().id == m_gameMonopolyPacketProcedure->MyInfo().id)
			{
				std::cout << "[ *\\^^/* WINNER *\\^^/* ]" << std::endl;
			}
			else
			{
				std::cout << "[ >.< LOSER >.< ]" << std::endl;
			}

			Sleep(3000);

			m_gameStarted = false;

			this->SendLeaveRoom();
			while (m_menuState == eChatMenuState::e_PlayGame)
			{
				Sleep(10);
			}
			//m_menuState = eChatMenuState::e_MainMenu;

			break;
		}
		default:
		{
			std::cout << "\t Unknown packet" << std::endl;
			break;
		}
		}


		Sleep(1);
	}
}

std::thread cTCPClient::GetChatSenderThread()
{
	// chating thread
	return std::thread([this] { UserSendThread(); });
}
std::thread cTCPClient::GetPlayMonopolySenderThread()
{
	// monopoly thread
	return std::thread([this] { PlayMonopolySendThread(); });
}
std::thread cTCPClient::GetReceiverThread()
{
	return std::thread([this] { ClientReceiveTherad(); });
}






bool cTCPClient::RunClient()
{
	m_gameMonopolyPacketProcedure = new cClientPacketProcedureMonopoly(*this);

	system("cls");

	std::thread recvthread = this->GetReceiverThread();
	std::thread sendthread = this->GetChatSenderThread();
	std::thread gamethread = this->GetPlayMonopolySenderThread();

	recvthread.join();
	//sendthread.join();

	std::cout << "try to detach thread" << std::endl;
	sendthread.detach();
	gamethread.detach();

	// sender thread
	//senderThread.join();
	std::cout << "detached thread" << std::endl;

	delete m_gameMonopolyPacketProcedure;
	m_gameMonopolyPacketProcedure = 0;

	return true;
}


//Step 5-2: Cleanup
bool cTCPClient::ShutDown()
{
	std::cout << "Step 5: Disconnect Server" << std::endl;

	m_isStopped = true;

	// shutdown the send half of the connection since no more data will be sent
	// TODO: when server socket is shutdown, all the times, error occur. client sockets are not tested yet
	int iResult = shutdown(m_connectedSocket, SD_BOTH);//SD_SEND/SD_RECEIVE/SD_BOTH
	if (iResult == SOCKET_ERROR)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("shutdown failed: ");
		return false;
	}


	if (!Disconnect(m_connectedSocket))
	{
		return false;
	}

	//Final clean up
	WSACleanup();

	return true;
}

//Step 1: Initial winsock
bool cTCPClient::Initialize()
{
	std::cout << "Step 1: Initalize WinSock" << std::endl;

	WSADATA m_windowsSocketApplicationData; //Windows Socket Application Data Object

	//Create a Windows Socket Application Data Object
	int resultInt = WSAStartup(MAKEWORD(2, 2), &m_windowsSocketApplicationData);
	if (resultInt != 0)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("WinSock Initalization failed:");
		return false;
	}

	return true;
}

bool cTCPClient::GetAddressInfo(addrinfo*& outAddressInfo)
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

	int resultInt = getaddrinfo(mc_DEFAULT_IPADDRESS, mc_DEFAULT_PORT, &hints, &outAddressInfo);
	if (resultInt != 0)
	{
		if (m_isDebug)
			cErrorReport::GetInstance().PrintWSAGetLastMessage("Socket Initalization failed:");
		return false;
	}

	if (m_isDebug)
		std::cout << "Step 1: WinSock Initalized" << std::endl;
	return true;
}

//Step 2: Create a Socket
bool cTCPClient::CreateSocket(const addrinfo& addressInfo, SOCKET& newSocket)
{
	//initialize a new socket for a specific protocol

	if (m_isDebug)
	{
		std::cout << "\tsocket(ai_family=" << addressInfo.ai_family << "(PF_XXX)";
		std::cout << ", ai_socktype=" << addressInfo.ai_socktype << "(SOCK_XXX)";
		std::cout << ", ai_protocol=" << addressInfo.ai_protocol << "(IPPROTO_XXX)";
		std::cout << std::endl;
	}

	newSocket = socket(addressInfo.ai_family, addressInfo.ai_socktype, addressInfo.ai_protocol);

	if (newSocket == INVALID_SOCKET)
	{
		if (m_isDebug)
			cErrorReport::GetInstance().PrintWSAGetLastMessage("Socket Initalization failed:");
		return false;
	}



	return true;
}

// Attempt to connect to an address
bool cTCPClient::ConnectToServer()
{

	// This will be used for the Step2 and Step3
	addrinfo *addressInfo = 0;
	if (!this->GetAddressInfo(addressInfo))
	{
		this->ShutDown();
		return false;
	}

	//Step 2: Create a Socket

	// Attempt to connect to an address until one succeeds
	if (m_isDebug)
		std::cout << "Step 2 & 3: Create a Socket & Connect to server" << std::endl;
	struct addrinfo *ptr = NULL;
	for (ptr = addressInfo; ptr != NULL; ptr = ptr->ai_next)
	{
		if (!this->CreateSocket(*addressInfo, m_connectedSocket))
		{
			//this->ShutDown();
			m_connectedSocket = INVALID_SOCKET;
			break;
		}


		// Connect to server.
		int result = connect(m_connectedSocket, addressInfo->ai_addr, (int)addressInfo->ai_addrlen);

		if (result == SOCKET_ERROR)
		{
			//closesocket(connectedSocket);
			m_connectedSocket = INVALID_SOCKET;
			break;
		}
	}

	freeaddrinfo(addressInfo); //Once bind is called the address info is no longer needed so free the memory allocated

	if (m_connectedSocket == INVALID_SOCKET)
	{
		if (m_isDebug)
			cErrorReport::GetInstance().PrintWSAGetLastMessage("Unable to connect to server!: ");
		//this->ShutDown();
		return false;
	}

	if (m_isDebug)
		std::cout << "Step 2 & 3: Socket created & Server connected" << std::endl;

	m_isConnected = true;
	m_menuState = eChatMenuState::e_StartMenu;

	return true;
}


//Step 4-1: Receiveing data
bool cTCPClient::ReceiveData(SOCKET& connectedSocket)
{
	if (!m_isConnected)
		return false;

	if(m_isDebug)
		std::cout << "\tReceive data" << std::endl;


	char recvbuf[cBuffer::mc_DEFAULT_BUFFER_LENGTH];

	//Receive the message
	int result = recv(connectedSocket, recvbuf, sizeof recvbuf, 0);
	if (result == SOCKET_ERROR)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("\trecv failed: ");
		this->Disconnect(m_connectedSocket);
		return false;
	}
	else if (result == 0)
	{
		//cErrorReport::GetInstance().PrintWSAGetLastMessage("\tConnection closing...");
		return false;
	}

	m_receiveBuffer.loadBuffer(recvbuf, result);// cBuffer::mc_DEFAULT_BUFFER_LENGTH);

	if (m_isDebug)
		std::cout << "\tBytes received: " << result << " buffer size: " << m_receiveBuffer.getLength() << std::endl;

	//std::cout << "\tnumber of chars: " << m_receiveBuffer.readUInt16BE() << std::endl;
	//std::cout << "\tdata: " << m_receiveBuffer.toCharArray() << std::endl;

	//m_isSentData = false;

	return true;
}

// Message framing and determin what task performed by packet header
bool cTCPClient::PacketProcedure(SOCKET& connectedSocket)
{
	// message framing
	if (m_receiveBuffer.isEmpty())
		return true;

	while (m_receiveBuffer.getLength() > 0)
	{
		sProtocolHeader header;
		// check the complition of packet_length value
		if (sizeof(header.packet_length) > m_receiveBuffer.getLength())
		{
			if (m_isDebug)
				std::cout << "WARNING: if (sizeof(header.packet_length) > m_receiveBuffer.getLength())" << std::endl;
			return false;
		}

		// check the total packet length
		header.packet_length = m_receiveBuffer.readInt32BE(0);
		if (header.packet_length > (int)(m_receiveBuffer.getLength() + sizeof(header.packet_length)))
		{
			if (m_isDebug)
				std::cout << "WARNING: if (header.packet_length > (int)(m_receiveBuffer.getLength() + sizeof(header.packet_length)))" << std::endl;
			return false;
		}


		m_receiveBuffer.Deserialize(header);

		switch (header.packet_id)
		{
		case sProtocolHeader::ePacketID::e_PlayMonopoly:
		{

			std::cout << "e_PlayMonopoly" << std::endl;

			m_gameMonopolyPacketProcedure->ProcessReceiveData(m_receiveBuffer);

			break;
		}
			case sProtocolHeader::ePacketID::e_ResponseSigninSuccess:
			{
				if (m_isDebug)
					std::cout << "e_ResponseSigninSuccess" << std::endl;

				m_menuState = eChatMenuState::e_SigninSuccess;

				break;
			}
			case sProtocolHeader::ePacketID::e_ResponseSigninFailure:
			{
				if (m_isDebug)
					std::cout << "e_ResponseSigninFailure" << std::endl;

				m_receiveBuffer.Deserialize(m_recvFailure);
				if (m_isDebug)
					std::cout << m_recvFailure.errormessage << std::endl;

				m_menuState = eChatMenuState::e_SigninFailure;

				break;
			}
			case sProtocolHeader::ePacketID::e_ResponseLoginSuccess:
			{
				if (m_isDebug)
					std::cout << "e_ResponseLoginSuccess" << std::endl;

				m_menuState = eChatMenuState::e_LoginSuccess;

				break;
			}
			case sProtocolHeader::ePacketID::e_ResponseLoginFailure:
			{
				if (m_isDebug)
					std::cout << "e_ResponseLoginFailure" << std::endl;

				m_receiveBuffer.Deserialize(m_recvFailure);
				if (m_isDebug)
					std::cout << m_recvFailure.errormessage << std::endl;

				m_menuState = eChatMenuState::e_LoginFailure;

				break;
			}
			case sProtocolHeader::ePacketID::e_ResponseLogoutSuccess:
			{
				if (m_isDebug)
					std::cout << "e_ResponseLogoutSuccess" << std::endl;

				m_menuState = eChatMenuState::e_StartMenu;
				break;
			}
			case sProtocolHeader::ePacketID::e_ResponseNewRoomSuccess:
			{
				if (m_isDebug)
					std::cout << "e_ResponseNewRoomSuccess" << std::endl;

				// change menu state
				m_menuState = eChatMenuState::e_CreateRoomSuccess;

				break;
			}
			case sProtocolHeader::ePacketID::e_ResponseNewRoomFailure:
			{
				if (m_isDebug)
					std::cout << "e_ResponseNewRoomFailure" << std::endl;

				m_receiveBuffer.Deserialize(m_recvFailure);
				if (m_isDebug)
					std::cout << m_recvFailure.errormessage << std::endl;

				m_menuState = eChatMenuState::e_CreateRoomFailure;

				break;
			}
			case sProtocolHeader::ePacketID::e_ResponseJoinRoomSuccess:
			{
				if (m_isDebug)
					std::cout << "e_ResponseJoinRoomSuccess" << std::endl;

				// change menu state
				m_menuState = eChatMenuState::e_JoinRoomSuccess;

				break;
			}
			case sProtocolHeader::ePacketID::e_ResponseJoinRoomFailure:
			{
				if (m_isDebug)
					std::cout << "e_ResponseJoinRoomFailure" << std::endl;

				m_receiveBuffer.Deserialize(m_recvFailure);
				if (m_isDebug)
					std::cout << m_recvFailure.errormessage << std::endl;

				m_menuState = eChatMenuState::e_JoinRoomFailure;

				break;
			}
			case sProtocolHeader::ePacketID::e_ResponseLeaveRoomSuccess:
			{
				if (m_isDebug)
					std::cout << "e_ResponseLeaveRoomSuccess" << std::endl;

				// change menu state
				m_menuState = eChatMenuState::e_LeaveRoomSuccess;
				break;
			}
			case sProtocolHeader::ePacketID::e_ResponseLeaveRoomFailure:
			{
				if (m_isDebug)
					std::cout << "e_ResponseLeaveRoomFailure" << std::endl;

				m_receiveBuffer.Deserialize(m_recvFailure);
				if (m_isDebug)
					std::cout << m_recvFailure.errormessage << std::endl;

				// change menu state
				m_menuState = eChatMenuState::e_LeaveRoomFailure;

				break;
			}
			case sProtocolHeader::ePacketID::e_ResponseLobbyUsers:
			{
				if (m_isDebug)
					std::cout << "e_ResponseLobbyUsers" << std::endl;

				m_lobbyUsers.users.clear();
				m_receiveBuffer.Deserialize(m_lobbyUsers);
				this->PrintLobbyUsers();

				//m_receivedLobbyUsers = true;
				break;
			}
			case sProtocolHeader::ePacketID::e_ResponseRoomList:
			{
				if (m_isDebug)
					std::cout << "e_ResponseRoomList" << std::endl;

				// room list
				m_roomList.rooms.clear();
				m_receiveBuffer.Deserialize(m_roomList);
				this->PrintRoomList();

				//m_receivedLoomList = true;
				break;
			}
			case sProtocolHeader::ePacketID::e_ResponseRoomInfo:
			{
				if (m_isDebug)
					std::cout << "e_ResponseRoomInfo" << std::endl;

				// room info
				m_currentRoomInfo.users.clear();
				m_receiveBuffer.Deserialize(m_currentRoomInfo);
				this->PrintCurrentRoomUsers();

				//m_receivedRoomInfo = true;
				break;
			}
			case sProtocolHeader::ePacketID::e_ResponseChatHistory:
			{
				if (m_isDebug)
					std::cout << "e_ResponseChatHistory" << std::endl;

				// chat history
				sProtocolResponseChatHistory chatHistory;
				m_receiveBuffer.Deserialize(chatHistory);

				for (int i = 0; i < chatHistory.numberOfMessages; ++i)
				{
					m_chatHistory.chatHistory.push_back(chatHistory.chatHistory[i]);
				}

				m_chatHistory.numberOfMessages = m_chatHistory.chatHistory.size();
				this->PrintChatHistory();
				break;
			}
			case sProtocolHeader::ePacketID::e_SendChatMessageFailure:
			{
				if (m_isDebug)
					std::cout << "e_SendChatMessageFailure" << std::endl;

				m_receiveBuffer.Deserialize(m_recvFailure);
				if (m_isDebug)
					std::cout << m_recvFailure.errormessage << std::endl;

				sProtocolChatMessage failmessage;
				failmessage.message = "[FAILED TO CHAT. TRY AGAIN]";
				failmessage.message_length = (int)(failmessage.message.size());
				m_chatHistory.chatHistory.push_back(failmessage);

				m_chatHistory.numberOfMessages = m_chatHistory.chatHistory.size();
				this->PrintChatHistory();
				break;
			}
			case sProtocolHeader::e_ResponseDisconnect:
			{
				if (m_isDebug)
					std::cout << "e_ResponseDisconnect" << std::endl;
				m_isStopped = true;
				m_menuState = eChatMenuState::e_Connect;
				break;
			}
			default:
			{
				std::cout << "ERROR: unknown packet" << std::endl;
				return false;
			}
		}

		if (m_receiveBuffer.getLength() > 0)
		{
			if (m_isDebug)
				std::cout << "remain buffer size: " << m_receiveBuffer.getLength() << std::endl;
		}
	}

	return true;
}


//Step 4-2: sending data
bool cTCPClient::SendData(SOCKET& connectedSocket, cBuffer& buffer)
{
	if (!m_isConnected)
		return false;

	if (m_isDebug)
		std::cout << "bool cTCPClient::SendData(const SOCKET connectedSocket, cBuffer& buffer)" << std::endl;

	int result = send(connectedSocket, buffer.toCharArray(), m_sendBuffer.getLength(), 0);
	if (result == SOCKET_ERROR)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("\tSend failed: ");
		this->Disconnect(connectedSocket);
		return false;
	}

	Sleep(1);
	//std::cout << "\tBytes sent: " << result << std::endl;

	// complete to send data
	buffer.clear();

	//m_isSentData = true;

	return true;
}


//Step 5-1: Disconnect
bool cTCPClient::Disconnect(SOCKET& connectedSocket)
{
	std::cout << "\tDisconnect socket: " << connectedSocket << std::endl;

	m_menuState = eChatMenuState::e_Connect;
	closesocket(connectedSocket);
	connectedSocket = INVALID_SOCKET;

	std::cout << "\tSocket disconnected" << std::endl;
	return true;
}


// when console close event occur, it make sure the clean up every thing before console shutdown
BOOL WINAPI cTCPClient::ConsoleHandler(DWORD CEvent)
{

	switch (CEvent)
	{
		case CTRL_C_EVENT:
		case CTRL_BREAK_EVENT:
			//system("pause");
			return TRUE;

	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_CLOSE_EVENT:
		cTCPClient::GetInstance().RequestDisconnect();
		while (cTCPClient::m_instance!=0)
		{
			Sleep(1);
		}
		// Detach Console
		//FreeConsole();
		// Prevent closing
		//std::cout << "exit ConsoleHandler" << std::endl;
		//ExitThread(0);
		return TRUE;
	default:
		break;
	}

	return FALSE;
}