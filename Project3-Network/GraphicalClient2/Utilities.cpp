#include "Utilities.h"

#include <stdlib.h>     // srand, rand 

// http://stackoverflow.com/questions/686353/c-random-float-number-generation
float getRandFloat( float LO, float HI )
{
	float r3 = LO + 
		static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(HI-LO) ) );
	return r3;
}

float calcApproxDistFromAtten(float targetLightLevel, 
							  float attenConst, 
							  float attenLinear,
							  float attenQuad,
							  float accuracy, 
                              float infiniteDistance, 
							  unsigned int maxIterations /*= DEFAULTMAXITERATIONS = 50*/ )
{
	const float DEFAULTINFINITEDISTANCE = 10000.0f;	
//static 
	const float DEFAULDIFFUSEACCURACYTHRESHOLD = 0.001f;

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
	//if ( calcDiffuseFromAttenByDistance( DEFAULTINFINITEDISTANCE, accuracy ) > targetLightLevelHigh )
	float diffuseCalculated = calcDiffuseFromAttenByDistance( DEFAULTINFINITEDISTANCE, 
															  attenConst, attenLinear, attenQuad );
															  
	if ( diffuseCalculated > targetLightLevelHigh )
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
		float curDiffuseAtGuessDistance = calcDiffuseFromAttenByDistance( curDistanceGuess, 
																		  attenConst, 
																		  attenLinear, 
																		  attenQuad );
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

//const float CLightDescription::DEFAULTZEROTHRESHOLD = 0.0001f;

float calcDiffuseFromAttenByDistance(float distance, 
									 float attenConst, 
									 float attenLinear, 
									 float attenQuad, 
									 float zeroThreshold /*=0.0001f  DEFAULTZEROTHRESHOLD*/)
{
	const float DEFAULTZEROTHRESHOLD = 0.0001f;

	float diffuse = 1.0f;		// Assume full brightness

	float denominator = attenConst + 
	                    attenLinear * distance +
						attenQuad * distance * distance;
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