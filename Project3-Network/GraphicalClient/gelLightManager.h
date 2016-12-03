#ifndef _GELLIGHTMANAGER_HG_
#define _GELLIGHTMANAGER_HG_

#include "iManager.h"
#include "iGameObject.h"

#include <vector>

class gelLightManager : public iManager
{
public:
	//Singleton 
	static gelLightManager& sharedInstance()
	{
		static gelLightManager instance;
		return instance;
	}

	void SetupUniforms(GLuint programID);

	void LoadLights(void); //linear on game objects, take first x enabled light.

	virtual void Update(float deltaTime);

private:
	gelLightManager() {};
	gelLightManager(gelLightManager const&); //dont implement - to build
	void operator=(gelLightManager const&); //dont implement - to build

	GLuint programID;
	int numberOfLights; //in program
	std::vector<LightInfo> activeLights; //first \(numberOfLights) active number of light slots for shader.
	std::vector<LightInfo::UniformInfo> uniforms;
};

#endif // !_GELLIGHTMANAGER_HG_

// "I am sensing a pattern here." -Skitskurr the Weaver