/*
* Program Name : INFO6017_AI_Project4 - cEnemy.cpp
* Purpose      : INFO-6017	Artificial Intelligence Poject3
* Description  : cEnemy class implementation
* Date         : December 8, 2016
* Coder        : Jaehuhn Park
*/

#include "GlobalStuff.h"

cEnemy::cEnemy()
{
	this->mRenderingInfo.Color = glm::vec3(0.0f, 1.0f, 0.0f);
	this->mPhysicsInfo.fRotationSpeed = 0.6;

}

cEnemy::~cEnemy()
{

}

void cEnemy::Update(float dt)
{
//	glm::vec3 playerToEnemy = this->mPhysicsInfo.Position - gPlayer->mPhysicsInfo.Position;
//	glm::vec3 playerForward = gPlayer->mPhysicsInfo.Orientation*glm::vec3(0.f, 0.f, 1.0f);
//	playerToEnemy = glm::normalize(playerToEnemy);
//	glm::vec3 myForward = this->mPhysicsInfo.Orientation*glm::vec3(0.f, 0.f, 1.0f);
//	glm::vec3 rotationAxis = glm::cross(myForward, playerToEnemy);
//
//	if (mbForwardable)
//		this->mPhysicsInfo.Position += myForward* 2.0f *dt;
//
//	//float dot = glm::dot(playerToEnemy, playerForward);
//	float YAxisValue = rotationAxis.y > 0.f ? 1.f : -1.f;
//
//if (!mbForwardable)
//	{
//		YAxisValue *= -1.f;
//	}
//
//	rotationAxis = glm::vec3(0.f, YAxisValue, 0.f);
//
//	float rotationVelocity = 1.f;
//	// this matrix is like "from" concept
//	this->mPhysicsInfo.Orientation = glm::rotate(this->mPhysicsInfo.Orientation, rotationVelocity*dt, rotationAxis);
//
//

} // end of cEnemy::Update




