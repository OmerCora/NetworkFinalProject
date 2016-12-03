#include "cConsolTool.h"

#include <conio.h>
#include <iostream>
#include <set>


struct sPreventionChars
{
	//const char HTAB = 11;
	//const char VTAB = 11;
	const char SPACE = 32;
	//const char DBLQUART = 34;
	//const char SNGQUART = 39;
	//const char COMMA = 44;

	//const char SEMICOLON = 59;
	//const char LESSTH = 60;
	//const char EQUAL = 61;
	//const char GRATERTH = 62;
	//const char QUESTION = 63;

	const char BKSHASH = 92;
	const char GRAVACC = 96;
	//const char BRACOP = 123;
	//const char VERTBAR = 124;
	//const char BRACCL = 125;

	//const char INV = 126;
	const char DEL = 127;

	static sPreventionChars preventionChars;
	static std::set<char> preventionCharsVec;
	static void Init()
	{
		//preventionCharsVec.reserve(17);

		for (int i = 0; i < 5; ++i)
		{
			const char c = ((const char*)&preventionChars)[i];
			preventionCharsVec.insert(c);
		}
	}
	static bool ContainChar(char ch, char exceptCh = 0)
	{
		return (ch != exceptCh && (ch < preventionChars.SPACE || ch >= preventionChars.DEL));
		//return (ch != exceptCh && preventionCharsVec.find(ch) != preventionCharsVec.end());
	}
};
sPreventionChars sPreventionChars::preventionChars;
std::set<char> sPreventionChars::preventionCharsVec;

cConsolTool::cConsolTool()
	: m_isPrinting(false)
	, m_inputCursorPos({ 0,0 })
{
	sPreventionChars::Init();
}


cConsolTool::~cConsolTool()
{
}

cConsolTool& cConsolTool::Reference()
{
	static cConsolTool instance;
	return instance;
}

void cConsolTool::SetCursorPos(short x, short y)
{
	if (m_isDebug)
		return;
	COORD cursorPos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);
}
void cConsolTool::StartPrinting()
{
	while (m_isPrinting)
	{
		Sleep(5);
	}
	m_isPrinting = true;
}
void cConsolTool::EndPrinting(bool isInput)
{
	if (isInput)
	{
		CONSOLE_SCREEN_BUFFER_INFO SBInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &SBInfo);
		m_inputCursorPos = SBInfo.dwCursorPosition;
	}
	else
	{
		SetCursorPos(m_inputCursorPos.X, m_inputCursorPos.Y);
	}

	m_isPrinting = false;
}

void cConsolTool::DrawRectangle(short x, short y, short width, short height)
{
	if (m_isDebug)
		return;
	static const char* lineHT = "/-\\";
	static const char* lineV = "| |";
	static const char* lineHB = "\\-/";

	SetCursorPos(x, y);
	printf("%c", lineHT[0]);
	for (int w = 0; w < width; ++w)
	{
		printf("%c", lineHT[1]);
	}
	printf("%c", lineHT[2]);

	for (int h = 1; h < height - 1; ++h)
	{
		SetCursorPos(x, y + h);
		printf("%c", lineV[0]);
		for (int w = 0; w < width; ++w)
		{
			printf("%c", lineV[1]);
		}
		printf("%c", lineV[2]);
	}

	SetCursorPos(x, y + height - 1);
	printf("%c", lineHB[0]);
	for (int w = 0; w < width; ++w)
	{
		printf("%c", lineHB[1]);
	}
	printf("%c", lineHB[2]);
}


void cConsolTool::PrintPos(short x, short y, const char* txt)
{
	if (!m_isDebug)
		SetCursorPos(x, y);
	std::cout << txt;
}
bool cConsolTool::InputNumber(int& outNumber)
{
	const char ESCAPE = 27;
	char c = _getch();
	if (c == ESCAPE)
		return false;
	outNumber = c - 48;// '0' = ascii 48
	return true;
}


bool cConsolTool::InputName(const std::string& title, unsigned int maximum_chars, std::string& outString, short CurX, short CurY, char exceptCh/*=0*/)
{
	this->StartPrinting();
	this->SetCursorPos(CurX, CurY);
	std::cout << "[" << title << ":  ] ";
	this->SetCursorPos((short)(CurX + title.size() + 3), CurY);
	this->EndPrinting(true);

	char ch;
	const char ESCAPE = 27;
	const char ENTER = 13;
	const char BACKSPACE = 8;
	outString.clear();
	while (ch = _getch())
	{
		if (ch == ESCAPE)
		{
			return false;
		}
		if (ch == ENTER)
			break;
		if (ch == BACKSPACE)
		{
			if (outString.size()>0)
				outString.pop_back();
		}
		else if (sPreventionChars::ContainChar(ch, exceptCh))
		{
			continue;
		}
		else if (outString.size() < maximum_chars)
		{
			outString += ch;
		}

		//this->PrintPos(40, 13, "[User e-mail: ");
		this->StartPrinting();
		this->SetCursorPos(CurX, CurY);
		std::cout << "[" << title << ": " << outString << " ] ";
		this->SetCursorPos((short)(CurX + title.size() + 3 + outString.size()), CurY);
		this->EndPrinting(true);
	}

	return true;
}

bool cConsolTool::InputPassword(const std::string& title, unsigned int maximum_chars, std::string& outString, short CurX, short CurY)
{
	this->StartPrinting();
	this->SetCursorPos(CurX, CurY);
	std::cout << "[" << title << ":  ] ";
	this->SetCursorPos((short)(CurX + title.size() + 3), CurY);
	this->EndPrinting(true);

	char ch;
	const char ESCAPE = 27;
	const char ENTER = 13;
	const char BACKSPACE = 8;
	outString.clear();
	std::string securestring;
	while (ch = _getch())
	{
		if (ch == ESCAPE)
		{
			return false;
		}
		if (ch == ENTER)
			break;
		if (ch == BACKSPACE)
		{
			if (outString.size() > 0)
			{
				outString.pop_back();
				securestring.pop_back();
			}
		}
		else if (ch < BACKSPACE || sPreventionChars::ContainChar(ch))
		{
			continue;
		}
		else if (outString.size() < maximum_chars)
		{
			outString += ch;
			securestring += "*";
		}

		//this->PrintPos(40, 13, "[User e-mail: ");
		this->StartPrinting();
		this->SetCursorPos(CurX, CurY);
		std::cout << "[" << title << ": " << securestring << " ] ";
		this->SetCursorPos((short)(CurX + title.size() + 3 + securestring.size()), CurY);
		this->EndPrinting(true);
	}

	return true;
}