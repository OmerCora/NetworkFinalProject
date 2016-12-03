#include "cLightSource.h"


#include "externals.h"
#include "managers.h"
#include "GelsenizeCore.h"

cLightSource::cLightSource()
{
	this->objectID = gelObjectManager::sharedInstance().GetNewGameObjectID();

	this->GetLight();
	this->GetPhysics();
	this->GetMesh();
	this->GetRender();

	this->lightInfo.bIsEnabled = true;
	this->lightInfo.attenConst = 0.0f;
	this->lightInfo.attenLinear = 0.141427f;
	this->lightInfo.attenQuad = 0.0027f;
	this->lightInfo.diffuse = glm::vec3(1.0f, 0.2f, 0.0f);
	this->lightInfo.ambient= glm::vec3(0.1f, 0.1f, 0.1f);
	this->lightInfo.specular = glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f);
	this->lightInfo.position = glm::vec3(0.0f, 0.0f, 0.0f);
}

cLightSource::~cLightSource()
{
}

LightInfo cLightSource::GetLight(void)
{
	return this->lightInfo;
}

MeshInfo cLightSource::GetMesh(void)
{
	return this->meshInfo;
}

RenderInfo cLightSource::GetRender(void)
{
	return this->renderInfo;
}


PhysicsInfo cLightSource::GetPhysics(void)
{
	return this->physicsInfo;
}

CollisionInfo cLightSource::GetCollision(void)
{
	return this->collisionInfo;
}

void cLightSource::SetCollision(CollisionInfo info)
{
	this->collisionInfo = info;
}

void cLightSource::SetLight(LightInfo info)
{
	this->lightInfo = info;
}

void cLightSource::SetMesh(MeshInfo info)
{
	this->meshInfo = info;
}
void cLightSource::SetRender(RenderInfo info)
{
	this->renderInfo = info;
}
void cLightSource::SetPhysics(PhysicsInfo info)
{
	this->physicsInfo = info;
}

int cLightSource::getObjectID()
{
	return this->objectID;
}

void cLightSource::Update(float deltaTime)
{
	this->lightInfo.diffuse = glm::vec3(this->renderInfo.color.x, this->renderInfo.color.y, this->renderInfo.color.z);
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

std::string cLightSource::ObjectType(void)
{
	return "lightSource";
}

