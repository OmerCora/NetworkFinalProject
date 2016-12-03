#include "PhysicsInfo.h"

PhysicsInfo::PhysicsInfo()
{
	this->velocity.x = this->velocity.y = this->velocity.z = 0.0f;
	this->acceleration.x = this->acceleration.y = this->acceleration.z = 0.0f;
	this->rotationalVelocity.x = this->rotationalVelocity.y = this->rotationalVelocity.z = 0.0f;

	this->enabled = false;
	this->ethereal = false;
	this->useSphericalBody = false;
}

PhysicsInfo::~PhysicsInfo()
{
}