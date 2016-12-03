#ifndef _IGAMEOBJECT_HG_
#define _IGAMEOBJECT_HG_

#include "infos.h"
#include <string>

class iGameObject
{
public:
	~iGameObject() {}

	virtual LightInfo GetLight(void) = 0;
	virtual CollisionInfo GetCollision(void) = 0;
	virtual MeshInfo GetMesh(void) = 0;
	virtual RenderInfo GetRender(void) = 0;
	virtual PhysicsInfo GetPhysics(void) = 0;

	virtual void SetLight(LightInfo info) = 0;
	virtual void SetCollision(CollisionInfo info) = 0;
	virtual void SetMesh(MeshInfo info) = 0;
	virtual void SetRender(RenderInfo info) = 0;
	virtual void SetPhysics(PhysicsInfo info) = 0;

	virtual int getObjectID(void) = 0;

	virtual void Update(float deltaTime) = 0;

	virtual std::string ObjectType(void) = 0;
	//virtual bool iGameObject::operator<(const iGameObject& right) = 0;
private:

};


#endif // !_IGAMEOBJECT_HG_

