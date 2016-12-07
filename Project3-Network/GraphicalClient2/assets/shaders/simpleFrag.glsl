#version 400

// FRAGMENT shader

in vec3 diffuseColour;
in vec4 vertWorldXYZ;
in vec4 vertNorm;

uniform bool bUseObjColorNoTexture;

const int NUMBEROFLIGHTS = 1;

struct sLightDesc
{
	vec4 position;		// In the world
	vec4 diffuse;		// 1,1,1 (white colour light)
	vec4 attenuation;	// x = const, y = linear, z = quad, w = not used
	vec4 typeAndParams;	// To be used soon. Specify type and attributes of type
};

uniform sLightDesc theLights[NUMBEROFLIGHTS];

// For spec:
uniform vec3 eyeLocation;

out vec4 out_Colour;

// This was taken out of the main portion. Just the light calculation
vec3 calcLightContribution( vec3 diffuseObjectColour, int lightIndex )
{
	vec3 lightv = theLights[lightIndex].position.xyz - vertWorldXYZ.xyz;
	
	float distanceFromLight = length(lightv); 
//	float lightConstAtten = 0.0f;		// Can set to 1.0 to "turn off" a light
//	float lightLinAtten = 0.1f;
//	float lightQuadAtten = 0.01f;
//	
//	float linAtten = (1.0f/distanceFromLight) * lightLinAtten;  
//	
//	float quadAtten = (1.0f/(distanceFromLight*distanceFromLight))
//	                    * lightQuadAtten;

	float atten = 1.0f /( theLights[lightIndex].attenuation.x + 
	                      theLights[lightIndex].attenuation.y * distanceFromLight +
						  theLights[lightIndex].attenuation.z * distanceFromLight * distanceFromLight );
	
	lightv = normalize(lightv);
	
 	vec3 diffuse = max(0.0, dot(lightv, normalize(vertNorm.xyz))) 
                   * diffuseObjectColour
				   * theLights[lightIndex].diffuse.xyz;
				   	
	// Calculate the linear attenuation of the diffuse
//	diffuse.xyz *= linAtten;	
	diffuse *= atten;

	
	// With newer (we're using version 4 of GLSL) some of the 
	// 	built in variables are depricated, like gl_FragColor
	//gl_FragColor = vec4(diffuse, 1.0f);	

	return diffuse;
}

void main()
{
	// Check to see if we are going to do the lighthing calculation
	if ( bUseObjColorNoTexture )
	{	// Bypass lighting entirely and set to uniform "debug" diffuse
		out_Colour.xyz = diffuseColour.xyz;
		out_Colour.a = 1.0f;
		return;
	}

	// Do lighting calculation per light
	vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);
	for ( int index = 0; index != NUMBEROFLIGHTS; index++ )
	{
		diffuse += calcLightContribution( diffuseColour.xyz, index );
	}
	
	
	// Bump up the brightness (projector is too dark)
//	diffuse.xyz *= vec3(1.5f);
	
	out_Colour = vec4(diffuse, 1.0f);	
	
//	out_Colour *= 0.001f;	// make it (essentially) zero.	
//	out_Colour += vec4(myLightDiffuse.xyz, 1.0f);
}