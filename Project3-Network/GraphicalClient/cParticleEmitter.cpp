#include "cParticleEmitter.h"

#include "OCHelper.h"		// for getRandomFloat
#include "managers.h"
#include "gelObjectFactory.h"

cParticleEmitter::ParticleInfo::ParticleInfo()
{
	// Sets defaults
	
	// If lifetimes are the same, they all "die" at the same time
	this->lifetimeMin = 0.0f;		// If they are less than zero, they are dead.
	this->lifetimeMax = 60.0f;

	this->regenerateParticles = true;

	// Controls the "spread" of the initial particles
	this->initVelMin = glm::vec3(-1.0f, -1.0f, -1.0f);
	this->initVelMax = glm::vec3(1.0f, 1.0f, 1.0f);

	this->ongoingForce = glm::vec3(0.0f, 0.0f, 0.0f);

	// If this is the same as "numberOfParticles", they are ALL alive
	// (like in an explosion)
	// Default is one particle per update
	this->particlesCreatedPerUpdate = 1;

	// If zero, particles don't rotate. Can rotate at different rates
	//this->rotationPerSecond = ??;

	this->randomizeColours = false;

	this->initColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->colourRangeMin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->colourRangeMax = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	this->alphaProportionalToLifeTime = false;

	return;
};


// Allocates a "pool" of particles
bool cParticleEmitter::AllocateParticles(int numberOfParticles, const ParticleInfo &initSettings, bool bOverwriteExisting /*=true*/)
{
	//
	this->particleSettings = initSettings;

	// TODO: think about if we are overwriting or not...
	this->m_vecParticles.reserve(numberOfParticles);

	// Load values from the default...
	// (which we may not want to do here... I'll exaplain in a moment)

	for (int count = 0; count != numberOfParticles; count++)
	{
		cParticle tempParticle;
		/* MeshInfo mesh;
		mesh.name = "sphere";*/
		tempParticle.meshName = this->meshInfo.name; //"square_XY";
		tempParticle.textureName_0 = this->meshInfo.textureName_0; //"brick";
		tempParticle.scale = this->renderInfo.scale;
		tempParticle.discardTextureBlack = this->renderInfo.discardTextureBlack;
		this->m_vecParticles.push_back(tempParticle);
		
	}

	return true;
}

void cParticleEmitter::ResetParticle(cParticle &particle)
{
	// Pick a starting lifetime
	//particle.SetLife( OCHelper::getRandFloat(this->particleSettings.lifetimeMin, this->particleSettings.lifetimeMax)) ;
	particle.life = OCHelper::getRandFloat(this->particleSettings.lifetimeMin, this->particleSettings.lifetimeMax);

	//PhysicsInfo physics = particle.GetPhysics();
	//RenderInfo render = particle.GetRender();

	//render.color = this->particleSettings.initColour;
	particle.colour = this->particleSettings.initColour;

	//physics.velocity.x = OCHelper::getRandFloat(this->particleSettings.initVelMin.x, this->particleSettings.initVelMax.x);
	//physics.velocity.y = OCHelper::getRandFloat(this->particleSettings.initVelMin.y, this->particleSettings.initVelMax.y);
	//physics.velocity.z = OCHelper::getRandFloat(this->particleSettings.initVelMin.z, this->particleSettings.initVelMax.z);

	particle.velocity.x = OCHelper::getRandFloat(this->particleSettings.initVelMin.x, this->particleSettings.initVelMax.x);
	particle.velocity.y = OCHelper::getRandFloat(this->particleSettings.initVelMin.y, this->particleSettings.initVelMax.y);
	particle.velocity.z = OCHelper::getRandFloat(this->particleSettings.initVelMin.z, this->particleSettings.initVelMax.z);

	// Place particle at the location of the emitter
	// +- some bounding box value, perhaps.
	particle.position = this->renderInfo.position;
	//render.position = this->position;

	//particle.SetPhysics(physics);
	//particle.SetRender(render);

	// Other things, too, in a moment

	return;
}

// Returns INDEX of a particle is found, false if they are all "alive"
// Returns -1 if no particle is found.
int cParticleEmitter::FindNextDeadParticle(void)
{
	// Linear search to find next "dead" particle
	for (int index = 0; index != this->m_vecParticles.size(); index++)
	{	// Is it dead, Jim?
		//if (this->m_vecParticles[index].GetLife() <= 0.0f)
		if (this->m_vecParticles[index].life <= 0.0f)
		{	// It's dead, Jim.
			return index;
		}
	}//for ( int index = 0
	 // Didn't find a live particle
	return -1;
}


void cParticleEmitter::Update(float deltaTime)
{
	// How many particles are we supposed to create? 
	for (int count = 0; count != this->particleSettings.particlesCreatedPerUpdate; count++)
	{
		// Find some "dead" particles... (less than equal to zero)
		int index = this->FindNextDeadParticle();
		if (index >= 0)
		{	// Update the particle
			this->ResetParticle(this->m_vecParticles[index]);
		}
	}//for ( int count = 0;

	 // Update the particles using physics and math
	for (int index = 0; index != this->m_vecParticles.size(); index++)
	{

		//if (this->m_vecParticles[index].GetLife() > 0.0f)
		if (this->m_vecParticles[index].life > 0.0f)
		{	// Update it as it's alive

			//PhysicsInfo physics = this->m_vecParticles[index].GetPhysics();
			//RenderInfo render = this->m_vecParticles[index].GetRender();

			// 1. Update acceleration based on forces
			//physics.acceleration += this->particleSettings.ongoingForce * deltaTime; // y=-10
			this->m_vecParticles[index].acceleration += this->particleSettings.ongoingForce * deltaTime; // y=-10

			// 2. Update velocity based on acceleration
			//physics.velocity += physics.acceleration * deltaTime;
			this->m_vecParticles[index].velocity += this->m_vecParticles[index].acceleration * deltaTime;

			// 3. Update position based on velocity
			//render.position += physics.velocity * deltaTime;
			this->m_vecParticles[index].position += this->m_vecParticles[index].velocity * deltaTime;
			//this->m_vecParticles[index].position += this->m_vecParticles[index].velocity * deltaTime;

			// If they are an imposter, orient the object towards the camera
			if (this->particleSettings.isImposter)
			{
				this->m_vecParticles[index].rotation = this->CalcImposterRotationFromEye(this->m_vecParticles[index].position);
				//render.rotation = this->CalcImposterRotationFromEye(render.position);
			}

			if (this->particleSettings.randomizeColours)
			{
				this->m_vecParticles[index].colour.r = OCHelper::getRandFloat(0.0f, 1.0f);
				this->m_vecParticles[index].colour.g = OCHelper::getRandFloat(0.0f, 1.0f);
				this->m_vecParticles[index].colour.b = OCHelper::getRandFloat(0.0f, 1.0f);
				this->m_vecParticles[index].colour.a = 1.0f;

				/*render.color.r = OCHelper::getRandFloat(0.0f, 1.0f);
				render.color.g = OCHelper::getRandFloat(0.0f, 1.0f);
				render.color.b = OCHelper::getRandFloat(0.0f, 1.0f);
				render.color.a = 1.0f;*/
			}

			//this->m_vecParticles[index].SetPhysics(physics);
			//this->m_vecParticles[index].SetRender(render);

		}//if ( this->m_vecParticles[index].life > 0.0f )

		 // "age" the particle
		//this->m_vecParticles[index].DecayLife( deltaTime );
		this->m_vecParticles[index].Update(deltaTime);
	}//for ( int index = 0;


	return;
}

std::string cParticleEmitter::ObjectType(void)
{
	return "particleEmitter";
}




int cParticleEmitter::GetParticlesCopy(std::vector<cParticle> &vecParticles)
{
	// OPTIONAL: 
	// Make sure the vector passed is already the right size, etc.
	if (vecParticles.size() < this->m_vecParticles.size())
	{	// It's not, so allocate and resize
		vecParticles.reserve(this->m_vecParticles.size());
		unsigned int particleToCreate = this->m_vecParticles.size() - vecParticles.size();
		for (int count = 0; count != particleToCreate; count++)
		{	// Add a "dummy" particle
			cParticle emptyParticle;		// instantiating a new particle
			vecParticles.push_back(emptyParticle);
		}
	}//if ( vecParticles.size() < this->m_vecParticles.size() )

	 // At this point, the vector being copied into is big enough.
	unsigned int indexReturned = 0;
	for (unsigned int index = 0; index != vecParticles.size(); index++)
	{
		// Copy if particles are alive, only
		//if (this->m_vecParticles[index].GetLife() > 0.0f)
		if (this->m_vecParticles[index].life > 0.0f)
		{
			cParticle particleToCopy = this->m_vecParticles[index];
			vecParticles[indexReturned] = particleToCopy;
			//vecParticles[indexReturned].SetLife(particleToCopy.GetLife());
			//vecParticles[indexReturned].SetRender(particleToCopy.GetRender());
			//vecParticles[indexReturned].SetMesh(particleToCopy.GetMesh());
			//vecParticles[indexReturned].SetPhysics(particleToCopy.GetPhysics());
			indexReturned++;
		}
	}

	return indexReturned;
}

void cParticleEmitter::UpdateImposterEye(glm::vec3 eye)
{
	this->particleSettings.imposterEyeLocation = eye;
	return;
}


glm::vec3 cParticleEmitter::CalcImposterRotationFromEye(glm::vec3 particlePos)
{
	// http://www.rastertek.com/dx11tut34.html
	// 	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	// angle = atan2(modelPosition.x - cameraPosition.x, 
	//               modelPosition.z - cameraPosition.z) 
	//			     * (180.0 / D3DX_PI);

	// Convert rotation into radians.
	// rotation = (float)angle * 0.0174532925f

	float angle = atan2(particlePos.x - this->particleSettings.imposterEyeLocation.x,
		particlePos.z - this->particleSettings.imposterEyeLocation.z);
	//* ( 180.0 / PI );
	// Since we aren't paying any attention to the x and z rotations.
	return glm::vec3(0.0f, angle, 0.0f);
}

cParticleEmitter::cParticleEmitter()
{
	this->objectID = gelObjectManager::sharedInstance().GetNewGameObjectID();
}

LightInfo cParticleEmitter::GetLight(void)
{
	return this->lightInfo;
}

MeshInfo cParticleEmitter::GetMesh(void)
{
	return this->meshInfo;
}

RenderInfo cParticleEmitter::GetRender(void)
{
	return this->renderInfo;
}


PhysicsInfo cParticleEmitter::GetPhysics(void)
{
	return this->physicsInfo;
}

CollisionInfo cParticleEmitter::GetCollision(void)
{
	return this->collisionInfo;
}

void cParticleEmitter::SetCollision(CollisionInfo info)
{
	this->collisionInfo = info;
}

void cParticleEmitter::SetLight(LightInfo info)
{
	this->lightInfo = info;
}

void cParticleEmitter::SetMesh(MeshInfo info)
{
	this->meshInfo = info;
}
void cParticleEmitter::SetRender(RenderInfo info)
{
	this->renderInfo = info;
}
void cParticleEmitter::SetPhysics(PhysicsInfo info)
{
	this->physicsInfo = info;
}

int cParticleEmitter::getObjectID()
{
	return this->objectID;
}