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
	vec4 typeAndParams;	// To be used soon. Specify type and attributes of type
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



// At least 80 texture units.
// Something like 16 to 32 on high end cards PER SHADER AT THE SAME TIME

uniform bool bUseTextureAsDiffuse;
uniform bool bUseLighting;
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

// out to the framebuffer
out vec4 out_Colour;

// This was taken out of the main portion. Just the light calculation
vec3 calcLightContribution( vec3 diffuseObjectColour, vec3 vecView, int lightIndex )
{
	if ( !theLights[lightIndex].enabled )
	{
		//no contributuion
		return vec3(0.0f, 0.0f, 0.0f);
	}


	// Can be used as an early exit for the shader
	// (if you add a "distance" value to each light, the distance 
	//  being how far the light has any influence. You'd set this to 
	//  a place where the light is essentially attenuated to "zero")
	float distanceFromLight = length(lightVector[lightIndex].xyz); 
	// Option: Add if statement to early exit if distance is "too far"
	
	// The final attenuation is calculated. This is done here so 
	//  that you can also, optionally, early exit, say if the 
	//  attenuation is something very high (making the light 
	//  contribution essentially zero).
	float atten = 1.0f /( theLights[lightIndex].attenuation.x + 
	                      theLights[lightIndex].attenuation.y * distanceFromLight +
						  theLights[lightIndex].attenuation.z * distanceFromLight * distanceFromLight );

	
	// Normalize the light vector (which is the ray from the 
	//  vertex (world space) to the light (world space).
	// Since it's normalized, we only care about the
	//  direction, so essentially this is like a ray 
	//  cast from the origin (like the "normal" vector).
	vec3 lightVec = normalize(lightVector[lightIndex].xyz);
	
	// Diffuse contribution is the amount of light being reflected 
	//  (++INDEPENDENT of the eye (view) location++)
	//  from the LIGHT and the normal. Recal:
	//  - "lightVector" is ray from vertex (world space) to light (world space), normalized
	//  - "vertNormal" is normal of vertex (world space) with no translation
	// So both vectors are effectively coming from the origin (are only directions)
 	vec3 diffuse = max(0.0, dot(vertNorm.xyz, lightVec)) * diffuseObjectColour * theLights[lightIndex].diffuse.xyz;
				   	
	diffuse *= atten;
	
	// Specular: Phong
	// We do a simliar thing to the diffuse, but this time we are 
	//  seeing how much light is being reflected into THE EYE.
	// With diffuse, we don't care about the eye, as the colour
	//  we get is the same from every angle - it's only altered
	//  based on the light and normal position. 
	// The "most" light is reflected when the eye is directly
	//  in the path of the reflection vector, so let's calculate 
	//  that, first:
	//
	// The "view", "eye", or "camera" vector is the ray cast from 
	// 	the vertex, in "world" space, to the eye (also in "world" space).
	vec3 viewVector = normalize(eyeLocation - vertWorld);
	// The "light vector" is the ray from the vertex to the light.
	// (we've already calculated that)
	// 
	//  lightVector[index].xyz = theLights[index].position.xyz - vertWorld.xyz; (in VS)
	//  ---and---
	//	vec3 lightVec = normalize(lightVector[lightIndex].xyz); (FS: line 60)
	// 
	// Calculate the reflection vector from the light around normal:
	vec3 vecReflect = normalize(reflect(-lightVec,vertNorm));
	// The closer the angle between the eye and the vertex (the "viewVector") 
	//  and this reflection vector, the "brighter" the light is.
	//
	float specReflect = max(dot(vecReflect, viewVector), 0.0f);	// Silimar to diffuse, but uses view
	
	float lightSpecShinniness = theLights[lightIndex].specular.w;
	lightSpecShinniness = 1000.0f;
	float specPower = pow( specReflect, lightSpecShinniness );
	vec3 specular = specPower * theLights[lightIndex].specular.xyz;
	//vec3 specular = specPower * vec3(1.0f, 1.0f, 1.0f);
	specular *= atten;
	specular = clamp(specular, 0.0f, 1.0f);

	

//	return diffuse;
//	return specular;
//	return specReflect * vec3(1.0f, 1.0f, 1.0f);
//	return specReflect * vec3(1.0f, 1.0f, 1.0f) * atten;
	
	vec3 result = clamp(diffuse + specular, 0.0, 1.0f);
	
	result += theLights[lightIndex].ambient.xyz * result;

	return result;
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
//		objectDiffuseColour = texture( texSamp2D_00, text_STU.xy ).rgb;
		
		vec3 diffuse0 = texture( texSamp2D_00, text_STU.xy ).rgb;
		vec3 diffuse1 = texture( texSamp2D_01, text_STU.xy ).rgb;
			
		vec3 diffuse2 = texture( texSamp2D_02, text_STU.xy ).rgb;	// Fench
		vec3 diffuse3 = texture( texSamp2D_03, text_STU.xy ).rgb;	// Fench alpha
		
		//diffuse1 = vec3(1.0f, 0.0f, 0.0f);
		objectDiffuseColour = diffuse1;		// Alpha value for the fence
		
//		if ( diffuse1.r < 0.1f ) 
//		{
///			discard;
//		}
		// here take the transparency directly from the image
		alphaOut = normalize(diffuse1.r + diffuse1.g + diffuse1.b );

//		diffuse0 *= 1.0f;
//		diffuse1 *= 1.0f;
		
//		objectDiffuseColour = mix(diffuse0, diffuse1, 0.5f);
		
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
			//lightContrib += calcLightContribution( diffuseColour.xyz, vecView, index );
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
	
	out_Colour *= 1.20f;

///	vec3 vecView = normalize(vertMV);
///	
///	// Do lighting calculation per light
///	vec3 lightContrib = vec3(0.0f, 0.0f, 0.0f);
///	for ( int index = 0; index != NUMBEROFLIGHTS; index++ )
///	{
///		lightContrib += calcLightContribution( diffuseColour.xyz, vecView, index );
///	}
	
///	// Bump up the brightness (projector is too dark)
//	diffuse.xyz *= vec3(1.5f);
	
///	out_Colour = vec4(lightContrib, 1.0f);	
	
///	vec4 littleDiffuse = vec4(vec3(0.02f,0.02f,0.02f) * diffuseColour.xyz, 1.0f);
///		out_Colour += littleDiffuse;

///	//vec3 ambientColour = vec3(0.14f,0.14f,0.14f) * out_Colour.xyz;
///	//out_Colour = vec4(out_Colour.xyz + ambientColour, 1.0f);

///	//out_Colour *= 0.001f;
	
//	out_Colour += diffuseColour;
	
	if(useGlobalAlphaValue)
	{
		out_Colour.a = globalAlphaValue;
	}
	else
	{
		out_Colour.a = diffuseColour.a;
	}

}