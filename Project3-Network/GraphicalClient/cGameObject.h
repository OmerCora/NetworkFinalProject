#ifndef _CGAMEOBJECT_HG_
#define _CGAMEOBJECT_HG_

#include <string>

#include "infos.h"
#include "GelStructures.h"
#include "iGameObject.h"

class cGameObject : public iGameObject
{
public:
	cGameObject();
	~cGameObject();

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
	PhysicsInfo physicsInfo; //TOTHINK: delegate of a GelPhysicsManager maybe in the future?
	CollisionInfo collisionInfo;
};


#endif // !_CGAMEOBJECT_HG_

