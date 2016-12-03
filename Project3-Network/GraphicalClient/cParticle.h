#ifndef _CPARTICLE_HG_
#define _CPARTICLE_HG_

#include "string"
#include "infos.h"
#include "GelStructures.h"
//#include "iGameObject.h"

class cParticle //: public iGameObject
{
public:
	cParticle();
	~cParticle();

	bool discardTextureBlack;
	float life;
	float scale;
	glm::vec3 position;
	glm::vec3 rotation; //remove ?
	glm::quat orientation;
	glm::vec4 colour;	// RGBA
	glm::vec3 velocity;
	glm::vec3 acceleration;
	std::string meshName;
	std::string textureName_0;
	//virtual LightInfo GetLight(void);
	//virtual MeshInfo GetMesh(void);
	//virtual RenderInfo GetRender(void);
	//virtual PhysicsInfo GetPhysics(void);

	//virtual void SetLight(LightInfo info);
	//virtual void SetMesh(MeshInfo info);
	//virtual void SetRender(RenderInfo info);
	//virtual void SetPhysics(PhysicsInfo info);

	//virtual int getObjectID();

	//virtual void Update(float deltaTime);
	void Update(float deltaTime);

	//void SetLife(float lifePt);
	//float GetLife(void);
	//float DecayLife(float deltaTime);

private:
	//int objectID;//not used
	//float life;

	//LightInfo lightInfo;
	//MeshInfo meshInfo;
	//RenderInfo renderInfo;
	//PhysicsInfo physicsInfo;
};


#endif // !_CPARTICLE_HG_

//
//
//class cParticle
//{
//public:
//	cParticle() : life(0.0f), colour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {};
//	glm::vec3 position;
//	float life;
//	glm::vec3 rotation;
//	glm::vec4 colour;	// RGBA
//	glm::vec3 velocity;
//	glm::vec3 acceleration;
//	// Anything else you might want
//};