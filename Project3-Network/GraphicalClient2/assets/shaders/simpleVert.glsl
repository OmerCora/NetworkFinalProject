#version 400

// VAO layout (vertex attribute layout)
in vec3 vColour;		
in vec3 vPosition;	
in vec3 vNormal;

uniform mat4 matModel;	// "per object" 
//uniform mat4 matModelRotationOnly;	// "per object" 
uniform mat4 matView;	// "per scene" or "per frame"
uniform mat4 matProj;	// "Projection" (only if size or aspect changes)

uniform vec3 objectColour;
uniform int isWireframe;		

out vec3 diffuseColour;		
out vec4 vertWorldXYZ;
out vec4 vertNorm;

// VERTEX shader
void main()
{
	mat4 MVP = matProj * matView * matModel;		// m = p * v * m;
	
    gl_Position = MVP * vec4( vPosition, 1.0f );
	
	vertWorldXYZ = matModel * vec4( vPosition, 1.0f );
	
	// Strip out scaling and tranlation part of the matrix...
	mat4 modelIT = inverse( transpose(matModel) );	
	vertNorm     = modelIT * vec4( normalize(vNormal), 1.0f );
	
	//vertNorm = vec4(vNormal.xyz, 1.0f);
	
	if ( isWireframe == 1 )  
	{
		diffuseColour = objectColour;
	}
	else  
	{	
		diffuseColour = vColour;
	}
}