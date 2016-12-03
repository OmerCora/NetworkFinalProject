#include "cAnimal.h"

#include "externals.h"
#include "managers.h"
#include "GelsenizeCore.h"

#include "gelWorldManager.h"
#include "gelObjectBuilder.h"

#include <queue>

cAnimal::cAnimal()
{
	this->objectID = gelObjectManager::sharedInstance().GetNewGameObjectID();

	this->GetLight();
	this->GetPhysics();
	this->GetMesh();
	this->GetRender();
	interpolationTime = -1.0f;
	this->renderInfo.useDebugColorsNoLighting = false;
}

cAnimal::~cAnimal()
{
}

void cAnimal::Update(float deltaTime)
{
	if (interpolationTime > 0.0f)
	{
		//shortcut to instant move right now;
		if (interpolationTime <= deltaTime)
		{
			//move instantly
			this->renderInfo.position.x = desiredLocation.x;
			this->renderInfo.position.y = desiredLocation.y;
			this->renderInfo.position.z = desiredLocation.z;
			interpolationTime = 0.0f; //stop moving
			acc_interpolationTime = 0.0f;
		}
		else
		{
			//incoming delta percentage
			acc_interpolationTime += deltaTime;
			float deltaPerc = (acc_interpolationTime * 100.0f) / interpolationTime;
			if (deltaPerc > 98.0f)
			{
				//move instantly
				this->renderInfo.position.x = desiredLocation.x;
				this->renderInfo.position.y = desiredLocation.y;
				this->renderInfo.position.z = desiredLocation.z;
				interpolationTime = 0.0f; //stop moving
				acc_interpolationTime = 0.0f;
			}
			else
			{
				//move the animal towards destination
				//get the full vector
				glm::vec3 moveVec = desiredLocation - initialLocation;
				this->renderInfo.position.x = initialLocation.x + moveVec.x * deltaPerc / 100.0f;
				this->renderInfo.position.y = initialLocation.y + moveVec.y * deltaPerc / 100.0f;
				this->renderInfo.position.z = initialLocation.z + moveVec.z * deltaPerc / 100.0f;
			}
		}
	}
	else
	{

	}
}

void cAnimal::Tick(float deltaTime)
{
	if (this->faunaInfo.dead)
	{
		return;
	}

	// 1- animals lose calories 5 percent
	this->faunaInfo.currentCalories -= this->faunaInfo.initialCalories * 0.008f; // actual wanted value *0.005 makes it last too long

	float hpPercentage = (this->faunaInfo.currentCalories * 100.0f) / this->faunaInfo.initialCalories;

	if (this->faunaInfo.currentCalories < 0.0f)
	{
		//die
		this->faunaInfo.dead = true;
		this->renderInfo.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (hpPercentage < 50.0f)
	{
		//hp below 50%

		std::priority_queue<FaunaInfo::FoodPreference> foodQueue;

		for (int i = 0; i != faunaInfo.foodPreference.size(); i++)
		{
			foodQueue.push(faunaInfo.foodPreference[i]);
		}
		glm::vec3 foodPosition;

		bool foundFood = false;

		while (!foodQueue.empty())
		{
			foundFood = gelWorldManager::sharedInstance().FindNearestPriorityFood(this->objectID, foodQueue.top().name, foodPosition);
			foodQueue.pop();
			if (foundFood)
			{
				//found food start moving to it
				desiredLocation = foodPosition;
				initialLocation = glm::vec3(renderInfo.position.x, renderInfo.position.y, renderInfo.position.z);
				interpolationTime = deltaTime * 0.9f; //shorter than actual tick time
				acc_interpolationTime = 0.0f;
				break;
			}
		}
	}
	else
	{
		//healthy
	}

	//reproduce
	faunaInfo.acc_reproduce += deltaTime;
	if (faunaInfo.reproductionDuration <= faunaInfo.acc_reproduce)
	{
		//create a new one at my position
		//implemented on next question

		//reproduce
		faunaInfo.acc_reproduce = 0.0f;

		iGameObject *newbornAnimal = gelObjectBuilder::sharedInstance().buildWorldObject(this->faunaInfo.type);
		RenderInfo render = newbornAnimal->GetRender();

		render.position.x = this->renderInfo.position.x;
		render.position.z = this->renderInfo.position.y;
		render.position.y = this->renderInfo.position.z;
		newbornAnimal->SetRender(render);

		gelObjectManager::sharedInstance().AddWorldObject(newbornAnimal);

	}

	//for reference
	// 2- for animals below 50%, 
	//		find food-> 
	//		move to food-> 
	//		eat food-> 
	//		absorb calories->
	//		also remove eaten plants&animals->
	//      


	// 3- Animals can reproduce. deliver tick to all world objects
}

void cAnimal::AssignFaunaInfo(FaunaInfo info)
{
	this->faunaInfo.type = info.type;
	this->faunaInfo.foodPreference = info.foodPreference;
	this->faunaInfo.initialCalories = info.initialCalories;
	this->faunaInfo.currentCalories = info.currentCalories;
	this->faunaInfo.reproductionDuration = info.reproductionDuration;
}

//std::string cAnimal::GetObjectType(void)
//{
//	return "animal";
//}
//
//std::string cAnimal::GetClassType(void)
//{
//	return this->faunaInfo.type;
//}

LightInfo cAnimal::GetLight(void)
{
	return this->lightInfo;
}

MeshInfo cAnimal::GetMesh(void)
{
	return this->meshInfo;
}

RenderInfo cAnimal::GetRender(void)
{
	return this->renderInfo;
}


PhysicsInfo cAnimal::GetPhysics(void)
{
	return this->physicsInfo;
}

CollisionInfo cAnimal::GetCollision(void)
{
	return this->collisionInfo;
}

void cAnimal::SetCollision(CollisionInfo info)
{
	this->collisionInfo = info;
}

void cAnimal::SetLight(LightInfo info)
{
	this->lightInfo = info;
}

void cAnimal::SetMesh(MeshInfo info)
{
	this->meshInfo = info;
}
void cAnimal::SetRender(RenderInfo info)
{
	this->renderInfo = info;
}
void cAnimal::SetPhysics(PhysicsInfo info)
{
	this->physicsInfo = info;
}

int cAnimal::getObjectID()
{
	return this->objectID;
}

std::string cAnimal::ObjectType(void)
{
	return "animal";
}

std::string cAnimal::GetObjectType(void)
{
	return this->faunaInfo.type;
}

glm::vec3 cAnimal::GetPosition(void)
{
	return glm::vec3(this->renderInfo.position.x, this->renderInfo.position.y, this->renderInfo.position.z);
}

void cAnimal::ReceiveMessage(std::string msg, float data)
{
	if (msg == "consume")
	{
		//data is calories
		this->faunaInfo.currentCalories += data;
	}
	else if ("eaten")
	{
		// "remove" object
		this->faunaInfo.dead = true;
		this->renderInfo.isDrawn = false;
	}
}

bool cAnimal::AskQuestion(std::string msg, float &data)
{
	if (msg == "calories")
	{
		if (this->faunaInfo.dead)
		{
			data = this->faunaInfo.initialCalories * 0.05f;
		}
		else
		{
			data = this->faunaInfo.initialCalories;
		}
		return true;
	}
	else if (msg == "edible")
	{
		return true;
	}
	else if (msg == "alive")
	{
		return !this->faunaInfo.dead;
	}
	return false;
}
