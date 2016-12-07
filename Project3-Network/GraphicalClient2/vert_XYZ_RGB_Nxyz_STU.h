#ifndef _vert_XYZ_RGB_Nxyz_STU_HG_
#define _vert_XYZ_RGB_Nxyz_STU_HG_

struct vert_XYZ_RGB_Nxyz_STU
{
	float r, g, b;	// in vec3 vColour;		
	float x, y, z;	// in vec3 vPosition;	
	float nX, nY, nZ;	// in vec3 vNormal;
	// ADDED: Texture coordinates
	float s, t, u;		// "s" is "u", "t" is "v"

	//glm::vec3 colour;
	//glm::vec3 positionl;
	//glm::vec3 normal;

	//float colour[3];
	//float position[3];
	//float normal[3];
};

#endif 
