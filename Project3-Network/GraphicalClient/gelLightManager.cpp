#include "gelLightManager.h"
#include "GelsenizeCore.h"
#include "managers.h"

#include <sstream>

void gelLightManager::SetupUniforms(GLuint programID)
{
	this->programID = programID;
	numberOfLights = 10;

	//generate uniform variables
	uniforms.clear();
	uniforms.reserve(numberOfLights);

	for (int i = 0; i < numberOfLights; i++)
	{
		uniforms.push_back(LightInfo::UniformInfo());

		std::ostringstream ss;
		ss << "theLights[" << i << "].";
		std::string base = ss.str();

		std::string enabled     = base + "enabled";
		std::string position    = base + "position";
		std::string ambient     = base + "ambient";
		std::string diffuse     = base + "diffuse";
		std::string specular    = base + "specular";
		std::string attenuation = base + "attenuation";
		std::string typeAndParams = base + "typeAndParams";
		std::string direction = base + "direction";
		//not using the last one yet

		uniforms[i].UniformLocation_enabled     = glGetUniformLocation(programID, enabled.c_str());
		uniforms[i].UniformLocation_position    = glGetUniformLocation(programID, position.c_str());
		uniforms[i].UniformLocation_ambient     = glGetUniformLocation(programID, ambient.c_str());
		uniforms[i].UniformLocation_diffuse     = glGetUniformLocation(programID, diffuse.c_str());
		uniforms[i].UniformLocation_specular    = glGetUniformLocation(programID, specular.c_str());
		uniforms[i].UniformLocation_attenuation = glGetUniformLocation(programID, attenuation.c_str());
		uniforms[i].UniformLocation_typeAndParams = glGetUniformLocation(programID, typeAndParams.c_str());
		uniforms[i].UniformLocation_direction   = glGetUniformLocation(programID, direction.c_str());
	}
	int stop = 0; //uniform test point
	
}

//Load after reading game file
void gelLightManager::LoadLights(void)
{
	//turn off and clear all active lights
	activeLights.clear();
	activeLights.reserve(numberOfLights);

	//fill with empty lights
	for (int index = 0; index != numberOfLights; index++)
	{
		activeLights.push_back(LightInfo());
	}

	//now connect these empty lights to iGameObjects and turn them on if the associated iGameObject is a light source

	//get first \(numberOfLights) gameObject's active light
	std::vector<iGameObject*> gameObjects = gelObjectManager::sharedInstance().GetGameObjects();
	int assignedLightCount = 0;

	for (int index = 0; index != gameObjects.size(); index++)
	{
		if (assignedLightCount >= numberOfLights)
		{
			break; //all slots are full
		}

		iGameObject* gameObject = gameObjects[index];
		LightInfo light = gameObject->GetLight();

		//is the object a light source?
		if (light.bIsEnabled)
		{
			/*
				1- attach light to object (weak reference, care, hmm) by objectID
				2- copy light properties    
				3- set uniforms
			*/
			activeLights[assignedLightCount].ownerObjectID = gameObject->getObjectID();
			activeLights[assignedLightCount].CopyLight(light);
			activeLights[assignedLightCount].uniformInfo = uniforms[assignedLightCount];

			assignedLightCount++;
		}
	}

}

void gelLightManager::Update(float deltaTime)
{
	for (int index = 0; index != activeLights.size(); index++)
	{
		LightInfo light = activeLights[index];

		if (light.bIsEnabled)
		{
			//update active light owner object found
			//right now only get position updates from
			iGameObject* attachedObject = gelObjectManager::sharedInstance().FindGameObjectWithID(light.ownerObjectID);
			if (attachedObject != 0)
			{
				//found attached object, update position
				RenderInfo render = attachedObject->GetRender();
				LightInfo objectLight = attachedObject->GetLight();
				objectLight.position.x = render.position.x;
				objectLight.position.y = render.position.y;
				objectLight.position.z = render.position.z;

				light.CopyLight(objectLight); //only updates light props
			}
		}

		//need to make these
		glUniform1i(light.uniformInfo.UniformLocation_enabled, light.bIsEnabled);
		glUniform4f(light.uniformInfo.UniformLocation_position, light.position.x, light.position.y, light.position.z, 1.0f);
		glUniform4f(light.uniformInfo.UniformLocation_ambient, light.ambient.x, light.ambient.y, light.ambient.z, 1.0f);
		glUniform4f(light.uniformInfo.UniformLocation_diffuse, light.diffuse.x, light.diffuse.y, light.diffuse.z, 1.0f);
		glUniform4f(light.uniformInfo.UniformLocation_specular, light.specular.x, light.specular.y, light.specular.z, light.specular.w);
		glUniform4f(light.uniformInfo.UniformLocation_attenuation, light.attenConst, light.attenLinear, light.attenQuad, 0.0f);
		//added
		glUniform4f(light.uniformInfo.UniformLocation_typeAndParams, light.typeAndParams.x, light.typeAndParams.y, light.typeAndParams.z, light.typeAndParams.w);
		glUniform4f(light.uniformInfo.UniformLocation_direction, light.direction.x, light.direction.y, light.direction.z, 0.0f);
	}

}
