#include "GelCollisionManager.h"

#include "GelsenizeCore.h"
#include "externals.h"
#include "managers.h"
#include "iGameObject.h"

#include "cAABB.h"

#include "OCHelper.h"

//some links i have read or used
//https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
//http://gamedev.stackexchange.com/questions/114955/m%C3%B6ller-trumbore-intersection-point
//http://stackoverflow.com/questions/37652337/m%C3%B6ller-trumbore-ray-tri-intersection-algorithm
//http://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection

//WutFace
bool GelCollisionManager::MollerTrumboreRayTriIntersection(glm::vec3 &rayOrigin, glm::vec3& rayDirection, glm::vec3 &vert0, glm::vec3 &vert1, glm::vec3 &vert2, glm::vec3 & P, bool cull)
{
	static const float eps = 0.000001f;
	glm::vec3 edge1(0.0f, 0.0f, 0.0f);
	glm::vec3 edge2(0.0f, 0.0f, 0.0f);
	glm::vec3 tvec(0.0f, 0.0f, 0.0f);
	glm::vec3 pvec(0.0f, 0.0f, 0.0f);
	glm::vec3 qvec(0.0f, 0.0f, 0.0f);
	float determinant, inverse_determinant;

	edge1 = vert1 - vert0;
	edge2 = vert2 - vert0;
	pvec = glm::cross(rayDirection, edge2);
	determinant = glm::dot(edge1, pvec);

	if (cull)
	{
		if (determinant < eps)
		{
			//std::cout << "cull test failed" << std::endl;
			return false;
		}

		tvec = rayOrigin - vert0;

		P.x = dot(tvec, pvec);

		if (P.x < 0.0f || P.x > determinant)
		{
			//std::cout << "U test failed" << std::endl;
			return false;
		}

		qvec = glm::cross(tvec, edge1);
		P.y = glm::dot(rayDirection, qvec);

		if (P.y < 0.0f || (P.x + P.y) > determinant)
		{
			//std::cout << "V test failed" << std::endl;
			return false;
		}

		P.z = glm::dot(edge2, qvec);
		inverse_determinant = 1.0f / determinant;
		P.x *= inverse_determinant;
		P.y *= inverse_determinant;
		P.z *= inverse_determinant;
	}
	else
	{
		if (determinant > -eps && determinant < eps)
		{
			return false;
		}

		inverse_determinant = 1.0f / determinant;
		tvec = rayOrigin - vert0;
		P.x = glm::dot(tvec, pvec) * inverse_determinant;

		if (P.x < 0.0f || P.x > 1.0f)
		{
			return false;
		}

		qvec = cross(tvec, edge1);
		P.y = glm::dot(rayDirection, qvec) * inverse_determinant;

		if (P.y < 0.0f || P.x + P.y > 1.0f)
		{
			return false;
		}

		P.z = dot(edge2, qvec) * inverse_determinant;
	}

	return true;
}

// "Point" is really vec3
// "Vector" is also really vec3
glm::vec3 ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	// Check if P in vertex region outside A
	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ap = p - a;
	float d1 = glm::dot(ab, ap);		// glm::dot( ab, ap );
	float d2 = glm::dot(ac, ap);
	if (d1 <= 0.0f && d2 <= 0.0f) return a; // barycentric coordinates (1,0,0)

											// Check if P in vertex region outside B
	glm::vec3 bp = p - b;
	float d3 = glm::dot(ab, bp);
	float d4 = glm::dot(ac, bp);
	if (d3 >= 0.0f && d4 <= d3) return b; // barycentric coordinates (0,1,0)

										  // Check if P in edge region of AB, if so return projection of P onto AB
	float vc = d1*d4 - d3*d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
		float v = d1 / (d1 - d3);
		return a + v * ab; // barycentric coordinates (1-v,v,0)
	}

	// Check if P in vertex region outside C
	glm::vec3 cp = p - c;
	float d5 = glm::dot(ab, cp);
	float d6 = glm::dot(ac, cp);
	if (d6 >= 0.0f && d5 <= d6) return c; // barycentric coordinates (0,0,1)

										  // Check if P in edge region of AC, if so return projection of P onto AC
	float vb = d5*d2 - d1*d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
		float w = d2 / (d2 - d6);
		return a + w * ac; // barycentric coordinates (1-w,0,w)
	}

	// Check if P in edge region of BC, if so return projection of P onto BC
	float va = d3*d6 - d5*d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return b + w * (c - b); // barycentric coordinates (0,1-w,w)
	}

	// P inside face region. Compute Q through its barycentric coordinates (u,v,w)
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	return a + ab * v + ac * w; // = u*a + v*b + w*c, u = va * denom = 1.0f - v - w
}

void GelCollisionManager::GetContacts(std::vector<cContact>& contactVec)
{
	contactVec = this->contactVector;
}

void GelCollisionManager::SetSphereArrayForAlias(std::string alias, std::vector<CollisionInfo::CollisionSphere> sphereArray)
{
	this->map_MeshNameToCollisionSphereArray[alias] = sphereArray;
}


bool GelCollisionManager::GetSphereArrayForAlias(std::string alias, std::vector<CollisionInfo::CollisionSphere> &sphereArray)
{
	std::map< std::string, std::vector<CollisionInfo::CollisionSphere> >::iterator itColSphere
		= this->map_MeshNameToCollisionSphereArray.find(alias);

	// If the iterator ISN'T set to end(), then it found it
	if (itColSphere == this->map_MeshNameToCollisionSphereArray.end())
	{	// On no! Didn't find it
		return false;
	}
	sphereArray = itColSphere->second;
	return true;
}


float distanceBetweenSpheres1(CollisionInfo::CollisionSphere sphereLeft, CollisionInfo::CollisionSphere sphereRight);
float distanceBetweenPoints1(glm::vec3 A, glm::vec3 B);

void GelCollisionManager::Update(float deltaTime)
{
	this->map_AABBToSphereArray.clear();
	this->contactVector.clear();

	//small/medium to small/medium sized object collision

	//this will come down to a Sphere to sphere collision with AABB broad phasing in the end

	std::vector<iGameObject*> gameObjects = gelObjectManager::sharedInstance().GetGameObjects();
	std::vector<iGameObject*> gameObjects_sphereArray;
	std::vector<iGameObject*> gameObjects_localAABB;
	for (int index = 0; index != gameObjects.size(); index++)
	{
		iGameObject* gameObject = gameObjects[index];
		CollisionInfo collision = gameObject->GetCollision();
		if (collision.type == CollisionInfo::TYPE::SPHERE_ARRAY)
		{
			gameObjects_sphereArray.push_back(gameObject);
			for (int index2 = 0; index2 != collision.sphereBodyVector.size(); index2++)
			{
				CollisionInfo::CollisionSphere sphere = collision.sphereBodyVector[index2];
				RenderInfo render = gameObject->GetRender();
				GelPosition position = render.position;
				glm::mat4x4 m = glm::translate( render.transform, sphere.center_offset);
				sphere.position = m[3];
				sphere.ownerID = gameObject->getObjectID();

				cAABB pTestAABB;

				pTestAABB.boxWidth = 10.0f; //nudge nudge, wink wink
				pTestAABB.minXYZ.x = floor(position.x / pTestAABB.boxWidth);
				pTestAABB.minXYZ.y = floor(position.y / pTestAABB.boxWidth);
				pTestAABB.minXYZ.z = floor(position.z / pTestAABB.boxWidth);
				pTestAABB.UpdateMaxFromMinAndWidth();

				//keep last drawn id 
				int aabbID = pTestAABB.CalculateID(pTestAABB.minXYZ);
				this->map_AABBToSphereArray[aabbID].push_back(sphere);
			}
		}
		else if (collision.type == CollisionInfo::TYPE::LOCAL_AABB)
		{
			gameObjects_localAABB.push_back(gameObject);
		}
	}

	if (map_AABBToSphereArray.size() > 0)
	{
		int x = 0;
	}

	typedef std::map< int, std::vector<CollisionInfo::CollisionSphere> >::iterator aabbIT;
	for (aabbIT iterator = map_AABBToSphereArray.begin(); iterator != map_AABBToSphereArray.end(); iterator++) {
		// iterator->first = key
		// iterator->second = value
		std::vector<CollisionInfo::CollisionSphere> sphereArray = iterator->second;

		if (sphereArray.size() <= 1)
		{
			continue;
		}

		for (int m = 0; m != sphereArray.size(); m++)
		{
			CollisionInfo::CollisionSphere sphereLeft = sphereArray[m];
			for (int n = m+1; n != sphereArray.size(); n++)
			{
				CollisionInfo::CollisionSphere sphereRight = sphereArray[n];
				
				if (sphereLeft.ownerID == sphereRight.ownerID)
				{
					continue;
				}

				float distanceBetweenSpheres = distanceBetweenSpheres1(sphereLeft, sphereRight);

				//radius times unitScale and radius is 2r here;
				if ( distanceBetweenSpheres < (sphereLeft.radius+sphereRight.radius)/20.0f)
				{
					cContact curContact;
					curContact.objectID = sphereLeft.ownerID;
					curContact.collisionType = cContact::WITH_SPHERE;
					curContact.contactXYZ = glm::vec3((sphereLeft.position + sphereRight.position) / 2.0f);
					this->contactVector.push_back(curContact);

				}
			}
		}
	}


	//return;


	/*if (contactVector.size() > 0)
	{
		int x = 0;
	}*/
	////////////


	for (aabbIT iterator = map_AABBToSphereArray.begin(); iterator != map_AABBToSphereArray.end(); iterator++) 
	{
		std::vector<CollisionInfo::CollisionSphere> sphereArray = iterator->second;

		if (sphereArray.size() <= 1)
		{
			continue;
		}

		//for each collision sphere, check for the collision model vicinity
		for (int m = 0; m != sphereArray.size(); m++)
		{
			CollisionInfo::CollisionSphere sphere = sphereArray[m];
			glm::vec3 spherePosition = sphere.position;

			for (int index2 = 0; index2 != gameObjects_localAABB.size(); index2++)
			{
				iGameObject* collisionModel = gameObjects_localAABB[index2];
				glm::vec3 collisionModelposition = collisionModel->GetRender().position;
				std::map <int, std::vector<GelTriangle> > triangleMap;
				float halfExtend;
				glm::vec3 minPoint;

				gelMeshManager::sharedInstance().GetCollisionModelTriangleInfoForMesh(collisionModel->GetCollision().GetCollisionModelName(), halfExtend, minPoint);

				float distanceToCollisionModel = glm::distance(spherePosition, collisionModel->GetRender().position);
				if (distanceToCollisionModel <= halfExtend)
				{
					//inside the local AABB

					cAABB bbox;
					int boxID = bbox.CalculateID_Local(spherePosition - collisionModelposition, halfExtend);
					std::vector<GelTriangle> vecTriangle;
					gelMeshManager::sharedInstance().GetCollisionModelTriangleArrayForMesh(collisionModel->GetCollision().GetCollisionModelName(), boxID, vecTriangle);
					if (vecTriangle.size() > 0)
					{
						for (int i = 0; i != vecTriangle.size(); i++)
						{

							glm::vec3 centerPointOfTriangle = ( vecTriangle[i].v1 + vecTriangle[i].v2 + vecTriangle[i].v3 ) / 3.0f;
							glm::vec3 checkPoint1 = (centerPointOfTriangle + vecTriangle[i].v1) / 2.0f;
							glm::vec3 checkPoint2 = (centerPointOfTriangle + vecTriangle[i].v2) / 2.0f;
							glm::vec3 checkPoint3 = (centerPointOfTriangle + vecTriangle[i].v3) / 2.0f;

							//this is not translated but only moved, ignoring orientation for now
							glm::vec3 sphereRelativePosition = spherePosition - collisionModelposition;

							float distanceBetween1 = glm::distance(sphereRelativePosition, checkPoint1);
							float distanceBetween2 = glm::distance(sphereRelativePosition, checkPoint2);
							float distanceBetween3 = glm::distance(sphereRelativePosition, checkPoint3);

							//cast ray to closest checkpoint
							float closestCheckPoint = distanceBetween1;
							if (closestCheckPoint > distanceBetween2)
							{
								closestCheckPoint = distanceBetween2;
							}
							if (closestCheckPoint > distanceBetween3)
							{
								closestCheckPoint = distanceBetween3;
							}

							glm::vec3 rayOrigin = sphereRelativePosition;
							glm::vec3 rayDirection = glm::normalize(closestCheckPoint - sphereRelativePosition);

							//float distanceBetween = glm::distance(spherePosition - collisionModelposition, vecTriangle[i].v1);

							//radius times unitScale and radius is 2r here;
							//if (distanceBetween < (sphere.radius) / 20.0f)
							glm::vec3 intersectionPoint = ClosestPtPointTriangle(sphereRelativePosition, vecTriangle[i].v1, vecTriangle[i].v2, vecTriangle[i].v3);
							if (glm::distance(intersectionPoint, sphereRelativePosition))
							{
								cContact curContact;
								curContact.objectID = sphere.ownerID;
								glm::vec3 contactNormal;
								OCHelper::calculateNormalOfTriangle(vecTriangle[i].v1, vecTriangle[i].v2, vecTriangle[i].v3, contactNormal);
								curContact.normal = contactNormal;
								curContact.collisionType = cContact::WITH_TRIANGLE; //not with sphere actually but for now
								curContact.contactXYZ = intersectionPoint + collisionModelposition;
								this->contactVector.push_back(curContact);
							}

							//if (this->MollerTrumboreRayTriIntersection(rayOrigin, rayDirection, vecTriangle[i].v1, vecTriangle[i].v2, vecTriangle[i].v3, intersectionPoint))
							//{
							//	cContact curContact;
							//	curContact.objectID = sphere.ownerID;
							//	curContact.collisionType = cContact::WITH_SPHERE; //not with sphere actually but for now
							//	curContact.contactXYZ = intersectionPoint;
							//	this->contactVector.push_back(curContact);
							//}
						}
					}

					//gelMeshManager::sharedInstance().GetCollisionModelTriangleMapForMesh(collisionModel->GetCollision().GetCollisionModelName(), triangleMap);
					
					//std::vector<GelTriangle> triangleVec = triangleMap[boxID];

					
					//std::cout << boxID << std::endl;
				}

			}

		}
	}

	return;

	//small/medium to large sized object collision

	//this will first check if smaller object is in vicinity of the large object, if so this object will be placed inside the local AABB system (actually glm::translated) and be checked by the triangles.
	//dont forget to consider that triangle drawing;


	//for (int index = 0; index != gameObjects_sphereArray.size(); index++)
	//{
	//	//get sphere position and translate into every collision model's world and see it's local AABB has some triangles in it.
	//	iGameObject* sphereArrayToCheck = gameObjects_sphereArray[index];
	//	glm::vec3 objectPosition = sphereArrayToCheck->GetRender().position;

	//	CollisionInfo collision = sphereArrayToCheck->GetCollision();
	//	for (int index1 = 0; index1 != collision.sphereBodyVector.size(); index1++)
	//	{
	//		CollisionInfo::CollisionSphere sphereToCheck = collision.sphereBodyVector[index1];
	//		glm::vec3 spherePosition;

	//		gelMeshManager::sharedInstance().GetCollisionModelTriangleInfoForMesh(collisionModel->GetCollision().GetCollisionModelName(), halfExtend, minPoint);

	//		float distanceToCollisionModel = glm::distance(objectPosition, collisionModel->GetRender().position);
	//		if (distanceToCollisionModel <= halfExtend)
	//		{
	//			//inside the local AABB

	//			cAABB bbox;
	//			gelMeshManager::sharedInstance().GetCollisionModelTriangleMapForMesh(collisionModel->GetCollision().GetCollisionModelName(), triangleMap);



	//		}

	//	}


	//	for (int index2 = 0; index2 != gameObjects_localAABB.size(); index2++)
	//	{
	//		iGameObject* collisionModel = gameObjects_localAABB[index2];
	//		std::map <int, std::vector<GelTriangle> > triangleMap;
	//		float halfExtend;
	//		glm::vec3 minPoint;
	//	
	//		

	//		gelMeshManager::sharedInstance().GetCollisionModelTriangleInfoForMesh(collisionModel->GetCollision().GetCollisionModelName(), halfExtend, minPoint);

	//		float distanceToCollisionModel = glm::distance(objectPosition, collisionModel->GetRender().position);
	//		if (distanceToCollisionModel <= halfExtend)
	//		{
	//			//inside the local AABB

	//			cAABB bbox;
	//			gelMeshManager::sharedInstance().GetCollisionModelTriangleMapForMesh(collisionModel->GetCollision().GetCollisionModelName(), triangleMap);



	//		}
	//		//	glm::mat4x4 m = glm::mat4x4(1.0f);
	//		//	m = glm::translate(collisionModel->GetRender().transform, objectPosition);

	//		//	glm::vec4 translatedPos = m[3];

	//		//	int aabbID = bbox.CalculateID_Local(glm::vec3(translatedPos) + minPoint, halfExtend);
	//		//	std::vector<GelTriangle> vecTriangle;
	//		//	gelMeshManager::sharedInstance().GetCollisionModelTriangleArrayForMesh(collisionModel->GetCollision().GetCollisionModelName(), aabbID, vecTriangle);
	//		//	if (vecTriangle.size() > 0)
	//		//	{
	//		//		int x = 0;
	//		//	}
	//		//}

	//		
	//	}
	//}



}


float distanceBetweenPoints1(glm::vec3 A, glm::vec3 B)
{
	float deltaX = A.x - B.x;
	float deltaY = A.y - B.y;
	float deltaZ = A.z - B.z;

	return sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);

	// ***************************************************
	// Note that glm already has a function for this:
	//return glm::distance( A, B );
	// ***************************************************
}

float distanceBetweenSpheres1(CollisionInfo::CollisionSphere sphereLeft , CollisionInfo::CollisionSphere sphereRight)
{
	
	float deltaX = sphereLeft.position.x - sphereRight.position.x;
	float deltaY = sphereLeft.position.y - sphereRight.position.y;
	float deltaZ = sphereLeft.position.z - sphereRight.position.z;

	return sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);

}