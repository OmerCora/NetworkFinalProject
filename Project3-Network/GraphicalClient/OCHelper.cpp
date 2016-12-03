#include "OCHelper.h"
#include <stdlib.h>
#include <algorithm>
#include <time.h>

OCHelper::OCHelper()
{
}

OCHelper::~OCHelper()
{

}

float OCHelper::getRandFloat(float LO, float HI)
{
	static int setup = 0;
	if (setup == 0)
	{
		srand(time(NULL));
		setup = 1;
	}
	float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	return r3;
}


float OCHelper::clamp(float x, float min, float max)
{	// Taken from a post on stack overflow
	return std::max(min, std::min(x, max));
}

// From Ken Perlin... 
float OCHelper::smootherStep(float min, float max, float x)
{
	// From: https://en.wikipedia.org/wiki/Smoothstep
	x = OCHelper::clamp((x - min) / (max - min), 0.0f, 1.0f);
	return x*x*x*(x*(x * 6 - 15) + 10);
}

void OCHelper::calculateNormalOfTriangle(glm::vec3 vertex_1, glm::vec3 vertex_2, glm::vec3 vertex_3, glm::vec3& normalVector)
{
	//normal on first vertex
	glm::vec3 n1_1(
		vertex_3.x - vertex_1.x,
		vertex_3.y - vertex_1.y,
		vertex_3.z - vertex_1.z);
	glm::vec3 n1_2(
		vertex_2.x - vertex_1.x,
		vertex_2.y - vertex_1.y,
		vertex_2.z - vertex_1.z);

	//normal on second vertex
	glm::vec3 n2_1(
		vertex_1.x - vertex_2.x,
		vertex_1.y - vertex_2.y,
		vertex_1.z - vertex_2.z);
	glm::vec3 n2_2(
		vertex_3.x - vertex_2.x,
		vertex_3.y - vertex_2.y,
		vertex_3.z - vertex_2.z);

	//normal on second vertex
	glm::vec3 n3_1(
		vertex_2.x - vertex_3.x,
		vertex_2.y - vertex_3.y,
		vertex_2.z - vertex_3.z);
	glm::vec3 n3_2(
		vertex_1.x - vertex_3.x,
		vertex_1.y - vertex_3.y,
		vertex_1.z - vertex_3.z);

	//get cross products for normals
	glm::vec3 normal_1 = glm::normalize(glm::cross(n1_1, n1_2));
	glm::vec3 normal_2 = glm::normalize(glm::cross(n2_1, n2_2));
	glm::vec3 normal_3 = glm::normalize(glm::cross(n3_1, n3_2));

	normalVector = -glm::normalize((normal_1 + normal_2 + normal_3) / 3.0f);
}
