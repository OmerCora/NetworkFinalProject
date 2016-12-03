#include "cParticle.h"

#include "cGameObject.h"

#include "externals.h"
#include "managers.h"
#include "GelsenizeCore.h"

//LightInfo cParticle::GetLight(void)
//{
//	return this->lightInfo;
//}
//
//MeshInfo cParticle::GetMesh(void)
//{
//	return this->meshInfo;
//}
//
//RenderInfo cParticle::GetRender(void)
//{
//	return this->renderInfo;
//}
//
//
//PhysicsInfo cParticle::GetPhysics(void)
//{
//	return this->physicsInfo;
//}
//
//void cParticle::SetLight(LightInfo info)
//{
//	this->lightInfo = info;
//}
//
//void cParticle::SetMesh(MeshInfo info)
//{
//	this->meshInfo = info;
//}
//void cParticle::SetRender(RenderInfo info)
//{
//	this->renderInfo = info;
//}
//void cParticle::SetPhysics(PhysicsInfo info)
//{
//	this->physicsInfo = info;
//}
//
//int cParticle::getObjectID()
//{
//	return this->objectID;
//}

void cParticle::Update(float deltaTime)
{
	life -= deltaTime;
}

cParticle::cParticle()
{
	//this->objectID = -1;
	//this->objectID = gelObjectManager::sharedInstance().GetNewGameObjectID();
	this->life = 0.0f;
	this->scale = 1.0f;
	this->discardTextureBlack = false;
	//this->GetLight();
	//this->GetPhysics();
	//this->GetMesh();
	//this->GetRender();

	//this->renderInfo.useDebugColorsNoLighting = false;
}

cParticle::~cParticle()
{
}

//void cParticle::SetLife(float lifePt)
//{
//	this->life = lifePt;
//}
//
//float cParticle::GetLife(void)
//{
//	return this->life;
//}
//
//float cParticle::DecayLife(float deltaTime)
//{
//	life -= deltaTime;
//	return life;
//}
