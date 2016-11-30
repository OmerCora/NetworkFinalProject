#ifndef _cDBAccessor_HG_
#define _cDBAccessor_HG_

//Authentication Service Database(5 marks)
//1. Create a table `web_auth` (2 marks)
//	1) id BIGINT AUTO_INCREMENT ?
//	2) email VARCHAR(255) ?
//	3) salt CHAR(64) ?
//	4) hashed_password CHAR(64) ?
//	5) userId BIGINT ?
//2. Create a table `user` (2 marks)
//	1) id BIGINT AUTO_INCREMENT ?
//	2) last_login TIMESTAMP ?
//	3) creation_date DATETIME ?

#include <string>
#include <../src/sqlite3.h>
#include "AuthenticationDef.pb.h"

class cDBAccessor
{
public:
	cDBAccessor();
	~cDBAccessor();

	bool IsOpen();
	bool Open(const std::string& dbfilename);
	bool Close();

	bool UserExist(const std::string& email);
	bool Signin(ProtocolAuthentication::CreateAccountWeb& account);
	bool Login(ProtocolAuthentication::AuthenticateWeb& account);

	bool ShowAllTables();

private:
	bool CreateWebAuthTable();
	bool CreateUserTableTable();

	struct sAuthInfo
	{
		long long id;
		std::string salt;
		std::string hashed_pw;
		long long user_id;
	};
	struct sUserInfo
	{
		long long id;
		std::string lastloginTime;
		std::string creationDate;
	};
	bool GetAuth(const std::string& email, sAuthInfo& outInfo);
	bool GetLastUser(sUserInfo& outInfo);
	static int SelectAuthCallback(void *data, int argc, char **argv, char **azColName);
	bool GetTableNames(std::vector<std::string>& outTables);

	sqlite3 *m_authenticationDB;
};

#endif