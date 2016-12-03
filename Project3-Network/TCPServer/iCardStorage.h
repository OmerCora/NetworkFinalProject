#pragma once

class iCardStorage
{
public:
	virtual ~iCardStorage() {}

	virtual int pickOneCard() = 0;
};

