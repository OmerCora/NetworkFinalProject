#ifndef _CStringHelper_HG_
#define _CStringHelper_HG_

#include <string>

class CStringHelper
{
public:
	~CStringHelper( );
	static CStringHelper* getInstance( void );
	// NOTE: There are WAY better (i.e. safer!) ways to do this. See MultiByteToWideChar(), etc. in windows
	// But this is "good enough" for now (and works file converting just the ASCII characters)
	static std::wstring ASCIIToUnicodeQnD( std::string ASCIIString );
	static std::string UnicodeToASCII_QnD( std::wstring UnicodeString );
private:
	CStringHelper( ) {};
	static CStringHelper* m_pTheInstance;

	// NOTE: There are WAY better (i.e. safer!) ways to do this. See MultiByteToWideChar(), etc. in windows
	// But this is "good enough" for now (and works file converting just the ASCII characters)
	std::wstring m_ASCIIToUnicodeQnD( std::string ASCIIString );

	// NOTE: There are WAY better (i.e. safer!) ways to do this. See MultiByteToWideChar(), etc. in windows
	// But this is "good enough" for now (and works file converting just the ASCII characters)
	std::string m_UnicodeToASCII_QnD( std::wstring UnicodeString );
};


#endif 
