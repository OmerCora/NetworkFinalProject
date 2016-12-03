#ifndef _CPLANT_HG_
#define _CPLANT_HG_

#include "iGameObject.h"
#include "iWorldObject.h"
#include "FloraInfo.h"

#include "iWorldObjectAssign.h"

class cPlant : public iGameObject, public  iWorldObject, public iPlantAssign
{
public:
	cPlant();
	~cPlant();

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
	virtual void Tick(float deltaTime);
	virtual glm::vec3 GetPosition(void);
	virtual std::string ObjectType(void);
	virtual std::string GetObjectType(void); //get object name
	virtual void ReceiveMessage(std::string msg, float data);
	virtual bool AskQuestion(std::string msg, float &data);

	virtual void AssignFloraInfo(FloraInfo info);

private:
	int objectID;

	LightInfo lightInfo;
	MeshInfo meshInfo;
	RenderInfo renderInfo;
	PhysicsInfo physicsInfo;
	CollisionInfo collisionInfo;

	FloraInfo floraInfo;



};

#endif // !_CANIMAL_HG_
