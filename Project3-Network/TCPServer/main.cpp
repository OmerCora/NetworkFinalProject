
#include <iostream>

#include "cTCPServer.h"
#include "cTCPAuthenticationClient.h"

//#include "cSha256.h"

//#include "AuthenticationDef.pb.h"

#include "cUser.h"
#include "cLogicMonoPoly.h"


int main()
{
	// TODO: this is test code for logic implementation
	{
		sUserInfo info;
		cUser* userA = new cUser(info);
		cUser* userB = new cUser(info);
		cLogicMonoPoly* logic = new cLogicMonoPoly();
		logic->PlayGame(userA, userB);
		delete userA;
		delete userB;
		delete logic;
	}

#if 0
	if (!cTCPAuthenticationClient::GetInstance().StartClient())
		exit(EXIT_FAILURE);

	// initialize server
	if(!cTCPServer::GetInstance().StartServer())
		exit(EXIT_FAILURE);

	// message loop
	if(!cTCPServer::GetInstance().RunServer())
		exit(EXIT_FAILURE);

	// clean up server
	if(!cTCPServer::GetInstance().ShutDown())
		exit(EXIT_FAILURE);

	// clean up server
	if (!cTCPAuthenticationClient::GetInstance().EndClient())
		exit(EXIT_FAILURE);


	// clean up
	cTCPServer::DeleteInstance();
	cTCPAuthenticationClient::DeleteInstance();

	//Keep the window open
	std::cout << "\nwaiting on exit";
	//system("pause");
#endif

	exit(EXIT_SUCCESS);
}