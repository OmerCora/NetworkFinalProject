#ifndef _CPLAYER_HG_
#define _CPLAYER_HG_

#include <string>

#include "infos.h"
#include "GelStructures.h"
#include "iGameObject.h"


#include "commands.h"

class cPlayer : public iGameObject
{
public:
	cPlayer();
	~cPlayer();

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
	int mMonstersConsumed;

	LightInfo lightInfo;
	MeshInfo meshInfo;
	RenderInfo renderInfo;
	PhysicsInfo physicsInfo;
	CollisionInfo collisionInfo;

	bool moveForward;
	//cCommandGroup cmdGrp;
};


#endif // !_CPLAYER_HG_
