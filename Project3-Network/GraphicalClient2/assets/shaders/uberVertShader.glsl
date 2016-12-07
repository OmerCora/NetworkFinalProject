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

const int NUMBEROFLIGHTS = 12;

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

out vec4 diffuseColour;	
out vec3 vertMV;
out vec3 vertWorld;
out vec3 vertNorm;

out vec3 lightVector[NUMBEROFLIGHTS];		// For Phong
out vec3 lightHalfVector[NUMBEROFLIGHTS];	// For Blinn-Phong
out vec3 text_STU;	// ADDED, texture coordinates (also called UV)

// VERTEX shader
void main()
{
	mat4 matMV = matView * matModel;	// model-view matrix
	//mat4 matMVP = matProj * matView * matModel;		// m = p * v * m;
	
	vertMV = vec3(matMV * vec4(vPosition.xyz, 1.0f)).xyz;
	
	mat4 matNormal = inverse( transpose(matModel) );
	
	vertNorm = normalize(mat3(matNormal) * normalize(vNormal.xyz));
	
	vertWorld = vec3(matModel * vec4(vPosition.xyz, 1.0f));
		

	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{
		lightVector[index].xyz = theLights[index].position.xyz - vertWorld.xyz;
	}
	
	gl_Position = matModel * vec4(vPosition.xyz, 1.0f);	
	gl_Position = matProj * matView * gl_Position;	
	//gl_Position = matMVP * vec4(vPosition.xyz, 1.0f);	
	

	if ( isWireframe == 1 )  
	{
		diffuseColour = objectColour;
	}
	else  
	{	
		diffuseColour = vec4(vColour, 1.0f);
	}
	

	text_STU = vTexSTU;
	
}