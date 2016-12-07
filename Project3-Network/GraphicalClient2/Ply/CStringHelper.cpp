#include "CStringHelper.h"

#include <sstream>

CStringHelper::~CStringHelper( )
{
	if (this->m_pTheInstance)
	{
		delete this->m_pTheInstance;
	}
	return;
}

/*static*/
CStringHelper* CStringHelper::m_pTheInstance = NULL;

/*static*/
CStringHelper* CStringHelper::getInstance( void )
{
	if (CStringHelper::m_pTheInstance == NULL)
	{
		CStringHelper::m_pTheInstance = new CStringHelper( );
	}
	return CStringHelper::m_pTheInstance;
}



/*static*/
std::wstring CStringHelper::ASCIIToUnicodeQnD( std::string ASCIIString )
{
	return CStringHelper::m_pTheInstance->m_ASCIIToUnicodeQnD( ASCIIString );
}

std::wstring CStringHelper::m_ASCIIToUnicodeQnD( std::string ASCIIString )
{
	std::wstringstream ss;
	for (std::string::iterator itChar = ASCIIString.begin( ); itChar != ASCIIString.end( ); itChar++)
	{
		char tempChar = *itChar;
		wchar_t tempCharUni = static_cast< wchar_t >(tempChar);
		ss << tempCharUni;
	}
	return ss.str( );
}

/*static*/
std::string CStringHelper::UnicodeToASCII_QnD( std::wstring UnicodeString )
{
	return CStringHelper::getInstance( )->m_UnicodeToASCII_QnD( UnicodeString );
}

std::string CStringHelper::m_UnicodeToASCII_QnD( std::wstring UnicodeString )
{
	std::stringstream ssReturnASCII;
	for (std::wstring::iterator itChar = UnicodeString.begin( ); itChar != UnicodeString.end( ); itChar++)
	{
		char theChar = static_cast<char>(*itChar);
		ssReturnASCII << theChar;
	}
	return ssReturnASCII.str( );
}
