#ifndef _COLLISIONINFO_HG_
#define _COLLISIONINFO_HG_

#include "GelStructures.h"
#include <string>
#include <vector>

class CollisionInfo
{
public:
	CollisionInfo();
	~CollisionInfo();

	std::string GetCollisionModelName(void);

	struct CollisionSphere
	{
		CollisionSphere();
		bool inContact;
		int ownerID; //having thoughts about this.
		GelPosition position; // glm::vec3
		GelPosition center_offset; // glm::vec3
		float radius;
	};

	enum TYPE
	{
		NO_COLLISION,
		SPHERE, //pretty much for small sized bland gameobjects that fit well enough into one sphere
		SPHERE_ARRAY, //for more fleshed out medium sized game objects, define a list of points and radii
		LOCAL_AABB,
		TERRAIN, //mb

		//BOX,   //use box triangles maybe
		//BOX_ARRAY, //use box array triangles maybe
	};

	enum DEBUG_MODE
	{
		NO_DRAW,
		DRAW_COLLISION_OBJECT,
		DRAW_COLLISION_OBJECT_CONTACT_ONLY
	};

	TYPE type;
	DEBUG_MODE debug_mode;

	CollisionSphere sphereBody; //for TYPE::SPHERE
	std::vector<CollisionSphere>  sphereBodyVector; //for TYPE::SPHERE_ARRAY

	GelColor defaultColor; //use this while drawing debug spheres, aabbs too maybe
	GelColor contactColor;

	void SetCollisionMode_Sphere(CollisionInfo::CollisionSphere sphere);
	void SetCollisionMode_SphereArray(std::vector<CollisionInfo::CollisionSphere> sphereArray);
	void SetCollisionMode_LocalAABB(std::string collisionModelAlias); //how to approach this hm

private:
	std::string collisionModelName;
	std::vector<GelTriangle> * vecTriangle;
};



#endif // !_COLLISIONINFO_HG_
