#version 400

// VAO layout (vertex attribute layout)
in vec3 vColour;		
in vec3 vPosition;	
in vec3 vNormal;
in vec3 vTexSTU;		// ADDED

uniform mat4 matModel;	// "per object" 
uniform mat4 matView;	// "per scene" or "per frame"
uniform mat4 matProj;	// "Projection" (only if size or aspect changes)

uniform vec4 objectColour;
uniform int isWireframe;		

const int NUMBEROFLIGHTS = 10;

struct sLightDesc
{
	bool enabled;
	vec4 position;		// In the world
	vec4 ambient;		// 0,0,0 (no ambient)
	vec4 diffuse;		// 1,1,1 (white colour light)
	vec4 specular;
	vec4 attenuation;	// x = const, y = linear, z = quad, w = not used
	vec4 direction;		// For directional and spot (ignored by others)
	vec4 typeAndParams;	// To be used soon. Specify type and attributes of type
	// x = type: 0 = point, 1 = spot, 2 = directional, 3 =hemispherical 
						// y = for Spot: cos cutoff start (start of penumbra, edge of bright spot)
						// z = for Spot: cos cutoff end (end of penumbra, beyond this it's dark)
};

uniform sLightDesc theLights[NUMBEROFLIGHTS];

// For spec:
uniform vec3 eyeLocation;

out vec4 diffuseColour;	
out vec3 vertMV;
out vec3 vertWorld;
out vec3 vertNorm;
//out vec3 viewVector;
out vec3 lightVector[NUMBEROFLIGHTS];		// For Phong
out vec3 lightHalfVector[NUMBEROFLIGHTS];	// For Blinn-Phong
out vec3 text_STU;	// ADDED, texture coordinates (also called UV)

out vec3 sb_texcoords;
out vec3 lightDirection[NUMBEROFLIGHTS];
out float attenuation[NUMBEROFLIGHTS];

// VERTEX shader
void main()
{
    // Copy the texture coordinates to the fragment shader
	text_STU = vTexSTU;

	sb_texcoords = vPosition;
	mat4 matMV = matView * matModel;	// model-view matrix
	mat4 matMVP = matProj * matView * matModel;		// m = p * v * m;
	
	vertMV = vec3(matMV * vec4(vPosition.xyz, 1.0f)).xyz;
	
	// The "normal" matrix. The normal without scaling or tranlation
	// (So this vector is extending from the origin, in the direction 
	//  of the vertex normal)
	// Since the vertex normal only needs rotation, we only 
	//  need to use a 3x3 matrix (the "top-left" portion of the 4x4 matrix)
	mat4 matNormal = inverse( transpose(matModel) );
	vertNorm = mat3(matNormal) * normalize(vNormal.xyz);
	
	vertNorm = normalize(vNormal.xyz);

	// Position of the vertex in the world
	// (i.e. without View or Projection matrix
	vertWorld = vec3(matModel * vec4(vPosition.xyz, 1.0f));
	
	vec3 viewVector = normalize(eyeLocation - vertWorld);

	vec3 hemiColor = vec3(0.0f,0.0f,0.0f);
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{

		if( int(theLights[index].typeAndParams.x) == 0 )
		{
			//point
			lightDirection[index].xyz = (theLights[index].position.xyz - vertWorld.xyz);
			float lightDistance = length(lightDirection[index].xyz);

			lightDirection[index].xyz = normalize(lightDirection[index].xyz);//lightDirection[index].xyz / lightDistance;
		
			float atten = 1.0f /(theLights[index].attenuation.x + 
				  theLights[index].attenuation.y * lightDistance +
				  theLights[index].attenuation.z * lightDistance * lightDistance );

			lightHalfVector[index].xyz = normalize(lightDirection[index].xyz + viewVector);

			attenuation[index] = atten;
		}
		else if( int(theLights[index].typeAndParams.x) == 1 )
		{
			//spot
			lightDirection[index].xyz = (theLights[index].position.xyz - vertWorld.xyz);
			float lightDistance = length(lightDirection[index].xyz);

			lightDirection[index].xyz = lightDirection[index].xyz / lightDistance;
		
			float atten = 1.0f /(theLights[index].attenuation.x + 
				  theLights[index].attenuation.y * lightDistance +
				  theLights[index].attenuation.z * lightDistance * lightDistance );

			lightHalfVector[index].xyz = normalize(lightDirection[index].xyz + viewVector);
			vec3 coneDirection = normalize(theLights[index].direction.xyz);

			float spotCos = dot( lightHalfVector[index].xyz, -coneDirection );
		
			// y = for Spot: cos cutoff start (start of penumbra, edge of bright spot)
			// z = for Spot: cos cutoff end (end of penumbra, beyond this it's dark)
		
			if ( spotCos < theLights[index].typeAndParams.z )
			{	
				// It's outside of the spot light "cone", so is dark
				atten = 0.0f;		
			}
			else if ( spotCos < theLights[index].typeAndParams.y )
			{
				atten *= smoothstep(theLights[index].typeAndParams.z, theLights[index].typeAndParams.y,  spotCos);
				//atten *= pow(spotCos, 10);
			}	

			attenuation[index] = atten;
		}
		else if( int(theLights[index].typeAndParams.x) == 2 )
		{
			//directional
			lightDirection[index].xyz = (theLights[index].position.xyz - vertWorld.xyz);
			float lightDistance = length(lightDirection[index].xyz);

			lightDirection[index].xyz = lightDirection[index].xyz / lightDistance;
			lightHalfVector[index].xyz = normalize(lightDirection[index].xyz + viewVector);

			attenuation[index] = 1.0f;
		}
		else if ( int(theLights[index].typeAndParams.x) == 3 )
		{
			//vec3 position = vec3(matMV * vec4(vPosition, 0.0f));
			//vec3 tnorm = vertNorm;
			//vec3 lightVec = normalize(theLights[index].position.xyz - position);

			//float costheta = dot(tnorm, lightVec);
			//float a = 0.5f + (0.5f * costheta ) ;

			//hemisphere
			//vec3 skycolor = vec3(1.0f,1.0f,1.0f);
			//vec3 groundcolor = vec3(0.0f,0.0f,0.0f);

			//vec3 finalHemiColor = mix(groundcolor, skycolor, a);
			
			//lightDirection[index].xyz = (theLights[index].position.xyz - vertWorld.xyz);
			//float lightDistance = length(lightDirection[index].xyz);

			//lightDirection[index].xyz = lightDirection[index].xyz / lightDistance;
			//lightHalfVector[index].xyz = normalize(lightDirection[index].xyz + viewVector);
			
			//finalHemiColor += hemiColor;	
		}
		
		//lightVector[index].xyz = theLights[index].position.xyz - vertWorld.xyz;
	}

	// Model+View+Projection matrix. 
	// i.e. the location of the pixel in screen space
	gl_Position = matMVP * vec4(vPosition.xyz, 1.0f);
	
	if ( isWireframe == 1 || true )  
	{
		diffuseColour = objectColour;
	}
	else  
	{	
		diffuseColour = objectColour; //vec4(vColour, 1.0f);
	}
	diffuseColour += hemiColor;
}