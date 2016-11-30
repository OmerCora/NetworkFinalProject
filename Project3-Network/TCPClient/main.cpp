
#include "cTCPClient.h"

#include <iostream>

int main()
{
	// initialize server
	if (!cTCPClient::GetInstance().StartClient())
		exit(EXIT_FAILURE);

	// message loop
	if (!cTCPClient::GetInstance().RunClient())
		exit(EXIT_FAILURE);

	// clean up server
	if (!cTCPClient::GetInstance().ShutDown())
		exit(EXIT_FAILURE);


	cTCPClient::DeleteInstance();

	//Keep the window open
	std::cout << "\nwaiting on exit";
	//system("pause");

	exit(EXIT_SUCCESS);
}