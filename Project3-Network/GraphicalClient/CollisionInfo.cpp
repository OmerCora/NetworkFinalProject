#include "CollisionInfo.h"

#include "gelMeshManager.h"

CollisionInfo::CollisionInfo()
{
	this->debug_mode = DRAW_COLLISION_OBJECT;
	this->type = SPHERE_ARRAY;
}

CollisionInfo::~CollisionInfo()
{
}

std::string CollisionInfo::GetCollisionModelName(void)
{
	return this->collisionModelName;
}

void CollisionInfo::SetCollisionMode_Sphere(CollisionInfo::CollisionSphere sphere)
{
	this->sphereBody = sphere;
	this->type = SPHERE;
}

void CollisionInfo::SetCollisionMode_SphereArray(std::vector<CollisionInfo::CollisionSphere> sphereArray)
{
	this->type = SPHERE_ARRAY;
	this->sphereBodyVector = sphereArray;
}

void CollisionInfo::SetCollisionMode_LocalAABB(std::string collisionModelAlias)
{
	//get object from ID and create a AABB considering objects' scale*10;
	this->type = LOCAL_AABB;
	this->collisionModelName = collisionModelAlias;
	//gelMeshManager::sharedInstance().GetCollisionModelTriangleArrayForMesh
	//mapTriangle = 
}

CollisionInfo::CollisionSphere::CollisionSphere()
{
	this->inContact = false;
	center_offset.x = 0.0f;
	center_offset.y = 0.0f;
	center_offset.z = 0.0f;
	this->radius = 1.0f;
}
