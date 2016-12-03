#include "LightInfo.h"

#include "cGLColourHelper.h"

#include <sstream>		// For the GetLightInfoString() method
#include <iomanip>		// For the GetLightInfoString() method

void LightInfo::SetSpotLightCutOff(float minVal, float maxVal)
{
	this->lightType = SPOT;
	this->cosCutOffMin = minVal;
	this->cosCutOffMax = maxVal;
	this->typeAndParams.y = minVal;
	this->typeAndParams.z = maxVal;
}

void LightInfo::SetLightType(LightInfo::LightType type)
{
	this->lightType = type;
	if (type == POINT)
	{
		this->typeAndParams.x = 0;
	}
	else if (type == SPOT)
	{
		this->typeAndParams.x = 1;
	}
	else if (type == DIRECTIONAL)
	{
		this->typeAndParams.x = 2;
	}
	else if (type == HEMISPHERE)
	{
		this->typeAndParams.x = 3;
	}
}

LightInfo::LightInfo()
{
	this->lightType = POINT;
	this->cosCutOffMax = 0.0f;
	this->cosCutOffMin = 0.0f;

	this->position = glm::vec3( 0.0f, 0.0f, 0.0f );	// Likely not needed as vec3 c'tor sets to zeros

	// Set to white by default... I guess?
	this->diffuse = cGLColourHelper::getInstance()->getColourRGB( cGLColourHelper::COLOUR_WHITE );
	this->ambient = this->diffuse * this->ambientToDiffuseRatio;
	//this->specular = this->diffuse;	// White highlights
	this->attenConst = this->DEFAULT_ATTEN_CONST;
	this->attenLinear = this->DEFAULT_ATTEN_LINEAR;
	this->attenQuad = this->DEFAULT_ATTEN_QUADRATIC;

	this->ambientToDiffuseRatio = LightInfo::DEFAULT_AMBIENT_TO_DIFFUSE_RATIO;

	this->bIsEnabled = false;
	this->bDrawAttenuationSpheres = false;
	this->ownerObjectID = -1; //not set

	// -1 is an invalid location for OpenGL: see glGetUniformLocation()
	this->uniformInfo.UniformLocation_enabled = GL_INVALID_VALUE;
	this->uniformInfo.UniformLocation_position = GL_INVALID_VALUE;
	this->uniformInfo.UniformLocation_ambient = GL_INVALID_VALUE;
	this->uniformInfo.UniformLocation_diffuse = GL_INVALID_VALUE;
	this->uniformInfo.UniformLocation_specular = GL_INVALID_VALUE;
	this->uniformInfo.UniformLocation_attenuation = GL_INVALID_VALUE;
	//this->uniformInfo.UniformLocation_attenConst = GL_INVALID_VALUE;
	//this->uniformInfo.UniformLocation_attenLinear = GL_INVALID_VALUE;
	//this->uniformInfo.UniformLocation_attenQuad = GL_INVALID_VALUE;
	
	this->TurnOff();

	return;
}

void LightInfo::SetToDefaultWhite(void)
{
	this->diffuse = cGLColourHelper::getInstance()->getColourRGB( cGLColourHelper::COLOUR_WHITE );
	this->ambient = this->diffuse * this->ambientToDiffuseRatio;
	//this->specular = this->diffuse;	// White highlights
	this->attenConst = this->DEFAULT_ATTEN_CONST;
	this->attenLinear = this->DEFAULT_ATTEN_LINEAR;
	this->attenQuad = this->DEFAULT_ATTEN_QUADRATIC;
	return;
}

void LightInfo::SetRandomColour(void)
{
	this->diffuse = cGLColourHelper::getInstance()->getRandomColourRGB();
	this->ambient = this->diffuse * this->ambientToDiffuseRatio;
	//this->specular = this->diffuse;	// White highlights
	return;
}

void LightInfo::CopyLight(LightInfo &lightToCopy)
{
	this->position    = lightToCopy.position;
	this->diffuse     = lightToCopy.diffuse;
	this->ambient     = lightToCopy.ambient;
	this->specular    = lightToCopy.specular;
	this->attenConst  = lightToCopy.attenConst;
	this->attenLinear = lightToCopy.attenLinear;
	this->attenQuad   = lightToCopy.attenQuad;

	this->typeAndParams = lightToCopy.typeAndParams;
	this->direction = lightToCopy.direction;

	this->ambientToDiffuseRatio = lightToCopy.ambientToDiffuseRatio;

	this->bIsEnabled = lightToCopy.bIsEnabled;
	this->bDrawAttenuationSpheres = lightToCopy.bDrawAttenuationSpheres; 

	//no, lets not do that for a sec.
	//this->uniformInfo.UniformLocation_position = lightToCopy.uniformInfo.UniformLocation_position;
	//this->uniformInfo.UniformLocation_ambient = lightToCopy.uniformInfo.UniformLocation_ambient;
	//this->uniformInfo.UniformLocation_diffuse = lightToCopy.uniformInfo.UniformLocation_diffuse;
	//this->uniformInfo.UniformLocation_specular = lightToCopy.uniformInfo.UniformLocation_specular;
	//this->uniformInfo.UniformLocation_attenuation = lightToCopy.uniformInfo.UniformLocation_attenuation;
	//this->uniformInfo.UniformLocation_typeAndParams = lightToCopy.uniformInfo.UniformLocation_typeAndParams;
	//this->uniformInfo.UniformLocation_direction = lightToCopy.uniformInfo.UniformLocation_direction;
}

void LightInfo::SetToBlack(void)
{
	this->diffuse = cGLColourHelper::getInstance()->getColourRGB( cGLColourHelper::COLOUR_BLACK );
	this->ambient = cGLColourHelper::getInstance()->getColourRGB( cGLColourHelper::COLOUR_BLACK );
	//this->specular = this->diffuse;	// White highlights
	//this->attenConst = 0.1f;
	//this->attenLinear = 0.1f;
	//this->attenQuad = 0.1f;
	return;
}

void LightInfo::TurnOff(void)		//	SaveOldColoursSetToBlack();
{
	this->old_ambient = this->ambient;
	this->old_diffuse = this->diffuse;
	this->old_specular = this->specular;
	this->SetToBlack();
	return;
}

void LightInfo::TurnOn(void)		//  RestoreOldColours();
{
	this->ambient = this->old_ambient;
	this->diffuse = this->old_diffuse;
	this->specular = this->old_specular;
	this->bIsEnabled = true;
	return;
}

//static 
const float LightInfo::DEFAULT_ATTEN_CONST = 0.1f;

//static 
const float LightInfo::DEFAULT_ATTEN_LINEAR = 0.1f;

//static 
const float LightInfo::DEFAULT_ATTEN_QUADRATIC = 0.1f;

//static
const float LightInfo::DEFAULT_AMBIENT_TO_DIFFUSE_RATIO = 0.2f;



std::string LightInfo::GetLightInfoString(void)
{

	std::stringstream ss;
	ss << std::fixed << std::setprecision(3)			// Fixed precision and 3 decimal places
		<< " pos: " << this->position.x << ", " << this->position.y << ", " << this->position.z
		<< " attenConst: " << this->attenConst
		<< " attenLin: " << this->attenLinear
		<< " attenQuad: " << this->attenQuad;
	return ss.str();
}

//static 
const float LightInfo::DEFAULTINFINITEDISTANCE = 10000.0f;	
//static 
const float LightInfo::DEFAULDIFFUSEACCURACYTHRESHOLD = 0.001f;

float LightInfo::calcApproxDistFromAtten( float targetLightLevel )	// Uses the defaults
{
	return this->calcApproxDistFromAtten( targetLightLevel, DEFAULDIFFUSEACCURACYTHRESHOLD );
}

float LightInfo::calcApproxDistFromAtten(float targetLightLevel, float accuracy )	// Uses the defaults
{
	return this->calcApproxDistFromAtten( targetLightLevel, accuracy, DEFAULTINFINITEDISTANCE, DEFAULTMAXITERATIONS );
}
                                                      
float LightInfo::calcApproxDistFromAtten(float targetLightLevel, float accuracy, float infiniteDistance, unsigned int maxIterations /*= DEFAULTMAXITERATIONS = 50*/ )
{
	// See if the accuracy being set it too big for the targetLightLevel, unless targetLightLevel is actually zero (0.0f)
	// If it's actually zero, then adjusting the accuracy to a tenth of zero would give... wait for it...
	//	zero, and we would max out the iterations
	if ( targetLightLevel != 0.0f )	
	{	
		if ( ( accuracy * 10.0f) >= targetLightLevel * 10.0f )
		{	// Adjust the accuracy by a hundreth
			accuracy = targetLightLevel / 10.0f;
		}
	}//if ( targetLightLevel != 0.0f )	

	float targetLightLevelLow = targetLightLevel - accuracy;
	//if ( targetLightLevelLow <= 0.0f )	{ targetLightLevel = 0.0f; }
	float targetLightLevelHigh = targetLightLevel + accuracy;

	// See if we're getting a value at infinite. i.e. at 'infinite distance', is the light level too high already
	if ( this->calcDiffuseFromAttenByDistance( DEFAULTINFINITEDISTANCE, accuracy ) > targetLightLevelHigh )
	{	// Yes, so we can never get down to this light level
		return DEFAULTINFINITEDISTANCE;
	}

	// There is a light level somewhere between a distance of 0.0 to DEFAULTINFINITEDISTANCE
	// Set initial guesses
	float distanceGuessLow = 0.0f;
	float distanceGuessHigh = DEFAULTINFINITEDISTANCE;

	unsigned int iterationCount = 0;
	while ( iterationCount < maxIterations )
	{
		// Pick a distance between the high and low
		float curDistanceGuess = ( (distanceGuessHigh - distanceGuessLow) / 2.0f ) + distanceGuessLow;
		// 
		float curDiffuseAtGuessDistance = this->calcDiffuseFromAttenByDistance( curDistanceGuess, DEFAULTZEROTHRESHOLD );
		// Could be three possibilities: too low, too high, or in between
		if ( curDiffuseAtGuessDistance < targetLightLevelLow )
		{	// Light is too dark, so distance is to HIGH. Reduce and guess again.
			distanceGuessHigh = curDistanceGuess;		// Lower the high limit for the guesses
		}
		else if ( curDiffuseAtGuessDistance > targetLightLevelHigh )
		{	// Light is too bright, so distance is to LOW. Increase and guess again
			distanceGuessLow = curDistanceGuess;
		}
		else 
		{	// Nailed it! Light level is within range, so return this distance
			return curDistanceGuess;
		}

		iterationCount++;

	}// while ( iterationCount < maxIterations )
	// If we are here, then we ran out of iterations.
	// Pick a distance between the low and high
	float distance = (distanceGuessHigh - distanceGuessLow) / 2.0f;

	return distance;
}

//static 
const float LightInfo::DEFAULTZEROTHRESHOLD = 0.0001f;
float LightInfo::calcDiffuseFromAttenByDistance(float distance, float zeroThreshold /*= DEFAULTZEROTHRESHOLD*/)
{
	float diffuse = 1.0f;		// Assume full brightness

	float denominator = this->attenConst + 
	                    this->attenLinear * distance +
						this->attenQuad * distance * distance;
	// Is this zero (we don't want a divide by zero, do we)?
	if ( denominator <= zeroThreshold )
	{	// Let's just say it's zero, shall we?
		diffuse = 1.0f;
	}
	else
	{
		float atten = 1.0f / denominator;
		diffuse *= atten;
		if ( diffuse > 1.0f )
		{
			diffuse = 1.0f;
		}
	}//if ( denominator <= zeroThreshold )
	return diffuse;
}