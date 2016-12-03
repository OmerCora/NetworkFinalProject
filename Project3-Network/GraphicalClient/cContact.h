#ifndef _CCONTACT_HG_
#define _CCONTACT_HG_

// Stores contact information for a SINGLE
//   object in the world. 
#include <glm/glm.hpp>

class cContact
{
public:
	enum eCollisionType
	{
		WITH_SPHERE,
		WITH_TRIANGLE,
		NO_COLLISION	/*is this even needed?*/
	};

	cContact() : penetrationDepth(0.0f), objectID(0), collisionType(NO_COLLISION) { }
	// From Ian Millington's book suggestion
	// Contact point (xyz) in world space
	// Contact normal (xyz) in world space (i.e. The direction of the contact)
	// Depth of penetration

	//for s->s only
	glm::vec3 contactVelocity;

	glm::vec3 contactXYZ;
	glm::vec3 normal;
	float penetrationDepth;
	// 
	unsigned int objectID;

	eCollisionType collisionType;
};

#endif
