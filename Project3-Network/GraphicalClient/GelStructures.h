#ifndef _GELSTRUCTURES_HG_
#define _GELSTRUCTURES_HG_

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/gtx/quaternion.hpp>

struct GelVertex
{
	GelVertex() : x(0.0f), y(0.0f), z(0.0f), r(1.0f), g(1.0f), b(1.0f), nX(0.0f), nY(0.0f), nZ(0.0f), s(0.0f), t(0.0f), u(0.0f) {}
	float x, y, z; //loc
	float r, g, b; //col
	float nX, nY, nZ;
	float s, t, u; //texture coordinates added
};

struct GelTriangle
{
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
};

//struct GelColor
//{
//	GelColor() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
//	float r, g, b, a;
//};

struct GelTriFace
{
	GelTriFace() : i1(-1), i2(-1), i3(-1) {}
	//indexes
	int i1, i2, i3;
};

struct GelQuadFace
{
	GelQuadFace() : i1(-1), i2(-1), i3(-1), i4(-1) {}
	//indexes
	int i1, i2, i3, i4;
};

struct GelThreeDimensional //maybe replace them with glm vectors? i am not sure about what to do here for the bestest pratice :(
{
	GelThreeDimensional() : x(0.0f), y(0.0f), z(0.0f) {}
	GelThreeDimensional(float a, float b, float c) : x(a), y(b), z(c) {}
	float x;
	float y;
	float z;
};
typedef glm::vec4 GelColor;
typedef glm::vec4 GelColour;
typedef glm::vec3 GelVelocity;
typedef glm::vec3 GelAcceleration;
typedef glm::vec3 GelPosition;
typedef glm::vec3 GelRotation;
typedef glm::quat GelOrientation; //added 
typedef glm::vec3 GelRotationalVelocity; //added

//typedef GelThreeDimensional GelPosition;
//typedef GelThreeDimensional GelRotation;
typedef GelThreeDimensional GelRotationPre;
//typedef GelThreeDimensional GelVelocity;
//typedef GelThreeDimensional GelAcceleration;
typedef float GelScale; //maybe later

#endif // !_GELSTRUCTURES_HG_

