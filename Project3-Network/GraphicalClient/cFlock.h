#ifndef _CFLOCK_HG_
#define _CFLOCK_HG_

#include <string>

#include "infos.h"
#include "GelStructures.h"
#include "iGameObject.h"

#include "iGameObjectAssign.h"

class cFlock : public iGameObject
{
public:
	cFlock();
	cFlock(int flockPopulation, glm::vec3 startingPoint);
	~cFlock();

	void GetMembers(std::vector<iGameObject*> &flockMembers);

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
private:
	int objectID;

	LightInfo lightInfo;
	MeshInfo meshInfo;
	RenderInfo renderInfo;
	PhysicsInfo physicsInfo;
	CollisionInfo collisionInfo;

	float separationWeight;
	float alignmentWeight;
	float cohesionWeight;

	float radiusSeparation;
	float radiusNeighbour;

	float speedMultiplier;

	std::vector<iGameObject*> members;

	void GetSeparationVector(iGameObject* flockMember, glm::vec3 &separationVector);
	void GetCohesionVector(iGameObject* flockMember, glm::vec3 &cohesionVector);
	void GetAlignmentVector(iGameObject* flockMember, glm::vec3 &alingmentVector);

};

#endif // !_CSEEKR_HG_
