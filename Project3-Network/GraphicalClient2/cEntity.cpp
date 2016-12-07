/*
* Program Name : INFO6017_AI_Project4 - cEntity.cpp
* Purpose      : INFO-6017	Artificial Intelligence Poject3
* Description  : cEntity class implementation
* Date         : December 8, 2016
* Coder        : Jaehuhn Park
*/

#include "cEntity.h"
#include "GlobalStuff.h"
#include <iostream>



cEntity::cEntity()
{
	mbForwardable = false;
	collisonDetectPoint = 0.0f;
}

void cEntity::GetRenderingInfo(sRenderingInfo & renderingInfo)
{
	renderingInfo = this->mRenderingInfo;
}

void cEntity::GetPhysicsInfo(sPhysicsInfo & physicsInfo)
{
	physicsInfo = this->mPhysicsInfo;
}

void cEntity::Update(float dt)
{
}

//will be called inside the DrawScene() for each entity
void cEntity::preRenderer()
{
	this->mRenderingInfo.modelMatrix = glm::translate(glm::mat4(1.0f), this->mPhysicsInfo.Position);
	this->mRenderingInfo.modelMatrix = this->mRenderingInfo.modelMatrix * glm::mat4_cast(this->mPhysicsInfo.Orientation);
	//this->mRenderingInfo.modelMatrix = glm::scale(this->mRenderingInfo.modelMatrix, this->mRenderingInfo.Scale);

}

const glm::vec3 & cEntity::GetForward()
{
	return this->mPhysicsInfo.Orientation * glm::vec3(0.f, 0.f, 1.f);
}

//const glm::vec3 cEntity::EstimatedFuturePosition(float dt)
//{
//	return this->mPhysicsInfo.Position + this->mPhysicsInfo.fSpeed*GetForward()*dt;
//}






