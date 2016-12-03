#include "cGameObject.h"

#include "externals.h"
#include "managers.h"
#include "GelsenizeCore.h"

LightInfo cGameObject::GetLight(void)
{
	return this->lightInfo;
}

MeshInfo cGameObject::GetMesh(void)
{
	return this->meshInfo;
}

RenderInfo cGameObject::GetRender(void)
{
	return this->renderInfo;
}


PhysicsInfo cGameObject::GetPhysics(void)
{
	return this->physicsInfo;
}

CollisionInfo cGameObject::GetCollision(void)
{
	return this->collisionInfo;
}

void cGameObject::SetCollision(CollisionInfo info)
{
	this->collisionInfo = info;
}

void cGameObject::SetLight(LightInfo info)
{
	this->lightInfo = info;
}

void cGameObject::SetMesh(MeshInfo info)
{
	this->meshInfo = info;
}
void cGameObject::SetRender(RenderInfo info)
{
	this->renderInfo = info;
}
void cGameObject::SetPhysics(PhysicsInfo info)
{
	this->physicsInfo = info;
}

int cGameObject::getObjectID()
{
	return this->objectID;
}

void cGameObject::Update(float deltaTime)
{
	//check sound
	//if (this->meshInfo.soundCooldown > 0.0f)
	//{
	//	this->meshInfo.soundCooldown_delta -= deltaTime;
	//	if (this->meshInfo.soundCooldown_delta <= 0.0f)
	//	{
	//		//play sound
	//		gelSoundManager::sharedInstance().PlayObjectSound(this);
	//		this->meshInfo.soundCooldown_delta = this->meshInfo.soundCooldown;
	//	}
	//}
}

std::string cGameObject::ObjectType(void)
{
	return "object";
}

cGameObject::cGameObject()
{
	this->objectID = gelObjectManager::sharedInstance().GetNewGameObjectID();

	this->GetLight();
	this->GetPhysics();
	this->GetMesh();
	this->GetRender();

	this->renderInfo.useDebugColorsNoLighting = false;
}

cGameObject::~cGameObject()
{
}
