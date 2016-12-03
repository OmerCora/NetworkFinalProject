
#include "cTCPClient.h"

#include <iostream>

int main()
{
	cTCPClient* client = new cTCPClient();

	// initialize server
	if (!client->StartClient())
		exit(EXIT_FAILURE);

	// message loop
	if (!client->RunClient())
		exit(EXIT_FAILURE);

	// clean up server
	if (!client->ShutDown())
		exit(EXIT_FAILURE);


	//cTCPClient::DeleteInstance();

	delete client;

	//Keep the window open
	std::cout << "\nwaiting on exit";
	//system("pause");

	exit(EXIT_SUCCESS);
}