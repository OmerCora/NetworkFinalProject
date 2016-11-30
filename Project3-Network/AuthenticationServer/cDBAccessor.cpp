#include "cDBAccessor.h"
#include <cSha256.h>

#include <iostream>
#include <sstream>

cDBAccessor::cDBAccessor()
	:m_authenticationDB(0)
{
}

cDBAccessor::~cDBAccessor()
{
}

bool cDBAccessor::IsOpen()
{
	return (m_authenticationDB != 0);
}

bool cDBAccessor::Open(const std::string& dbfilename)
{
	//Attempt to open the database
	int returnCode = sqlite3_open(dbfilename.c_str(), &m_authenticationDB);
	//Test to see if the database opened correctly
	if (returnCode)
	{
		std::cout << "Can't open database: " << sqlite3_errmsg(m_authenticationDB) << std::endl;
		return false;
	}
	else
	{
		std::cout << "Database opened!" << std::endl;
	}

	if (!CreateWebAuthTable())
	{
		this->Close();
		return false;
	}
	if (!CreateUserTableTable())
	{
		this->Close();
		return false;
	}

	return true;
}
bool cDBAccessor::Close()
{
	//Close database
	sqlite3_close(m_authenticationDB);

	return true;
}


bool cDBAccessor::GetTableNames(std::vector<std::string>& outTables)
{
	std::string sql = "SELECT name FROM sqlite_master WHERE type='table';";
	sqlite3_stmt* selectstmt = 0;
	sqlite3_prepare(m_authenticationDB, sql.c_str(), sql.size() + 1, &selectstmt, NULL);

	outTables.reserve(1024);
	while (true)
	{
		int s;
		s = sqlite3_step(selectstmt);
		if (s == SQLITE_ROW)
		{
			std::string result = (const char*)sqlite3_column_text(selectstmt, 0);
			std::cout << "\ttable		: " << result << std::endl;

			outTables.push_back(result);
		}
		else if (s == SQLITE_DONE)
		{
			break;
		}
		else
		{
			std::cout << stderr << std::endl;// , "Failed.\n");
			return false;
		}
	}

	sqlite3_finalize(selectstmt);

	return true;
}

bool cDBAccessor::ShowAllTables()
{
	std::vector<std::string> tableNames;
	this->GetTableNames(tableNames);

	for(unsigned int i=0;i<tableNames.size();++i)
	{
		std::string sql = "SELECT * FROM "+ tableNames[i] + ";";

		const char* title = "ShowAllTables";
		char *errorMessage = 0;
		// Execute SQL statement
		int returnCode = sqlite3_exec(m_authenticationDB, sql.c_str(), SelectAuthCallback, (void*)title, &errorMessage);
		if (returnCode != SQLITE_OK)
		{
			std::cout << "SQL error: " << errorMessage << std::endl;
			sqlite3_free(errorMessage);
			return false;
		}
		else
		{
			std::cout << "Table created succesfully!" << std::endl;
		}
	}
	return true;
}

bool cDBAccessor::CreateWebAuthTable()
{
	//create table if not exists TableName(col1 typ1, ..., colN typN)
	//drop table if exists TableName

	//from http://sqlite.org/autoinc.html
	//The keyword AUTOINCREMENT can be used with INTEGER field only.
	//1 The AUTOINCREMENT keyword imposes extra CPU, memory, disk space, and disk I/O overhead and should be avoided if not strictly needed. It is usually not needed.
	//2 In SQLite, a column with type INTEGER PRIMARY KEY is an alias for the ROWID(except in WITHOUT ROWID tables) which is always a 64 - bit signed integer.
	//3	On an INSERT, if the ROWID or INTEGER PRIMARY KEY column is not explicitly given a value, then it will be filled automatically with an unused integer, usually one more than the largest ROWID currently in use.This is true regardless of whether or not the AUTOINCREMENT keyword is used.
	//4	If the AUTOINCREMENT keyword appears after INTEGER PRIMARY KEY, that changes the automatic ROWID assignment algorithm to prevent the reuse of ROWIDs over the lifetime of the database.In other words, the purpose of AUTOINCREMENT is to prevent the reuse of ROWIDs from previously deleted rows.

	std::string sql = "CREATE TABLE if not exists web_auth("  \
		"id INTEGER PRIMARY KEY AUTOINCREMENT," \
		"email VARCHAR(255) NOT NULL," \
		"salt CHAR(64) NOT NULL,"\
		"hashed_pw CHAR(64) NOT NULL,"\
		"userID BIGINT NOT NULL);";

	const char* title = "CreateWebAuthTable";
	char *errorMessage = 0;
	// Execute SQL statement
	int returnCode = sqlite3_exec(m_authenticationDB, sql.c_str(), SelectAuthCallback, (void*)title, &errorMessage);
	if (returnCode != SQLITE_OK)
	{
		std::cout << "SQL error: " << errorMessage << std::endl;
		sqlite3_free(errorMessage);
		return false;
	}
	else
	{
		std::cout << "Table created succesfully!" << std::endl;
	}

	return true;
}
bool cDBAccessor::CreateUserTableTable()
{
	//create table if not exists TableName(col1 typ1, ..., colN typN)
	//drop table if exists TableName
	//CURRENT_TIME – Inserts only time
	//CURRENT_DATE – Inserts only date
	//CURRENT_TIMESTAMP – Inserts both time and date
	//CREATE TABLE users(
	//	id INTEGER PRIMARY KEY,
	//	username TEXT,
	//	created_at DATETIME DEFAULT CURRENT_TIMESTAMP
	//);

	std::string sql = "CREATE TABLE if not exists user("  \
		"id INTEGER PRIMARY KEY AUTOINCREMENT," \
		"last_login DATETIME," \
		"creation_date DATETIME default CURRENT_TIMESTAMP);";

	const char* title = "CreateUserTableTable";
	char *errorMessage = 0;
	// Execute SQL statement
	int returnCode = sqlite3_exec(m_authenticationDB, sql.c_str(), SelectAuthCallback, (void*)title, &errorMessage);
	if (returnCode != SQLITE_OK)
	{
		std::cout << "SQL error: " << errorMessage << std::endl;
		sqlite3_free(errorMessage);
		return false;
	}
	else
	{
		std::cout << "Table created succesfully!" << std::endl;
	}
	return true;
}

bool cDBAccessor::UserExist(const std::string& email)
{
	sAuthInfo accountInfo;
	return this->GetAuth(email, accountInfo);
}
bool cDBAccessor::GetAuth(const std::string& email, sAuthInfo& outInfo)
{
	std::string sql = "SELECT * FROM web_auth WHERE email='" + email + "';";
	sqlite3_stmt* selectstmt = 0;
	sqlite3_prepare(m_authenticationDB, sql.c_str(), sql.size() + 1, &selectstmt, NULL);

	std::vector<sAuthInfo> data;
	while (true)
	{
		int s;
		s = sqlite3_step(selectstmt);
		if (s == SQLITE_ROW)
		{
			sAuthInfo result;
			result.id = sqlite3_column_int64(selectstmt, 0);
			result.salt = (const char*)sqlite3_column_text(selectstmt, 1);
			result.hashed_pw = (const char*)sqlite3_column_text(selectstmt, 2);
			result.user_id = sqlite3_column_int64(selectstmt, 3);
			std::cout << "\tid			: " << result.id << std::endl;
			std::cout << "\tsalt		: " << result.salt << std::endl;
			std::cout << "\thashed_pw	: " << result.hashed_pw << std::endl;
			std::cout << "\tuser_id		: " << result.user_id << std::endl;

			data.push_back(result);
		}
		else if (s == SQLITE_DONE)
		{
			break;
		}
		else
		{
			std::cout << stderr << std::endl;// , "Failed.\n");
			return false;
		}
	}

	sqlite3_finalize(selectstmt);

	if (data.size() != 1)
		return false;

	outInfo = data[0];

	return true;
}
bool cDBAccessor::GetLastUser(sUserInfo& outInfo)
{
	//std::string sql = "SELECT LAST( 'id' ) FROM user;";

	std::string sql = "SELECT * FROM user ORDER BY id DESC LIMIT 1;";
	//std::string sql = "SELECT * FROM user;";
	sqlite3_stmt* selectstmt = 0;
	sqlite3_prepare(m_authenticationDB, sql.c_str(), sql.size() + 1, &selectstmt, NULL);

	//sqlite3_last_insert_rowid(m_authenticationDB);

	std::vector<sUserInfo> data;
	while (true)
	{
		int s;
		s = sqlite3_step(selectstmt);
		if (s == SQLITE_ROW)
		{
			sUserInfo result;
			result.id = sqlite3_column_int64(selectstmt, 0);
			result.lastloginTime = (const char*)sqlite3_column_text(selectstmt, 1);
			result.creationDate = (const char*)sqlite3_column_text(selectstmt, 2);
			std::cout << "\tid				: " << result.id << std::endl;
			std::cout << "\tlastloginTime	: " << result.lastloginTime << std::endl;
			std::cout << "\tcreationDate	: " << result.creationDate << std::endl;

			data.push_back(result);
		}
		else if (s == SQLITE_DONE)
		{
			break;
		}
		else
		{
			std::cout << stderr << std::endl;// , "Failed.\n");
			return false;
		}
	}

	sqlite3_finalize(selectstmt);

	if (data.size() != 1)
		return false;

	outInfo = data[0];

	return true;
}
bool cDBAccessor::Signin(ProtocolAuthentication::CreateAccountWeb& account)
{
	// check already exists
	{
		sAuthInfo authInfo;
		if (this->GetAuth(account.email(), authInfo))
			return false;
	}

	// insert new user into user table
	{
		std::string sql = "INSERT INTO user (last_login,creation_date) VALUES ((DATETIME('now')), (DATETIME('now')) );";

		const char* title = "INSERT INTO user";
		char *errorMessage = 0;
		// Execute SQL statement
		int returnCode = sqlite3_exec(m_authenticationDB, sql.c_str(), SelectAuthCallback, (void*)title, &errorMessage);
		if (returnCode != SQLITE_OK)
		{
			std::cout << "SQL error: " << errorMessage << std::endl;
			sqlite3_free(errorMessage);
			return false;
		}
	}

	// insert new user account into web_auth table
	{
		// get last user info
		sUserInfo userInfo;
		if (!this->GetLastUser(userInfo))
			return false;

		std::string salt = account.email();
		std::string hash = sha256(account.plaintextpassword() + "." + salt);
		//std::string sql = "INSERT INTO web_auth (email,salt,hashed_pw,userID) VALUES ('" + account.email() + "', '" + salt + "', '" + hash + "', '" + userInfo.id + "' ); ";
		std::stringstream sql;
		sql << "INSERT INTO web_auth (email,salt,hashed_pw,userID) VALUES ('" + account.email() + "', '" + salt + "', '" + hash + "', '" << userInfo.id << "' ); ";

		const char* title = "INSERT INTO web_auth";
		char *errorMessage = 0;
		// Execute SQL statement
		int returnCode = sqlite3_exec(m_authenticationDB, sql.str().c_str(), SelectAuthCallback, (void*)title, &errorMessage);
		if (returnCode != SQLITE_OK)
		{
			std::cout << "SQL error: " << errorMessage << std::endl;
			sqlite3_free(errorMessage);
			return false;
		}
	}

	std::cout << "Signin successs!" << std::endl;

	return true;
}
bool cDBAccessor::Login(ProtocolAuthentication::AuthenticateWeb& account)
{
	// get account info
	sAuthInfo authInfo;
	if (!this->GetAuth(account.email(), authInfo))
		return false;

	// update user login time
	std::stringstream sql;
	sql << "UPDATE user set last_login = '" << authInfo.user_id << "'; ";

	const char* title = "UPDATE user";
	char *errorMessage = 0;
	// Execute SQL statement
	int returnCode = sqlite3_exec(m_authenticationDB, sql.str().c_str(), SelectAuthCallback, (void*)title, &errorMessage);
	if (returnCode != SQLITE_OK)
	{
		std::cout << "SQL error: " << errorMessage << std::endl;
		sqlite3_free(errorMessage);
		return false;
	}

	return true;
}

//CREATE TABLE MyTable(
//	ID INTEGER PRIMARY KEY,
//	Name TEXT,
//	Other STUFF,
//	sqltime TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL
//);
//INSERT INTO my_table(name, sqltime) VALUES('test1', '2010-05-28T15:36:56.200');


//This function will be called for every row of data returned in the select statement
int cDBAccessor::SelectAuthCallback(void *data, int argc, char **argv, char **azColName)
{
	if (data == 0)
	{
		std::cout << "Callback" << std::endl;
	}
	else
	{
		std::cout << (char*)data << std::endl;
	}
	std::vector<sAuthInfo>* result = (std::vector<sAuthInfo>*)data;

	for (int i = 0; i < argc; i++)
	{
		std::cout << "\t" << azColName[i] << " = " << (argv[i]==0?"null":argv[i]) << std::endl;
	}

	//just for spacing
	std::cout << std::endl;

	return 0;
}

#if 0
AuthenticationProtocol::CreateAccountWeb* createProtocol = AuthenticationProtocol::CreateAccountWeb().New();
createProtocol->set_requestid(1);
createProtocol->set_email("test");
createProtocol->set_plaintextpassword("password");

delete createProtocol;

//Create a database pointer
sqlite3 *database;
char *errorMessage = 0;
int returnCode;

//Attempt to open the database
returnCode = sqlite3_open("test.db", &database);

//Test to see if the database opened correctly
if (returnCode)
{
	std::cout << "Can't open database: " << sqlite3_errmsg(database) << std::endl;
	return(0);
}
else
{
	std::cout << "Database opened!" << std::endl;
}

//////////////////////
/* Create Statement */
//////////////////////

// Create SQL statement
//char* sql = "CREATE TABLE COMPANY("  \
		//	"ID INT PRIMARY KEY     NOT NULL," \
	//	"NAME           TEXT    NOT NULL," \
	//	"AGE            INT     NOT NULL," \
	//	"ADDRESS        CHAR(50)," \
	//	"SALARY         REAL );";

char* sql = "CREATE TABLE hashes("  \
"ID INT PRIMARY KEY     NOT NULL," \
"SALT        CHAR(64)," \
"HASH        CHAR(64));";
// use AUTOINCREMENT for ID


// Execute SQL statement
returnCode = sqlite3_exec(database, sql, NULL, 0, &errorMessage);
if (returnCode != SQLITE_OK)
{
	std::cout << "SQL error: " << errorMessage << std::endl;
	sqlite3_free(errorMessage);
}
else
{
	std::cout << "Table created succesfully!" << std::endl;
}

//////////////////////
/* Insert Statement */
//////////////////////

struct sSaltHash
{
	std::string id;
	std::string salt;
	std::string hash;
	void UpdateHash(std::string _id, std::string _salt, std::string _password)
	{
		id = _id;
		salt = _salt;
		hash = sha256(salt + _password);
	}
	std::string InsertStatement()
	{
		std::string sql = "INSERT INTO hashes (ID,SALT,HASH) VALUES (" + id + ", '" + salt + "', '" + hash + "' ); ";
		return sql;
	}
	std::string UpdateStatement()
	{
		std::string sql = "UPDATE hashes set SALT = '" + salt + "', HASH = '" + hash + "' where ID=" + id + "; SELECT * from hashes";
		return sql;
	}
};
sSaltHash sqlhashdata[4];
sqlhashdata[0].UpdateHash("1", "Paul", "this.is.a.password");
sqlhashdata[1].UpdateHash("2", "Allen", "this.is.a.password");
sqlhashdata[2].UpdateHash("3", "Teddy", "this.is.a.password");
sqlhashdata[3].UpdateHash("4", "Mark", "this.is.a.password");


// Create SQL insert statement
//sql = "INSERT INTO hashes (ID,SALT,HASH) "  \
		//	"VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
	//	"INSERT INTO hashes (ID,SALT,HASH) "  \
	//	"VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
	//	"INSERT INTO hashes (ID,SALT,HASH)" \
	//	"VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
	//	"INSERT INTO hashes (ID,SALT,HASH)" \
	//	"VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

std::string sqlstatements;
sqlstatements += sqlhashdata[0].InsertStatement();
sqlstatements += sqlhashdata[1].InsertStatement();
sqlstatements += sqlhashdata[2].InsertStatement();
sqlstatements += sqlhashdata[3].InsertStatement();

// Execute SQL statement
returnCode = sqlite3_exec(database, sqlstatements.c_str(), NULL, 0, &errorMessage);
if (returnCode != SQLITE_OK)
{
	std::cout << "SQL error: " << errorMessage << std::endl;
	sqlite3_free(errorMessage);
}
else
{
	std::cout << "INSERT executed succesfully!" << std::endl;
}

//////////////////////
/* Select Statement */
//////////////////////

// Create SQL statement
sql = "SELECT * from hashes";

const char* data = "Callback function called";

// Execute SQL statement
returnCode = sqlite3_exec(database, sql, selectCallback, (void*)data, &errorMessage);
if (returnCode != SQLITE_OK)
{
	std::cout << "SQL error: " << errorMessage << std::endl;
	sqlite3_free(errorMessage);
}
else
{
	std::cout << "SELECT executed succesfully!" << std::endl;
}

//////////////////////
/* Update Statement */
//////////////////////

std::cout << "\n\n\n Attempt UPDATE \n\n\n\n";

sqlhashdata[3].UpdateHash("4", "Ben", "this.is.a.password");
// Create merged SQL statement
//sql = "UPDATE hashes set SALT = 'Ben' where ID=1; " \
		//	"SELECT * from hashes";


// Execute SQL statement
returnCode = sqlite3_exec(database, sqlhashdata[3].UpdateStatement().c_str(), selectCallback, (void*)data, &errorMessage);
if (returnCode != SQLITE_OK)
{
	std::cout << "SQL error: " << errorMessage << std::endl;
	sqlite3_free(errorMessage);
}
else
{
	std::cout << "UPDATE executed succesfully!" << std::endl;
}

//////////////////////
/* Delete Statement */
//////////////////////

std::cout << "\n\n\n Attempt DELETE \n\n\n\n";

// Create merged SQL statement
sql = "DELETE from hashes where ID=2; " \
"SELECT * from hashes";

// Execute SQL statement
returnCode = sqlite3_exec(database, sql, selectCallback, (void*)data, &errorMessage);
if (returnCode != SQLITE_OK)
{
	std::cout << "SQL error: " << errorMessage << std::endl;
	sqlite3_free(errorMessage);
}
else
{
	std::cout << "DELETE executed succesfully!" << std::endl;
}

//Close database
sqlite3_close(database);
std::cout << "Database closed!" << std::endl;

system("pause");

exit(EXIT_SUCCESS);
#endif