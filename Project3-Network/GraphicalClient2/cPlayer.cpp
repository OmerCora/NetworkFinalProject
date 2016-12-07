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

	//initial direction
	this->mPhysicsInfo.Orientation = glm::rotate(this->mPhysicsInfo.Orientation, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//this->mPhysicsInfo.fRotationSpeed = 1.6;
}

cPlayer::~cPlayer()
{

}

void cPlayer::Update(float dt)
{
	if(g_bMonopolyProceed)
	{
		float locationRange = gAreaInfo->getLocationRange();//will be re-arrange in the loop below
		float jumpToNextRange = locationRange; 

		glm::vec3 targetLocation(0.0f, 0.0f, 0.0f);

		int currentLocNumber = 0; // will get from the game logic 
		int numberOnDice = 13; // will get from the game logic 
		int targetLocNumber = (currentLocNumber + numberOnDice) %40; // total number of locations

		
		float currentPositionX = this->mPhysicsInfo.Position.x;
		float currentPositionZ = this->mPhysicsInfo.Position.z;


		int currnetPlayer = 1; // will get from the game logic 

		if (this->mPlayerNumber == currnetPlayer)
		{
			for (int i = 0; i < numberOnDice; i++)
			{
				currentLocNumber++;

				if (currentLocNumber % 10 == 0)
				{
					jumpToNextRange = 1.5 * locationRange;
				}
				else
				{
					jumpToNextRange = locationRange;
				}

				if (currentLocNumber >= 0 && currentLocNumber < 10)
				{
					targetLocation = glm::vec3(currentPositionX - jumpToNextRange, 0.0f, currentPositionZ);
				}
				else if(currentLocNumber >= 10 && currentLocNumber < 20)
				{
					targetLocation = glm::vec3(currentPositionX, 0.0f, currentPositionZ + jumpToNextRange);
				}
				else if (currentLocNumber >= 20 && currentLocNumber < 30)
				{
					targetLocation = glm::vec3(currentPositionX + jumpToNextRange, 0.0f, currentPositionZ);
				}
				else
				{
					targetLocation = glm::vec3(currentPositionX, 0.0f, currentPositionZ - jumpToNextRange);
				}

				glm::vec3 targetToPlayer = this->mPhysicsInfo.Position - targetLocation;
				targetToPlayer = glm::normalize(targetToPlayer);
				glm::vec3 myForward = this->mPhysicsInfo.Orientation*glm::vec3(0.f, 0.f, 1.0f);

				glm::vec3 rotationAxis = glm::cross(myForward, targetToPlayer);
				if(mbForwardable)
				this->mPhysicsInfo.Position += myForward* 2.0f *dt;

				float YAxisValue = rotationAxis.y > 0.f ? 1.f : -1.f;
				YAxisValue *= -1.f;
				rotationAxis = glm::vec3(0.f, YAxisValue, 0.f);
				float rotationVelocity = 2.f;
				this->mPhysicsInfo.Orientation = glm::rotate(this->mPhysicsInfo.Orientation, rotationVelocity*dt, rotationAxis);
				
				
			}
		}	//if (this->mPlayerNumber == currnetPlayer)	

		//g_bMonopolyProceed = false; //should be turn this to true when a player did something

	}//if(g_bMonopolyProceed)

	

	////up vector for rotation
	//glm::vec3 up(0.f, 1.f, 0.f);

	////key press A -> turn left / key press 'D' -> turn right
	//bool pressA = (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS);
	//bool pressD = (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS);
	//if (pressA != pressD)
	//{
	//	this->mPhysicsInfo.fRotationSpeed = pressA ? 1.8f : -1.8f;

	//	this->mPhysicsInfo.Orientation = glm::rotate(this->mPhysicsInfo.Orientation, 
	//												this->mPhysicsInfo.fRotationSpeed*dt, up);

	//	if (!mbForwardable) mbForwardable = true;
	//}

	////key press W ->forward / key press 'S' -> backward
	//bool pressW = (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS);
	//bool pressS = (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS);

	////glm::vec3 forward(0.f, 0.f, 1.f);
	////glm::vec3 mforward(0.f, 0.f, 1.f);

	//if (pressW != pressS)
	//{
	//	this->mPhysicsInfo.fSpeed = pressW ? 10.0f : -10.0f;
	//	
	//	//mforward = this->GetForward();
	//	//forward = mPhysicsInfo.Orientation * forward;
	//	//this->mPhysicsInfo.Velocity = this->GetForward() * this->mPhysicsInfo.fSpeed;
	//	this->mPhysicsInfo.Velocity = this->mPhysicsInfo.Orientation * glm::vec3(0.f, 0.f, 1.f) * this->mPhysicsInfo.fSpeed;
	//	if(mbForwardable)
	//		this->mPhysicsInfo.Position += this->mPhysicsInfo.Velocity*dt;
	//	
	//}

	////gCamera->SetTargetTransform(this->mRenderingInfo.modelMatrix);

}