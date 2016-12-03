#version 400

// FRAGMENT shader

uniform bool bUseDebugDiffuseNoLighting;

const int NUMBEROFLIGHTS = 10;

struct sLightDesc
{
	bool enabled;
	vec4 position;		// In the world
	vec4 ambient;		// 0,0,0 (no ambient)
	vec4 diffuse;		// 1,1,1 (white colour light)
	vec4 specular;		// w is specular power
	vec4 attenuation;	// x = const, y = linear, z = quad, w = specular ratio
	vec4 direction;		// For directional and spot (ignored by others)
	vec4 typeAndParams;	// To be used soon. Specify type and attributes of type
	// x = type: 0 = point, 1 = spot, 2 = directional, 3 =hemispherical 
						// y = for Spot: cos cutoff start (start of penumbra, edge of bright spot)
						// z = for Spot: cos cutoff end (end of penumbra, beyond this it's dark)
};

uniform sLightDesc theLights[NUMBEROFLIGHTS];

uniform bool useGlobalAlphaValue;
uniform float globalAlphaValue;
uniform float alpha;

// For spec:
uniform vec3 eyeLocation;


// Texture "sampler"
// note that this ISN'T a 'texture', they are in 'texture units'
uniform sampler2D texSamp2D_00;			// Location 12	
uniform sampler2D texSamp2D_01;		
uniform sampler2D texSamp2D_02;			// Location 12	
uniform sampler2D texSamp2D_03;

uniform samplerCube mycubemapSampler;
uniform samplerCube myCubemapSkyBoxSampler;
uniform bool bIsCubeMapObject;
uniform int usedTexNum;



// At least 80 texture units.
// Something like 16 to 32 on high end cards PER SHADER AT THE SAME TIME

uniform bool bUseTextureAsDiffuse;
uniform bool bUseLighting;
uniform bool bIsDiscardTextureBlack;
uniform bool bIsImposterBlackIsAlpha;


// In from the vertex shader
in vec4 diffuseColour;	
in vec3 vertMV;			// view vector
in vec3 vertWorld;
in vec3 vertNorm;
//in vec3 viewVector;
in vec3 lightVector[NUMBEROFLIGHTS];
in vec3 lightHalfVector[NUMBEROFLIGHTS];
in vec3 text_STU;	// ADDED, texture coordinates (also called UV)

in vec3 sb_texcoords;
in vec3 lightDirection[NUMBEROFLIGHTS];
in float attenuation[NUMBEROFLIGHTS];

// out to the framebuffer
out vec4 out_Colour;

vec3 calcLightContribution( vec3 diffuseObjectColour, vec3 vecView, int lightIndex )
{
	if ( !theLights[lightIndex].enabled )
	{
		//no contributuion
		return vec3(0.0f, 0.0f, 0.0f);
	}

	float diffuseVal = max(0.0f, dot(vertNorm.xyz, lightDirection[lightIndex]));
	float specularVal = max(0.0f, dot(vertNorm.xyz, lightHalfVector[lightIndex]));

	if (diffuseVal == 0.0f)
		specularVal = 0.0f;
	else
		specularVal = pow(specularVal,  theLights[lightIndex].specular.w) * 1.0f;

	vec3 diffuseVec = theLights[lightIndex].diffuse.xyz * diffuseVal;
	vec3 specularVec = theLights[lightIndex].specular.xyz * specularVal;
	
	diffuseVec *= attenuation[lightIndex];
	specularVec *= attenuation[lightIndex];		   	
	
	if (  int(theLights[lightIndex].typeAndParams.x) == 2)
	{
		//directional
		vec3 result = clamp((diffuseObjectColour*theLights[lightIndex].ambient.xyz) * (diffuseVec + specularVec), 0.0f, 1.0f);
		return result;
	}
	else
	{
		vec3 result = clamp(diffuseObjectColour * (diffuseVec + specularVec + theLights[lightIndex].ambient.xyz), 0.0f, 1.0f);
		return result;
	}
	
}

void main()
{
	// Check to see if we are going to do the lighthing calculation
	if ( bUseDebugDiffuseNoLighting)
	{	// Bypass lighting entirely and set to uniform "debug" diffuse
		out_Colour.xyz = diffuseColour.xyz;
		out_Colour.a = 1.0f;
		return;
	}

	// For the cube map
	if ( bIsCubeMapObject )
	{
		// Cube maps require 3 texture coordinates, specifically 
		//  a "ray" cast towards the sphere or cube.
		// We could calculate this, or simply use the normal
		//  from the object we're using as a "cube", which 
		//  in our case is actually a sphere.
		out_Colour = texture( myCubemapSkyBoxSampler, sb_texcoords); //vertNorm.xyz );
		out_Colour.a = 1.0f;
		return;
	}



	vec3 objectDiffuseColour = diffuseColour.xyz;

	float alphaOut = alpha;

	// are we using textures? 
	if ( bUseTextureAsDiffuse )
	{	// Replace the diffuse colour with what's in the texture
		vec3 diffuse0 = texture( texSamp2D_00, text_STU.xy ).rgb;
		vec3 diffuse1 = texture( texSamp2D_01, text_STU.xy ).rgb;
			
		vec3 diffuse2 = texture( texSamp2D_02, text_STU.xy ).rgb;	
		vec3 diffuse3 = texture( texSamp2D_03, text_STU.xy ).rgb; //alpha mask
		
		//diffuse1 = vec3(1.0f, 0.0f, 0.0f);
		if (usedTexNum == 0)
		{
			objectDiffuseColour = diffuse0;
		}
		else if (usedTexNum == 1)
		{
			objectDiffuseColour = mix(diffuse0, diffuse1, 0.5f);
		}
		else if (usedTexNum == 2)
		{
			objectDiffuseColour = mix(diffuse0, diffuse1, 0.5f);
			objectDiffuseColour = mix(objectDiffuseColour, diffuse2, 0.5f);
		}
		else if (usedTexNum == 3)
		{
			//objectDiffuseColour = diffuse3;
		}
		
		if (bIsDiscardTextureBlack)
		{
			float diffuseGray = (objectDiffuseColour.r + objectDiffuseColour.g + objectDiffuseColour.b) / 3.0f;
			if ( diffuseGray <= 0.01f ) 
			{
				discard;
			}
		}

		// here take the transparency directly from the image
		alphaOut = normalize(diffuse1.r + diffuse1.g + diffuse1.b );
	}



	if ( ! bUseLighting )
	{	// Don't perform the lighting calculation, just use existing diffuse
		out_Colour = vec4(objectDiffuseColour, alphaOut);	
	}
	else
	{	// Perform lighting calculation
		vec3 vecView = normalize(vertMV);
		
		// Do lighting calculation per light
		vec3 lightContrib = vec3(0.0f, 0.0f, 0.0f);
		for ( int index = 0; index != NUMBEROFLIGHTS; index++ )
		{
			lightContrib += calcLightContribution( objectDiffuseColour, vecView, index );
		}
		
		out_Colour = vec4(lightContrib, alphaOut);	
	}//if ( bUseLighting )


	if ( bIsImposterBlackIsAlpha )
	{
		// Treat anything that is close to "black" as transparent
		if (( (out_Colour.r + out_Colour.g + out_Colour.b)/3.0f) < 0.2f )
		{	// Fully transparent
			out_Colour.w = 0.0f;
		}
	}
	
	//out_Colour *= 1.20f;

	//vec3 ambientColour = vec3(0.14f,0.14f,0.14f) * out_Colour.xyz;
	//out_Colour = vec4(out_Colour.xyz + ambientColour, 1.0f);

///	//out_Colour *= 0.001f;
	
//	out_Colour += diffuseColour;
	
	if(useGlobalAlphaValue)
	{
		out_Colour.a = globalAlphaValue;
	}
	else
	{
		out_Colour.a = alphaOut;
	}

}