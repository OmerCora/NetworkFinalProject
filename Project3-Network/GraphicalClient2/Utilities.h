#ifndef _Utilities_HG_
#define _Utilities_HG_

float getRandFloat( float LO, float HI );

float calcApproxDistFromAtten(float targetLightLevel, 
							  float attenConst, 
							  float attenLinear,
							  float attenQuad,
							  float accuracy, 
                              float infiniteDistance, 
							  unsigned int maxIterations = 50 /*DEFAULTMAXITERATIONS*/ );

float calcDiffuseFromAttenByDistance(float distance, 
									 float attenConst, 
									 float attenLinear, 
									 float attenQuad, 
									 float zeroThreshold =0.0001f /*DEFAULTZEROTHRESHOLD*/);

#endif
