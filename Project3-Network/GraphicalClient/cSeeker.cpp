#include "cSeeker.h"

#include "externals.h"
#include "managers.h"
#include "OCHelper.h"
#include "GelsenizeCore.h"

cSeeker::cSeeker()
{
	this->currentMood = CURIOUS;
	this->currentBehaviour = REST;

	this->wanderAccumulator = 0.0f;
	this->wanderDuration = 2.0f; //pick a direction every 2.0f~ deltaTime

	this->random_x = (OCHelper::getRandFloat(-1.0, 1.0));
	this->random_z = (OCHelper::getRandFloat(-1.0, 1.0));

	this->attentionRadius = GelsenizeCore::sharedInstance().GetAttentionRadius();
	this->dangerRadius = GelsenizeCore::sharedInstance().GetDangerRadius();
	this->drawAttentionRadius = false;

	this->dexterityModifier = rand() % 3; // between 0 and +2 
	this->intelligenceModifier = rand() % 6; // between 0 and +5 
	this->focus = 1.0f;
	this->logicalCooldown = 2.0f + (intelligenceModifier / 10.0f); // 2+int seconds of delta time is needed for this to make a decision
	this->logicalCooldown = 2.0f;
	this->speedK = OCHelper::getRandFloat(1.8f, 2.4f) + (dexterityModifier / 10.0f); //calculate speed k

	this->objectID = gelObjectManager::sharedInstance().GetNewGameObjectID();

	this->GetLight();
	this->GetPhysics();
	this->GetMesh();
	this->GetRender();
}

cSeeker::~cSeeker()
{
}

LightInfo cSeeker::GetLight(void)
{
	return this->lightInfo;
}

MeshInfo cSeeker::GetMesh(void)
{
	return this->meshInfo;
}

RenderInfo cSeeker::GetRender(void)
{
	return this->renderInfo;
}


PhysicsInfo cSeeker::GetPhysics(void)
{
	return this->physicsInfo;
}

CollisionInfo cSeeker::GetCollision(void)
{
	return this->collisionInfo;
}

void cSeeker::SetCollision(CollisionInfo info)
{
	this->collisionInfo = info;
}

void cSeeker::SetLight(LightInfo info)
{
	this->lightInfo = info;
}

void cSeeker::SetMesh(MeshInfo info)
{
	this->meshInfo = info;
}
void cSeeker::SetRender(RenderInfo info)
{
	this->renderInfo = info;
}
void cSeeker::SetPhysics(PhysicsInfo info)
{
	this->physicsInfo = info;
}

int cSeeker::getObjectID()
{
	return this->objectID;
}

void cSeeker::Update(float deltaTime)
{

	//AI
	this->focus += deltaTime; //gain focus every cycle

	RenderInfo playerRender = gelObjectManager::sharedInstance().GetMainPlayer()->GetRender();
	glm::mat4x4 playerTransform = playerRender.transform;

	glm::vec3 playerPosition(playerTransform[3].x, 1.0f, playerTransform[3].z);
	glm::vec3 myPosition(this->renderInfo.position.x, 1.0f, this->renderInfo.position.z);
	float distanceToPlayer = glm::distance(myPosition, playerPosition);

	if (focus >= logicalCooldown)
	{
		//i am allowed to think finally...

		//get player's forward vector to see it if it can see the monster.
		glm::vec4 playerDirection = glm::vec4(playerPosition, 0.0f);
		glm::vec4 forward(0.0f, 0.0f, -1.0f, 0.0f); //forward based on model
		playerDirection = playerTransform * forward;
		playerDirection = glm::normalize(playerDirection); //really needed as we are only interested in the angle between

		//subctract two points to get "sight" vector of monster
		//this may be a property for this class might as well. 
		//maybe with parameters like, field of vision angle and gameobject reference
		glm::vec4 mySight = glm::vec4(
			playerRender.position.x - this->renderInfo.position.x,
			1.0f,
			playerRender.position.z - this->renderInfo.position.z,
			0.0f /*direction*/);
		mySight = glm::normalize(mySight);

		float dotProduct = glm::dot(mySight, playerDirection); //dot product mysight and player's forward to see if the player can see me
		BEHAVIOUR chosenBehaviour;

		//check the attention radius
		//if out-> rest for now
		//else ->decide to pursuit
		if (distanceToPlayer <= attentionRadius)
		{
			//inside attention radius
			//check danger zone - back away if too close
			if (dotProduct > 0.0f)
			{
				//player can see me
				if (currentMood == MOOD::DEFAULT)
				{
					chosenBehaviour = RUN;

					//change color to green
					this->renderInfo.color.r = 0.0f;
					this->renderInfo.color.g = 1.0f;
					this->renderInfo.color.b = 0.0f;
				}
				else
				{
					if (currentMood == MOOD::ANGRY)
					{
						chosenBehaviour = EVADE;

						//change color to orange
						this->renderInfo.color.r = 0.93f;
						this->renderInfo.color.g = 0.25f;
						this->renderInfo.color.b = 0.0f;
					}
					else if (currentMood == MOOD::CURIOUS)
					{
						chosenBehaviour = EVADE;

						//change color to purple
						this->renderInfo.color.r = 0.60f;
						this->renderInfo.color.g = 0.0f;
						this->renderInfo.color.b = 0.60f;
					}
					else
					{
						// n/a
					}
				}

			}
			else
			{
				//player can't see me
				if (currentMood == MOOD::DEFAULT)
				{
					chosenBehaviour = SEEK;

					//change color to red
					this->renderInfo.color.r = 1.0f;
					this->renderInfo.color.g = 0.0f;
					this->renderInfo.color.b = 0.0f;
				}
				else
				{
					if (currentMood == MOOD::ANGRY)
					{
						if (distanceToPlayer < dangerRadius)
						{
							chosenBehaviour = EVADE;
							//change color to orange
							this->renderInfo.color.r = 0.93f;
							this->renderInfo.color.g = 0.25f;
							this->renderInfo.color.b = 0.0f;
						}
						else
						{
							chosenBehaviour = PURSUIT;
							//change color to red
							this->renderInfo.color.r = 1.0f;
							this->renderInfo.color.g = 0.0f;
							this->renderInfo.color.b = 0.0f;
						}
					}
					else if (currentMood == MOOD::CURIOUS)
					{
						chosenBehaviour = ARRIVE;

						//change color to blue
						this->renderInfo.color.r = 0.0f;
						this->renderInfo.color.g = 0.0f;
						this->renderInfo.color.b = 1.0f;
					}
					else
					{
						// n/a
						chosenBehaviour = REST;
					}
				}
			}
		}
		else
		{
			//outside attention radius
			chosenBehaviour = WANDER; //wander here
			//wanderAccumulator = 0.0f;
			//change color to transparent-gray
			this->renderInfo.color.r = 1.0f;// 0.8f;
			this->renderInfo.color.g = 1.0f;//0.38f;
			this->renderInfo.color.b = 1.0f;//0.56f;
			this->renderInfo.color.a = 1.0f;
		}

		

		if (currentBehaviour != chosenBehaviour)
		{
			//change in the state of monster
			//thinking reduces focus
			focus = 0.0f;
		}
		currentBehaviour = chosenBehaviour;
	}
	else
	{
		//reflexes 
		//these can trigger without cooldown
		if (currentMood == MOOD::ANGRY && distanceToPlayer < dangerRadius)
		{
			currentBehaviour = EVADE;
			//change color to orange
			this->renderInfo.color.r = 0.93f;
			this->renderInfo.color.g = 0.25f;
			this->renderInfo.color.b = 0.0f;
		}
	}

	//apply last behaviour no matter what
	Behave(deltaTime);
}

std::string cSeeker::ObjectType(void)
{
	return "seeker";
}

void cSeeker::AssignMoodAndSpeed(std::string newMood, float averageSpeed)
{
	if (newMood == "curious")
	{
		this->currentMood = MOOD::CURIOUS;
	}
	else if (newMood == "angry")
	{
		this->currentMood = MOOD::ANGRY;
	}
	else
	{
		this->currentMood = MOOD::DEFAULT;
	}

	this->speedK = OCHelper::getRandFloat(averageSpeed*0.75f, averageSpeed*1.25f);
}

void cSeeker::Behave(float deltaTime)
{
	//get the render info of player and this monster
	iGameObject *mainPlayer = gelObjectManager::sharedInstance().GetMainPlayer();
	RenderInfo playerRender = mainPlayer->GetRender();

	glm::vec3 playerPosition(playerRender.transform[3].x, 1.0f, playerRender.transform[3].z);
	glm::vec3 myPosition(this->renderInfo.position.x, 1.0f, this->renderInfo.position.z);
	float distanceToPlayer = glm::distance(myPosition, playerPosition);

	glm::mat4x4 playerTransform = playerRender.transform;
	glm::mat4x4 monsterTransform = this->renderInfo.transform;
	glm::vec4 forwardVector;
	glm::vec4 forward(0.0f, 0.0f, -1.0f, 0.0f); //forward for this model
	glm::vec4 playerForward(0.0f, 0.0f, 2.0f, 0.0f); //forward for this model chase 2.0f ahead

	//subctract two points to get "sight" vector of monster
	
	//block to set monster sight
	glm::vec4 monsterSight;
	if (currentBehaviour == RUN || currentBehaviour == SEEK || currentBehaviour == ARRIVE)
	{
		//base the monster sight on player's position
		monsterSight = glm::vec4( playerRender.position.x - this->renderInfo.position.x, 1.0f, playerRender.position.z - this->renderInfo.position.z, 0.0f /*direction*/);
		monsterSight = glm::normalize(monsterSight);
	}
	else if (currentBehaviour == PURSUIT || currentBehaviour == EVADE)
	{
		PhysicsInfo playerPhysics = mainPlayer->GetPhysics();
		GelVelocity playerVelocity = playerPhysics.velocity;
		if (playerVelocity.x == 0.0f && playerVelocity.z == 0.0f)
		{
			//base the monster sight on player's position
			monsterSight = glm::vec4( playerRender.position.x - this->renderInfo.position.x, 1.0f, playerRender.position.z - this->renderInfo.position.z, 0.0f /*direction*/);
			monsterSight = glm::normalize(monsterSight);
		}
		else
		{
			//base the monster sight on player's forward direction
			glm::vec4 playerPosition = glm::vec4(playerTransform[3].x, 1.0f, playerTransform[3].z, 0.0f);
			glm::vec4 playerForwardVector = playerPosition;// +playerForward;
			playerForwardVector.x += playerPhysics.velocity.x *deltaTime;
			playerForwardVector.y = 1.0f;
			playerForwardVector.z += playerPhysics.velocity.z *deltaTime;
			//get monster x,z to find monster's sight
			glm::vec4 monsterPosition = glm::vec4(this->renderInfo.position.x , 1.0f, this->renderInfo.position.z, 0.0f);

			monsterSight = playerForwardVector - monsterPosition;
			monsterSight = glm::normalize(monsterSight);
		}
	}
	else if (currentBehaviour == WANDER)
	{
		wanderAccumulator += deltaTime;
		if (wanderAccumulator >= wanderDuration)
		{
			//pick a new position to go
			wanderAccumulator = 0.0f;
			random_x = OCHelper::getRandFloat(-1.0, 1.0);
			random_z = OCHelper::getRandFloat(-1.0, 1.0);
		}
		//choose direction go that way for up to 1 second before choosing a new direction
		
		glm::vec4 targetDirection(this->renderInfo.position.x + random_x, this->renderInfo.position.y, this->renderInfo.position.z + random_z, 0.0f);
		glm::vec4 monsterPosition = glm::vec4(this->renderInfo.position.x, 1.0f, this->renderInfo.position.z, 0.0f);

		monsterSight = targetDirection - monsterPosition;
		monsterSight = glm::normalize(monsterSight) * 2.0f;
	}
	else
	{
		//decide what to do on rest
	}

	glm::vec4 speedVec; //sight vector becomes the velocity vector
	GelVelocity newVelocity;
	float rotationValue = 0;
	float defaultRotationValue = 1.5f;

	//get forward vector
	forwardVector = monsterTransform * forward;
	//cross product forward vector of the monster with my sight (because i need to rotate towards my sight, which is the player's position)
	glm::vec3 crossP = glm::cross(
		glm::vec3(monsterSight.x,  monsterSight.y,  monsterSight.z),
		glm::vec3(forwardVector.x, forwardVector.y, forwardVector.z));

	if (currentBehaviour == RUN || currentBehaviour == EVADE)
	{
		//set speed
		speedVec = monsterSight;
		speedVec *= speedK; //running away speed 20% faster

		//run away direction
		newVelocity.x = -speedVec.x ;
		newVelocity.y = this->physicsInfo.velocity.y ; //chase in 2d
		newVelocity.z = -speedVec.z ;

		if (crossP.y > 0.0f) //check the Y axis sign to determine facing rotation & set rotation values
		{
			rotationValue = -defaultRotationValue;
			;
		}
		else
		{
			rotationValue = defaultRotationValue;
		}
	}
	else if (currentBehaviour == SEEK || currentBehaviour == PURSUIT || currentBehaviour == WANDER)
	{
		speedVec = monsterSight;
		speedVec *= speedK;

		//chase direction
		newVelocity.x = speedVec.x ;
		newVelocity.y = this->physicsInfo.velocity.y ; //chase in 2d
		newVelocity.z = speedVec.z ;

		if (crossP.y > 0.0f) //check the Y axis sign to determine facing rotation & set rotation values
		{
			rotationValue = defaultRotationValue;
		}
		else
		{
			rotationValue = -defaultRotationValue;
		}
	}
	else if (currentBehaviour == ARRIVE)
	{
		//speed depends on the distance to player, scaled linearly to 0% at 0.0f 100% at attention radius/4.0f
		
		//no matter what the distance, if we are closer than danger zone we need to back away,
		if (distanceToPlayer < dangerRadius)
		{
			//set speed
			speedVec = monsterSight;
			speedVec *= speedK * 2.0f; //back away speed fast 
			//back away
			newVelocity.x = -speedVec.x;
			newVelocity.y = this->physicsInfo.velocity.y; //chase in 2d
			newVelocity.z = -speedVec.z;
		}
		else if (distanceToPlayer < dangerRadius + 0.1f)
		{
			newVelocity.x = 0.0f;
			newVelocity.y = this->physicsInfo.velocity.y; //chase in 2d
			newVelocity.z = 0.0f;
		}
		else
		{
			//scale velocity
			//100% at (attentionRadius-dangerRadius) and greater, scale to 0 
			float desiredVelocityPercentage = distanceToPlayer * 1 / (attentionRadius - (attentionRadius*0.8f));
			if (desiredVelocityPercentage > 1.0f)
			{
				desiredVelocityPercentage = 1.0f;
			}
			
			//set speed
			speedVec = monsterSight;
			speedVec *= speedK; 
			speedVec *= desiredVelocityPercentage;

			newVelocity.x = speedVec.x;
			newVelocity.y = this->physicsInfo.velocity.y; //chase in 2d
			newVelocity.z = speedVec.z;
		}


		if (crossP.y > 0.0f) //check the Y axis sign to determine facing rotation & set rotation values
		{
			rotationValue = defaultRotationValue;
		}
		else
		{
			rotationValue = -defaultRotationValue;
		}
	}
	else
	{
		newVelocity = GelVelocity(0.0f, 0.0f, 0.0f);
		rotationValue = 0;
	}

	//assign set values to render and physics info

	//this->renderInfo.rotation.y += rotationValue;

	// Represents the change over 1 second.
	glm::quat qRotChangeZeroSeconds;		// No rotational change
	glm::quat qRotChange(glm::vec3(0.0f, rotationValue, 0.0f));
	glm::quat qRotChangeThisFrame = glm::slerp(qRotChangeZeroSeconds, qRotChange, deltaTime);

	this->renderInfo.orientation *= qRotChangeThisFrame;

	this->physicsInfo.velocity = newVelocity;
}