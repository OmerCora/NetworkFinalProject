#pragma once

#include <windows.h>
#include <string>

class cConsolTool
{
	cConsolTool();

public:
	~cConsolTool();
	static cConsolTool& Reference();


	// disaplay user interactive information
	bool InputNumber(int& outNumber);
	bool InputName(const std::string& title, unsigned int maximum_chars, std::string& outString, short CurX, short CurY, char exceptCh = 0);
	bool InputPassword(const std::string& title, unsigned int maximum_chars, std::string& outString, short CurX, short CurY);


	void StartPrinting();
	void EndPrinting(bool isInput);

	void SetCursorPos(short x, short y);

	void DrawRectangle(short x, short y, short width, short height);
	void PrintPos(short x, short y, const char* txt);

	bool m_isDebug;
private:
	bool m_isPrinting;
	COORD m_inputCursorPos;
};

