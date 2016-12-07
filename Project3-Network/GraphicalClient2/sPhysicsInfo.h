#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct sPhysicsInfo
{
	sPhysicsInfo() : IsStatic(false) {}
	
	bool IsStatic;

	glm::vec3 Position;
	glm::vec3 Velocity;
	glm::vec3 Acceleration;
	
	glm::quat Orientation;

	float fSpeed;
	float fRotationSpeed;
};