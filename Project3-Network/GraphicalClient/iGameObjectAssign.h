#ifndef _IGAMEOBJECTASSIGN_HG_
#define _IGAMEOBJECTASSIGN_HG_

#include <string>

class iGameObjectAssign
{
public:
	virtual ~iGameObjectAssign() {}

	virtual void AssignMoodAndSpeed(std::string moon, float avrgSpd) = 0;
private:

};

#endif // !_IGAMEOBJECTASSIGN_HG_
