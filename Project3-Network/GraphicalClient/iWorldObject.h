#ifndef _IWORLDOBJECT_HG_
#define _IWORLDOBJECT_HG_

#include <string>
#include "externals.h"

class iWorldObject
{
public:
	~iWorldObject() {}
	virtual void ReceiveMessage(std::string msg, float data) = 0;
	virtual bool AskQuestion(std::string msg, float &data) = 0;

	virtual glm::vec3 GetPosition(void) = 0;
	virtual std::string GetObjectType(void) = 0;

	virtual void Tick(float deltaTime) = 0;
private:

};

#endif // !_IWORLDOBJECT_HG_

