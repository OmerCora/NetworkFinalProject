#ifndef _CLightDescription_HG_
#define _CLightDescription_HG_

#include "externals.h"
//#include <glm/glm.hpp>		// because we are using the glm:vec3 classes
//#include <GL/glew.h>		// for the GL_INVALID_VALUE (which is -1) and GLint
#include <string>


// NOTE: this has NOTHING to do with what's in the shader
// Keep in mind that we eventually HAVE to set the uniform variables
//	inside the shader for the lights to actually work. 
// But, here we an "easy to use" (or easier, anyway) structure 
//	that can hold the information for the lights. 
class LightInfo
{
public:
	enum LightType
	{
		POINT,
		SPOT,
		DIRECTIONAL,
		HEMISPHERE
	};

	static const float DEFAULT_ATTEN_CONST;					// = 0.1f;
	static const float DEFAULT_ATTEN_LINEAR;				// = 0.1f;
	static const float DEFAULT_ATTEN_QUADRATIC;				// = 0.1f;
	static const float DEFAULT_AMBIENT_TO_DIFFUSE_RATIO;	// = 0.2f;

	bool bIsEnabled;
	bool bDrawAttenuationSpheres;

	int ownerObjectID; //id of the iGameObject conformer that owns this light, use this to update position with attached object's position
	//the ownerObjectID of `iGameObject`s' `LightInfo`s are not used, only the active lights' ownerObjectID is relevant for us.

	glm::vec3 position;
	glm::vec3 ambient;	
	glm::vec3 diffuse;
	glm::vec4 specular;
	glm::vec4 direction;
	glm::vec4 typeAndParams;

	glm::vec3 old_ambient;	
	glm::vec3 old_diffuse;
	glm::vec4 old_specular;	

	float cosCutOffMin;//for spotlight
	float cosCutOffMax;//for spotlight

	LightType lightType;

	float attenConst;	// = 0.1f;
	float attenLinear;	// = 0.1f;
	float attenQuad;	// = 0.1f;

	float ambientToDiffuseRatio;	// Used when we set the colours
	void SetSpotLightCutOff(float minVal, float maxVal);
	void SetLightType(LightInfo::LightType type);

	struct UniformInfo
	{
		UniformInfo() :
			UniformLocation_enabled(-2),
			UniformLocation_position(-2),
			UniformLocation_ambient(-2),
			UniformLocation_diffuse(-2),
			UniformLocation_specular(-2),
			UniformLocation_attenuation(-2),
			UniformLocation_typeAndParams(-2), 
			UniformLocation_direction(-2) {}

		GLint UniformLocation_enabled;
		GLint UniformLocation_position;
		GLint UniformLocation_ambient;
		GLint UniformLocation_diffuse;
		GLint UniformLocation_specular;
		GLint UniformLocation_attenuation;
		//added
		GLint UniformLocation_typeAndParams;
		GLint UniformLocation_direction;
		//we only need one vec4 for all later merge the data on the class too
		//GLint UniformLocation_attenConst;
		//GLint UniformLocation_attenLinear;
		//GLint UniformLocation_attenQuad;
	};

	UniformInfo uniformInfo;

	LightInfo();

	void TurnOff(void);		//	SaveOldColoursSetToBlack();
	void TurnOn(void);		//  RestoreOldColours();

	void SetToDefaultWhite(void);	
	void SetToBlack(void);
	void SetRandomColour(void);
	void CopyLight(LightInfo &lightToCopy);
	std::string GetLightInfoString(void);
	// ... and so on...

	// This is a helper function that returns a light "brightness" level
	//	based on the attenuation functions. It's closely tied to the shader
	//	so if the shader changes, you'd have to change this, too
	// The idea is that you can approximate how far an object would have 
	//	to be from the light for a given lightLevel (from 0.0 to 1.0), 
	//	within a certain accuracy. 
	// Accuracy could be something like 1% or 0.01f.
	// infiniteDistance is where there's no point in continuing the calculation. 
	//	i.e. if we still haven't hit the desired light level by infiniteDistance, it will reutrn infiniteDistance
	static const unsigned int DEFAULTMAXITERATIONS = 50;
	static const float DEFAULTINFINITEDISTANCE;	// = 10,000.0f;	
	static const float DEFAULDIFFUSEACCURACYTHRESHOLD; // = 0.001f;
	float calcApproxDistFromAtten( float targetLightLevel );	// Uses the defaults
	float calcApproxDistFromAtten( float targetLightLevel, float accuracy );	// Uses the defaults
	float calcApproxDistFromAtten( float targetLightLevel, float accuracy, float infiniteDistance, unsigned int maxIterations = DEFAULTMAXITERATIONS);
	static const float DEFAULTZEROTHRESHOLD;	// = 0.0001f;
	// If value gets below the "zeroThreshold", will be treated as zero
	//float calcAttenFromDist(float distance, float zeroThreshold = DEFAULTZEROTHRESHOLD);
	float calcDiffuseFromAttenByDistance(float distance, float zeroThreshold = DEFAULTZEROTHRESHOLD);

};

#endif