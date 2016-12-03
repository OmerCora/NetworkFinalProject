#include "cApproachVelocityCommand.h"

#include <iostream>
#include "externals.h"

//JSON
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>

#include "OCHelper.h"

void cApproachVelocityCommand::SetGameObject(iGameObject * newGameObject)
{
	this->gameObject = newGameObject;
}

void cApproachVelocityCommand::Init(rapidjson::Value &initJSON)
{
	glm::vec3 objectPosition = this->gameObject->GetRender().position;

	if (initJSON.HasMember("direction"))
	{
		this->direction.x = initJSON["direction"]["x"].GetFloat();
		this->direction.y = initJSON["direction"]["y"].GetFloat();
		this->direction.z = initJSON["direction"]["z"].GetFloat();
	}
	else
	{
		this->direction = glm::vec3(0.0f, 0.0f, 1.0f);
	}

	//total speed
	if (initJSON.HasMember("speed"))
	{
		speed = initJSON["speed"].GetFloat();
	}
	else
	{
		speed = 1.0f;
	}

	//aprroach in duration
	if (initJSON.HasMember("duration"))
	{
		duration = initJSON["duration"].GetFloat();
	}
	else
	{
		duration = 1.0f;
	}
	durationAcc = duration;

	//find velocity
	velocity = glm::normalize(direction) * speed;

	return;
}

bool cApproachVelocityCommand::Execute(float deltaTime)	// Called over and over again
{
	float deltaTimeInSeconds = deltaTime;

	PhysicsInfo objectPhysics = this->gameObject->GetPhysics();

	bool reverseApproach = false;
	if (glm::length(objectPhysics.velocity) > speed)
	{
		reverseApproach = true;
	}

	if (durationAcc > 0.0f)
	{
		durationAcc -= deltaTimeInSeconds; //deplete duration

		glm::vec3 desiredVelocity = velocity;

		float velStep;
		if (reverseApproach)
		{
			velStep = 1- OCHelper::smootherStep(0.0f, duration, duration - durationAcc);
		}
		else
		{
			velStep = OCHelper::smootherStep(0.0f, duration, duration - durationAcc);
		}
		

		objectPhysics.velocity = glm::normalize(velocity) * velStep * speed;

		this->gameObject->SetPhysics(objectPhysics);

	}
	else
	{
		glm::vec3 desiredVelocity = velocity;

		objectPhysics.velocity = desiredVelocity;

		this->gameObject->SetPhysics(objectPhysics);
	}
	
	return true;// It's all good.
}

bool cApproachVelocityCommand::IsDone(void)
{
	if (glm::length(this->gameObject->GetPhysics().velocity) == speed)
	{
		//glm::vec3 desiredVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
		//PhysicsInfo objectPhysics = this->gameObject->GetPhysics();
		//objectPhysics.velocity = desiredVelocity;
		//this->gameObject->SetPhysics(objectPhysics);
		return true;
	}
	return false;
}
