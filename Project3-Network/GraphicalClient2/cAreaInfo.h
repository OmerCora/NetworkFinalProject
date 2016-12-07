#pragma once
#include <glm\vec3.hpp>

class cAreaInfo
{
public:
	cAreaInfo();
	~cAreaInfo();

	float getLocationRange();

	glm::vec3 Max;
	glm::vec3 Min;

};