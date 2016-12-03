#ifndef _CMoveCommand_HG_
#define _CMoveCommand_HG_

#include "iCommand.h"

class cMoveCommand : public iCommand
{
public:
	void SetGameObject(iGameObject* newGameObject);
	// Move the object to this point at this speed
	virtual void Init(rapidjson::Value& initJSON);
	virtual bool Execute(float deltaTime);		// Called over and over again
	virtual bool IsDone(void);
private:
	glm::vec3 destination;
	glm::vec3 velocity;
	float speed;
	float duration;
	float durationAcc;
	float easeInDuration; //const
	float easeInDurationAcc;
	float easeOutDuration; //const
	float easeOutDurationAcc;

	iGameObject* gameObject;
};


#endif
