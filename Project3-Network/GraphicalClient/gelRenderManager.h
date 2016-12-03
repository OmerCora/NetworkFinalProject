#ifndef _GELRENDERMANAGER_HG_
#define _GELRENDERMANAGER_HG_

#include "iManager.h"
#include "iGameObject.h"

#include "cParticleEmitter.h"
#include <vector>

class gelRenderManager : iManager
{
public:
	//Singleton
	static gelRenderManager& sharedInstance()
	{
		static gelRenderManager instance;
		return instance;
	}

	int getAABBID(void);
	void DrawAABB(glm::vec3 position, glm::vec4 colour, float scale);
	virtual void Update(float deltaTime);
	void Setup(void);

	//~GelRenderManager() {}
private:
	gelRenderManager() {};
	gelRenderManager(gelRenderManager const&); //dont implement - to build
	void operator=(gelRenderManager const&); //dont implement -  to build

	//void DrawScene(void);
	void DrawGameObjects(void);
	void DrawParticle(cParticle particle);
	void DrawPlayer(void);
	void DrawGameObject(iGameObject* gameObject);
	void DrawSkyboxObject(void);
	void DrawParticleEmitter(cParticleEmitter* emitter); //which refers to all particles in an emitter

	//a more generic draw method
	void DrawWithInfos(RenderInfo render, MeshInfo mesh, CollisionInfo collision);

	iGameObject* aabb;
	int aabbID;

	float globalAlpha;
	bool useGlobalAlpha;
};


#endif // !_GELRENDERMANAGER_HG_