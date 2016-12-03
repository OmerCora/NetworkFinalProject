#ifndef _CPARTICLEEMITTER_HG_
#define _CPARTICLEEMITTER_HG_

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "iGameObject.h"

#include "cParticle.h"

class cParticleEmitter : public iGameObject
{
public:
	cParticleEmitter();

	static const int DEFAULT_NUMBER_OF_PARTICLES = 100;

	// Used during allocation and generation to 
	// specify various characteristics of the particles
	struct ParticleInfo
	{
		ParticleInfo();	// Sets defaults
							// If lifetimes are the same, they all "die" at the same time
		bool regenerateParticles;
		bool isImposter;
		// If this is true, than a random colour between the range will
		//	be chosen, otherwise "initColour" will be used
		bool randomizeColours;
		// If true, the alpha value is a ratio of the current vs the 
		//  max particle life, so they "fade" away over time
		bool alphaProportionalToLifeTime;

		// If this is the same as "numberOfParticles", they are ALL alive
		// (like in an explosion)
		int particlesCreatedPerUpdate;

		float lifetimeMin, lifetimeMax;
		
		// Controls the "spread" of the initial particles
		glm::vec3 initVelMin, initVelMax;
		glm::vec3 ongoingForce;
		// If zero, particles don't rotate. Can rotate at different rates
		glm::vec3 rotationPerSecond;
		glm::vec3 imposterEyeLocation;
		glm::vec4 initColour, colourRangeMin, colourRangeMax;
	};

	virtual LightInfo GetLight(void);
	virtual MeshInfo GetMesh(void);
	virtual RenderInfo GetRender(void);
	virtual PhysicsInfo GetPhysics(void);
	virtual CollisionInfo GetCollision(void);

	virtual void SetCollision(CollisionInfo info);
	virtual void SetLight(LightInfo info);
	virtual void SetMesh(MeshInfo info);
	virtual void SetRender(RenderInfo info);
	virtual void SetPhysics(PhysicsInfo info);

	virtual int getObjectID();


	virtual void Update(float deltaTime);
	virtual std::string ObjectType(void);
	//virtual bool iGameObject::operator<(const iGameObject& right);
	// Assumes that the imposter is rotating around the Y axis
	void UpdateImposterEye(glm::vec3 eye);

	// Allocates a "pool" of particles
	bool AllocateParticles(int numberOfParticles,
		const ParticleInfo &initSettings,
		bool bOverwriteExisting = true);

	// Return a COPY of the particles to be rendered. 
	// This is so this method can return very quickly and we aren't
	//  potentially "messing" with the actual vector of particles.
	// It's only "fast" if you pass a vector that's big enough and 
	//	has particles allocated for copying. 
	// +++Returns the number of particles we copied over.++
	int GetParticlesCopy(std::vector<cParticle> &vecParticles);

	//glm::vec3 position;

	// This is public and can be changed at any time. 
	// It is checked at each update. 
	// You might want to more tightly control this, if you'd like
	ParticleInfo particleSettings;
private:

	std::vector<cParticle> m_vecParticles;

	glm::vec3 CalcImposterRotationFromEye(glm::vec3 particlePos);

	// This is called when we want to "re-spawn" a particle.	
	// NOTE: We AREN'T instantiating a new particle, just recylcing
	void ResetParticle(cParticle& particle);

	// Returns INDEX of a particle is found, false if they are all "alive"
	// Returns -1 if no particle is found.
	int FindNextDeadParticle(void);

	int objectID;

	LightInfo lightInfo;
	MeshInfo meshInfo;
	RenderInfo renderInfo;
	PhysicsInfo physicsInfo;
	CollisionInfo collisionInfo;
};

#endif //_CPARTICLEEMITTER_HG_
