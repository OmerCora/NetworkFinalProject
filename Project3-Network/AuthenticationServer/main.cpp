#include <stdio.h>
#include <iostream>
#include <string>


// need to link corenetwork.lib
#pragma comment (lib, "CoreNetwork.lib")

#include "cDBAccessor.h"
#include "cTCPAuthenticationServer.h"


int main()
{
#if 1
	cDBAccessor db;

	if (db.Open("Authentication.db"))
	{
		const char* datas[] = {
			"test1",
			"test2",
			"test3",
			"test4",
			"test5",
		};
		for(int i=0;i<5;++i)
		{
			if (!db.UserExist(datas[i]))
			{
				ProtocolAuthentication::CreateAccountWeb signinaccount;
				signinaccount.set_requestid(0);
				signinaccount.set_email(datas[i]);
				signinaccount.set_plaintextpassword("password");
				if (db.Signin(signinaccount))
				{
					std::cout << "sign-in success" << std::endl;
				}
				else
				{
					std::cout << "sign-in failure" << std::endl;
				}
			}
		}
		//ProtocolAuthentication::AuthenticateWeb loginaccount;
		//loginaccount.set_requestid(0);
		//loginaccount.set_email("test@test");
		//loginaccount.set_plaintextpassword("password");
		//if (db.Login(loginaccount))
		//{
		//	std::cout << "log-in success" << std::endl;
		//}
		//else
		//{
		//	std::cout << "log-in failure" << std::endl;
		//}

		db.ShowAllTables();

		db.Close();
	}

	//exit(EXIT_SUCCESS);
#endif

	// initialize server
	if (!cTCPAuthenticationServer::GetInstance().ExecuteServer())
		exit(EXIT_FAILURE);

	// clean up
	cTCPAuthenticationServer::DeleteInstance();

	//Keep the window open
	std::cout << "\nwaiting on exit";
	//system("pause");


	exit(EXIT_SUCCESS);

}