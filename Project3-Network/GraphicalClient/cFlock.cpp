#include "cFlock.h"

#include "externals.h"
#include "managers.h"
#include "GelsenizeCore.h"

#include "gelObjectFactory.h"

#include "OCHelper.h"
#include "cMoveCommand.h"

LightInfo cFlock::GetLight(void)
{
	return this->lightInfo;
}

MeshInfo cFlock::GetMesh(void)
{
	return this->meshInfo;
}

RenderInfo cFlock::GetRender(void)
{
	return this->renderInfo;
}


PhysicsInfo cFlock::GetPhysics(void)
{
	return this->physicsInfo;
}

CollisionInfo cFlock::GetCollision(void)
{
	return this->collisionInfo;
}

void cFlock::SetCollision(CollisionInfo info)
{
	this->collisionInfo = info;
}

void cFlock::SetLight(LightInfo info)
{
	this->lightInfo = info;
}

void cFlock::SetMesh(MeshInfo info)
{
	this->meshInfo = info;
}
void cFlock::SetRender(RenderInfo info)
{
	this->renderInfo = info;
}
void cFlock::SetPhysics(PhysicsInfo info)
{
	this->physicsInfo = info;
}

int cFlock::getObjectID()
{
	return this->objectID;
}

void cFlock::Update(float deltaTime)
{
	
	//
	// FLOCKING LOGIC
	//

	//foreach flock member
	for (int index = 0; index != members.size(); index++)
	{
		iGameObject* flockMember = members[index];

		//calculate and add these vectors, then set it as velocity
		glm::vec3 cohesionVector;
		glm::vec3 alignmentVector;
		glm::vec3 separationVector;
		
		GetCohesionVector(flockMember, cohesionVector);
		GetAlignmentVector(flockMember, alignmentVector);
		GetSeparationVector(flockMember, separationVector);

		if (glm::length(cohesionVector) > 0.0f )
			cohesionVector = glm::normalize(cohesionVector);
		if (glm::length(alignmentVector) > 0.0f )
			alignmentVector = glm::normalize(alignmentVector);
		if (glm::length(separationVector) > 0.0f )
			separationVector = glm::normalize(separationVector);

		cohesionVector *= cohesionWeight;
		alignmentVector *= alignmentWeight;
		separationVector *= separationWeight;

		glm::vec3 velocityFinal = cohesionVector + alignmentVector + separationVector;

		if (velocityFinal.x != velocityFinal.x || velocityFinal.z != velocityFinal.z)
		{
			int notANumberError = -1;
		}

		RenderInfo memberRender = flockMember->GetRender();
		PhysicsInfo memberPhysics = flockMember->GetPhysics();
		glm::vec3 desiredVelocity = velocityFinal * speedMultiplier;

		glm::vec3 distToTarget;
		distToTarget.x = std::abs(desiredVelocity.x - memberPhysics.velocity.x);
		distToTarget.y = std::abs(desiredVelocity.y - memberPhysics.velocity.y);
		distToTarget.z = std::abs(desiredVelocity.z - memberPhysics.velocity.z);

		// Calculate the relative speeds for each axis (could be moving fast on one axis and slow on another)
		glm::vec3 moveStep;
		moveStep.x = OCHelper::smootherStep(0.0f, desiredVelocity.x, distToTarget.x);
		moveStep.y = OCHelper::smootherStep(0.0f, desiredVelocity.y, distToTarget.y);
		moveStep.z = OCHelper::smootherStep(0.0f, desiredVelocity.z, distToTarget.z);

		// Scale for time step
		moveStep.x *= 5.0f * deltaTime;
		moveStep.y *= 5.0f * deltaTime;
		moveStep.z *= 5.0f * deltaTime;

		// If camera is LT position, reverse the moving direction (it's positive because of ABS distance)
		if (desiredVelocity.x < memberPhysics.velocity.x) { moveStep.x = -moveStep.x; }
		if (desiredVelocity.y < memberPhysics.velocity.y) { moveStep.y = -moveStep.y; }
		if (desiredVelocity.z < memberPhysics.velocity.z) { moveStep.z = -moveStep.z; }

		// If we are outside of the min distance, move the camera
		if (distToTarget.x > 0.0f) { memberPhysics.velocity.x += moveStep.x; }
		if (distToTarget.y > 0.0f) { memberPhysics.velocity.y += moveStep.y; }
		if (distToTarget.z > 0.0f) { memberPhysics.velocity.z += moveStep.z; }


		//memberPhysics.velocity = velocityFinal;
		memberPhysics.velocity.y = 0.0f;

		flockMember->SetPhysics(memberPhysics);

		glm::vec4 forward = glm::vec4(0.0f,0.0f,-1.0f,0.0f);
		glm::vec4 forwardVector = memberRender.transform * forward;

		glm::vec3 crossP = glm::cross(
			glm::vec3(memberPhysics.velocity.x, memberPhysics.velocity.y, memberPhysics.velocity.z),
			glm::vec3(forwardVector.x, forwardVector.y, forwardVector.z));

		float rotationValue = 1.0f;

		rotationValue = 1 - crossP.y;
		if (crossP.y > 0.0f)
		{
			rotationValue *= +1.0f;
		}
		else
		{
			rotationValue *= -1.0f;
		}

		//adjust orientation
		glm::quat qRotChangeZeroSeconds;		// No rotational change
		glm::quat qRotChange(glm::vec3(0.0f, rotationValue, 0.0f));
		glm::quat qRotChangeThisFrame = glm::slerp(qRotChangeZeroSeconds, qRotChange, deltaTime);

		memberRender.orientation *= qRotChangeThisFrame;
		flockMember->SetRender(memberRender);

		//calculate separation vector
	}
}

std::string cFlock::ObjectType(void)
{
	return "flock";
}

void cFlock::GetSeparationVector(iGameObject * flockMember, glm::vec3 &separationVector)
{
	int memberID = flockMember->getObjectID();

	RenderInfo memberRender = flockMember->GetRender();
	PhysicsInfo memberPhysics = flockMember->GetPhysics();

	glm::vec3 memberPosition = memberRender.position;

	glm::vec3 separationVectorFinal = glm::vec3(0.0f,0.0f,0.0f);

	//check if each member is close enough to be avoided
	for (int index = 0; index != members.size(); index++)
	{
		iGameObject* otherFlockMember = members[index];
		if (memberID != otherFlockMember->getObjectID()) //check if really "another"
		{
			RenderInfo otherMemberRender = otherFlockMember->GetRender();
			glm::vec3 otherMemberPosition = otherMemberRender.position;

			float distanceToOtherMember = glm::distance(memberPosition, otherMemberPosition);
			if (distanceToOtherMember <= radiusSeparation)
			{
				//too close

				float ratioDistance = distanceToOtherMember / radiusSeparation;

				//add distance scaled vector to separation total
				separationVectorFinal += glm::normalize(memberPosition - otherMemberPosition) * ratioDistance;
			}
		}
	}

	//assign
	separationVector = separationVectorFinal;
}

void cFlock::GetCohesionVector(iGameObject * flockMember, glm::vec3 &cohesionVector)
{
	int memberID = flockMember->getObjectID();

	RenderInfo memberRender = flockMember->GetRender();
	PhysicsInfo memberPhysics = flockMember->GetPhysics();

	glm::vec3 memberPosition = memberRender.position;

	glm::vec3 neighbourCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	int neighbourCount = 0;
	glm::vec3 cohesionVectorFinal = glm::vec3(0.0f, 0.0f, 0.0f);

	//check to find the perfect spot to fit yourself in flock
	for (int index = 0; index != members.size(); index++)
	{
		iGameObject* otherFlockMember = members[index];
		if (memberID != otherFlockMember->getObjectID()) //check if really "another"
		{
			RenderInfo otherMemberRender = otherFlockMember->GetRender();
			glm::vec3 otherMemberPosition = otherMemberRender.position;

			float distanceToOtherMember = glm::distance(memberPosition, otherMemberPosition);
			if (distanceToOtherMember <= radiusNeighbour)
			{
				//in neighbour radius
				neighbourCount++;
				neighbourCenter += otherMemberPosition;
			}
		}
	}

	if (neighbourCount > 0)
	{
		neighbourCenter /= neighbourCount;
	}

	//average
	cohesionVectorFinal = neighbourCenter;

	//assign
	cohesionVector = cohesionVectorFinal;
}

void cFlock::GetAlignmentVector(iGameObject * flockMember, glm::vec3 &alingmentVector)
{
	int memberID = flockMember->getObjectID();

	RenderInfo memberRender = flockMember->GetRender();
	PhysicsInfo memberPhysics = flockMember->GetPhysics();

	glm::vec3 memberPosition = memberRender.position;

	glm::vec3 neighbourVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	int neighbourCount = 0;

	glm::vec3 alingmentVectorFinal = glm::vec3(0.0f, 0.0f, 0.0f);

	//check if each member is close enough to be followed
	for (int index = 0; index != members.size(); index++)
	{
		iGameObject* otherFlockMember = members[index];
		if (memberID != otherFlockMember->getObjectID()) //check if really "another"
		{
			RenderInfo otherMemberRender = otherFlockMember->GetRender();
			glm::vec3 otherMemberPosition = otherMemberRender.position;

			float distanceToOtherMember = glm::distance(memberPosition, otherMemberPosition);
			if (distanceToOtherMember <= radiusNeighbour)
			{
				//neighbour
				neighbourCount++;

				//get velocity
				alingmentVectorFinal += otherFlockMember->GetPhysics().velocity;
			}
		}
	}

	//average
	if (neighbourCount > 0)
	{
		neighbourVelocity /= neighbourCount;
	}

	//limit
	alingmentVectorFinal = glm::clamp(neighbourVelocity, -radiusNeighbour, radiusNeighbour);

	//assign
	alingmentVector = alingmentVectorFinal;
}

cFlock::cFlock()
{
	this->objectID = gelObjectManager::sharedInstance().GetNewGameObjectID();

	this->GetLight();
	this->GetPhysics();
	this->GetMesh();
	this->GetRender();

	this->renderInfo.useDebugColorsNoLighting = false;
}

cFlock::cFlock(int flockPopulation, glm::vec3 startingPoint)
{
	this->objectID = gelObjectManager::sharedInstance().GetNewGameObjectID();

	radiusNeighbour = 8.0f;
	radiusSeparation = 2.0f;
	
	//altering these changes flocking behaviour
	separationWeight = 0.8f;
	alignmentWeight = 1.0f;
	cohesionWeight = 2.4f; 
	
	speedMultiplier = 3.0f;

	this->members.clear();
	this->members.reserve(flockPopulation);

	for (int i = 0; i < flockPopulation; i++)
	{
		iGameObject* newFlockMember = gelObjectFactory::sharedInstance().createGameObject("object");
		RenderInfo render;
		MeshInfo mesh;
		PhysicsInfo physicsFlock;

		render.position.x = startingPoint.x + OCHelper::getRandFloat(-2.0f, 2.0f);
		render.position.y = startingPoint.y;
		render.position.z = startingPoint.z + OCHelper::getRandFloat(-2.0f, 2.0f);
		render.useTextureAsDiffuse = false;

		render.color = glm::vec4(0.90f, 0.45f, 0.80f, 1.0f);

		mesh.name = "plane1";
		
		physicsFlock.enabled = true;
		physicsFlock.velocity.x = OCHelper::getRandFloat(0.0f, 2.0f);
		physicsFlock.velocity.y = 0.0f;
		physicsFlock.velocity.z = OCHelper::getRandFloat(0.0f, 2.0f);

		newFlockMember->SetPhysics(physicsFlock);
		newFlockMember->SetMesh(mesh);
		newFlockMember->SetRender(render);

		this->members.push_back(newFlockMember);
	}
}

cFlock::~cFlock()
{
}

void cFlock::GetMembers(std::vector<iGameObject*>& flockMembers)
{
	flockMembers = this->members;
}
