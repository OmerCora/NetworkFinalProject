#ifndef _CANIMAL_HG_
#define _CANIMAL_HG_

#include "iGameObject.h"
#include "iWorldObject.h"

#include "FaunaInfo.h"

#include "iWorldObjectAssign.h"

class cAnimal : public iGameObject, public iWorldObject, public iAnimalAssign
{
public:
	cAnimal();
	~cAnimal();

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
	virtual std::string ObjectType(void);
	virtual std::string GetObjectType(void);
	virtual glm::vec3 GetPosition(void);
	virtual void ReceiveMessage(std::string msg, float data);
	virtual bool AskQuestion(std::string msg, float &data);
	virtual void Update(float deltaTime);
	virtual void Tick(float deltaTime);

	virtual void AssignFaunaInfo(FaunaInfo info);

	//virtual std::string GetObjectType(void); //get object name
private:
	int objectID;

	LightInfo lightInfo;
	MeshInfo meshInfo;
	RenderInfo renderInfo;
	PhysicsInfo physicsInfo;
	CollisionInfo collisionInfo;

	FaunaInfo faunaInfo;

	glm::vec3 initialLocation;
	glm::vec3 desiredLocation;
	float interpolationTime;
	float acc_interpolationTime;
};

#endif // !_CANIMAL_HG_
