#include "cPlant.h"

#include "externals.h"
#include "managers.h"
#include "GelsenizeCore.h"

#include "OCHelper.h"

cPlant::cPlant()
{
	this->objectID = gelObjectManager::sharedInstance().GetNewGameObjectID();

	this->GetLight();
	this->GetPhysics();
	this->GetMesh();
	this->GetRender();

	this->renderInfo.useDebugColorsNoLighting = false;
}

cPlant::~cPlant()
{
}

void cPlant::Update(float deltaTime)
{
	if (this->floraInfo.hasFruit)
	{
		this->renderInfo.color = this->floraInfo.fruitColor;
	}
	else
	{
		this->renderInfo.color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
	}
}

void cPlant::Tick(float deltaTime)
{
	if (this->floraInfo.hasFruit)
	{
		//has fruit

		//rotting phase
		floraInfo.acc_spoil += deltaTime;
		if (floraInfo.spoilDuration <= floraInfo.acc_spoil)
		{
			floraInfo.hasFruit = false;
			//fruit gone, reset accumulator
			floraInfo.acc_spoil = 0.0f;
		}
	}
	else
	{
		//no fruit, keep growing
		floraInfo.acc_spawn += deltaTime;
		if (floraInfo.spawnDuration <= floraInfo.acc_spawn)
		{
			//fruit grown
			floraInfo.hasFruit = true;
			floraInfo.acc_spoil = 0.0f;
			floraInfo.acc_spawn = 0.0f; //reset the acc_spawn
			floraInfo.generateRandomSpawnDuration(); //new cooldown ready up
		}
	}
}

void cPlant::AssignFloraInfo(FloraInfo info)
{
	this->floraInfo.hasFruit = false;
	this->floraInfo.type = info.type;
	this->floraInfo.calories = info.calories;
	this->floraInfo.minSpawnDuration = info.minSpawnDuration;
	this->floraInfo.maxSpawnDuration = info.maxSpawnDuration;
	this->floraInfo.spoilDuration = info.spoilDuration;
	this->floraInfo.generateRandomSpawnDuration();
	this->floraInfo.fruitColor = info.fruitColor;
}

glm::vec3 cPlant::GetPosition(void)
{
	return glm::vec3(this->renderInfo.position.x, this->renderInfo.position.y, this->renderInfo.position.z);
}

std::string cPlant::ObjectType(void)
{
	return "plant";
}

std::string cPlant::GetObjectType(void)
{
	return this->floraInfo.type;
}

void cPlant::ReceiveMessage(std::string msg, float data)
{
	if ("eaten")
	{
		this->floraInfo.hasFruit = false;
		this->floraInfo.acc_spawn = 0.0f;
		this->floraInfo.acc_spoil = 0.0f;
		this->floraInfo.generateRandomSpawnDuration();
	}
}

bool cPlant::AskQuestion(std::string msg, float &data)
{
	if (msg == "edible")
	{
		return this->floraInfo.hasFruit;
	}
	else if (msg == "calories")
	{
		data = this->floraInfo.calories;
		return true;
	}
	else if (msg == "alive")
	{
		return true; //always alive
	}
	return false;
}


//std::string cPlant::GetClassType(void)
//{
//	return this->floraInfo.type;
//}

LightInfo cPlant::GetLight(void)
{
	return this->lightInfo;
}

MeshInfo cPlant::GetMesh(void)
{
	return this->meshInfo;
}

RenderInfo cPlant::GetRender(void)
{
	return this->renderInfo;
}


PhysicsInfo cPlant::GetPhysics(void)
{
	return this->physicsInfo;
}

CollisionInfo cPlant::GetCollision(void)
{
	return this->collisionInfo;
}

void cPlant::SetCollision(CollisionInfo info)
{
	this->collisionInfo = info;
}

void cPlant::SetLight(LightInfo info)
{
	this->lightInfo = info;
}

void cPlant::SetMesh(MeshInfo info)
{
	this->meshInfo = info;
}
void cPlant::SetRender(RenderInfo info)
{
	this->renderInfo = info;
}
void cPlant::SetPhysics(PhysicsInfo info)
{
	this->physicsInfo = info;
}

int cPlant::getObjectID()
{
	return this->objectID;
}
