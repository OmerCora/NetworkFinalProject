/*
* Program Name : INFO6017_AI_Project4 - cPlayer.cpp
* Purpose      : INFO-6017	Artificial Intelligence Poject3
* Description  : cPlayer class implementation
* Date         : December 8, 2016
* Coder        : Jaehuhn Park
*/

#include "GlobalStuff.h"

cPlayer::cPlayer()
{
	this->mRenderingInfo.Color = glm::vec3(0.0f, 1.0f, 1.0f);
	this->mRenderingInfo.Scale = glm::vec3(0.1f, 0.1f, 0.1f);
	this->mPhysicsInfo.Position = glm::vec3(0.f, 0.f, 0.f);
	//this->mPhysicsInfo.fRotationSpeed = 1.6;
}

cPlayer::~cPlayer()
{

}

void cPlayer::Update(float dt)
{
	//up vector for rotation
	glm::vec3 up(0.f, 1.f, 0.f);

	//key press A -> turn left / key press 'D' -> turn right
	bool pressA = (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS);
	bool pressD = (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS);
	if (pressA != pressD)
	{
		this->mPhysicsInfo.fRotationSpeed = pressA ? 1.8f : -1.8f;

		this->mPhysicsInfo.Orientation = glm::rotate(this->mPhysicsInfo.Orientation, 
													this->mPhysicsInfo.fRotationSpeed*dt, up);

		if (!mbForwardable) mbForwardable = true;
	}

	//key press W ->forward / key press 'S' -> backward
	bool pressW = (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS);
	bool pressS = (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS);

	//glm::vec3 forward(0.f, 0.f, 1.f);
	//glm::vec3 mforward(0.f, 0.f, 1.f);

	if (pressW != pressS)
	{
		this->mPhysicsInfo.fSpeed = pressW ? 10.0f : -10.0f;
		
		//mforward = this->GetForward();
		//forward = mPhysicsInfo.Orientation * forward;
		//this->mPhysicsInfo.Velocity = this->GetForward() * this->mPhysicsInfo.fSpeed;
		this->mPhysicsInfo.Velocity = this->mPhysicsInfo.Orientation * glm::vec3(0.f, 0.f, 1.f) * this->mPhysicsInfo.fSpeed;
		if(mbForwardable)
			this->mPhysicsInfo.Position += this->mPhysicsInfo.Velocity*dt;
		
	}

	gCamera->SetTargetTransform(this->mRenderingInfo.modelMatrix);

}