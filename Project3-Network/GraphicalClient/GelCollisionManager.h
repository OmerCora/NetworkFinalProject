#ifndef _GELCOLLISIONMANAGER_HG_
#define _GELCOLLISIONMANAGER_HG_

#include "iManager.h"
#include "iGameObject.h"\

#include "cContact.h"

#include <map>
#include <vector>
#include <string>

class GelCollisionManager : public iManager
{
public:
	//Singleton 
	static GelCollisionManager& sharedInstance()
	{
		static GelCollisionManager instance;
		return instance;
	}

	bool MollerTrumboreRayTriIntersection(glm::vec3 &rayOrigin, glm::vec3& rayDirection, glm::vec3 &vert0, glm::vec3 &vert1, glm::vec3 &vert2, glm::vec3 & P, bool cull = true);

	void GetContacts(std::vector<cContact>&contactVec);

	void SetSphereArrayForAlias(std::string alias, std::vector<CollisionInfo::CollisionSphere> sphereArray);
	bool GetSphereArrayForAlias(std::string alias, std::vector<CollisionInfo::CollisionSphere> &sphereArray);

	virtual void Update(float deltaTime);

private:
	GelCollisionManager() {};
	GelCollisionManager(GelCollisionManager const&); //dont implement - to build
	void operator=(GelCollisionManager const&); //dont implement - to build

	std::map<std::string, std::vector<CollisionInfo::CollisionSphere> > map_MeshNameToCollisionSphereArray;
	std::map< int, std::vector<CollisionInfo::CollisionSphere> > map_AABBToSphereArray;
	std::vector<cContact> contactVector;

};

#endif // !_GELLIGHTMANAGER_HG_

// "I am sensing a pattern here." -Skitskurr the Weaver