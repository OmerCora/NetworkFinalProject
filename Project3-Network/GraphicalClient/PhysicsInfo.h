#ifndef _PHYSICSINFO_HG_
#define _PHYSICSINFO_HG_

#include "GelStructures.h"

class PhysicsInfo
{
public:
	PhysicsInfo();
	~PhysicsInfo();

	bool enabled;
	bool ethereal;
	bool useSphericalBody;

	GelVelocity velocity;
	GelAcceleration acceleration;
	GelRotationalVelocity rotationalVelocity; //glm::vec3

private:
	


	
};

#endif // !_PHYSICSINFO_HG_
