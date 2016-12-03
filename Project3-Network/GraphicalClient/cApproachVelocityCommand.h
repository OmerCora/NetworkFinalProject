#ifndef _CAPPROACHVELOCITYCOMMAND_HG_
#define _CAPPROACHVELOCITYCOMMAND_HG_

#include "iCommand.h"

class cApproachVelocityCommand : public iCommand
{
public:
	void SetGameObject(iGameObject* newGameObject);
	// Move the object to this point at this speed
	virtual void Init(rapidjson::Value& initJSON);
	virtual bool Execute(float deltaTime);		// Called over and over again
	virtual bool IsDone(void);
private:
	glm::vec3 direction;
	glm::vec3 velocity;
	float speed;
	float duration;
	float durationAcc;

	iGameObject* gameObject;
};


#endif
