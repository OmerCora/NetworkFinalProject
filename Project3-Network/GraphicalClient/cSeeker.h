#ifndef _CSEEKER_HG_
#define _CSEEKER_HG_

#include <string>

#include "infos.h"
#include "GelStructures.h"
#include "iGameObject.h"

#include "iGameObjectAssign.h"

class cSeeker : public iGameObject, public iGameObjectAssign
{
public:
	cSeeker();
	~cSeeker();

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
	virtual void AssignMoodAndSpeed(std::string newMood, float averageSpeed);
	//virtual bool iGameObject::operator<(const iGameObject& right);
private:
	int objectID;

	LightInfo lightInfo;
	MeshInfo meshInfo;
	RenderInfo renderInfo;
	PhysicsInfo physicsInfo;
	CollisionInfo collisionInfo;

	enum MOOD
	{
		DEFAULT,  //project 1 behaviour pattern
		ANGRY,   //pursuit
		CURIOUS, //arrive
		BIPOLAR  //change mood at intervals
	};
	MOOD currentMood;

	enum BEHAVIOUR
	{
		RUN,     //run directly
		EVADE,   //run away considering player direction
		SEEK,    //chase directly
		PURSUIT, //chase considering player direction
		ARRIVE,  //scale speed so that it slows and stops at a "too close" distance
		REST,    //dont move
		WANDER   //occasionally pick a location randomly and go there 
	};
	BEHAVIOUR currentBehaviour;
	void Behave(float deltaTime);

	//for wander
	float wanderDuration; //1.0f
	float wanderAccumulator;
	float random_x;
	float random_z;

	bool drawAttentionRadius;
	float attentionRadius; //decide to arrive or pursuit depending mood, if player within attentionRadius
	float dangerRadius; //stride backwards to avoid staying this close to the player at any time

	float focus; //resource for decision
	float logicalCooldown; //cooldown for decision
	int intelligenceModifier; //shorter decision interval
	int dexterityModifier;    //higher velocity
	
	float speedK; //speed vec multiplication mod
};

#endif // !_CSEEKR_HG_
