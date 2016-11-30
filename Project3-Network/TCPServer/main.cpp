
#include <iostream>

#include "cTCPServer.h"
#include "cTCPAuthenticationClient.h"

//#include "cSha256.h"

//#include "AuthenticationDef.pb.h"

int main()
{
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


	exit(EXIT_SUCCESS);
}