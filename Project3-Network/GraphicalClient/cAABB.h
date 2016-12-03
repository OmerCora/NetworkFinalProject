#ifndef _CAABB_HG_
#define _CAABB_HG_

#include <vector>

#include "externals.h"

struct cPoint
{
	float x,y,z;
};

// Each AABB has a 10x10x10 "child" AABBs inside
class cAABB
{
public:
	cAABB() {
		localOrigin.x = 0.0f;
		localOrigin.y = 0.0f;
		localOrigin.z = 0.0f;
		this->maxXYZ.x = 0.0f;
		this->maxXYZ.y = 0.0f;
		this->maxXYZ.z = 0.0f;
		this->minXYZ.x = 0.0f;
		this->minXYZ.y = 0.0f;
		this->minXYZ.z = 0.0f;
		this->boxWidth = 10.0f;
	}
	cAABB(glm::vec3 minXYZ, float length)
	{
		localOrigin.x = 0.0f;
		localOrigin.y = 0.0f;
		localOrigin.z = 0.0f;
		this->minXYZ = minXYZ;
		this->minXYZ.x = 0.0f;
		this->minXYZ.y = 0.0f;
		this->minXYZ.z = 0.0f;
		this->boxWidth = length;
	}
	glm::vec3 minXYZ;
	glm::vec3 maxXYZ;

	glm::vec3 localOrigin; //offset the aabb to this; //use minXYZ instead

	void UpdateMaxFromMinAndWidth(void) 
	{
		this->maxXYZ.x = this->minXYZ.x + this->boxWidth;
		this->maxXYZ.y = this->minXYZ.y + this->boxWidth;
		this->maxXYZ.z = this->minXYZ.z + this->boxWidth;
	};
	float boxWidth;
	// Hash identifier for this AABB (related to the location in space)
//	45200.0, 67000.0, 85700.0
//	45000.0, 67000.0, 85000.0   minXYZ
//
//	1 meters.
//
//	850 670 450				32bits --> 0 to 4,000,000,000

	int CalculateID(glm::vec3 minXYZ)
	{
		//xxx,yyy,zzz; 
		//float boxWidth = 10.0f;		// Nudge nudge, wink wink
		unsigned int zPart = (unsigned int)(minXYZ.z) *       1;
		unsigned int yPart = (unsigned int)(minXYZ.y) *    1000;
		unsigned int xPart = (unsigned int)(minXYZ.x) * 1000000; 
		unsigned int ID = xPart + yPart + zPart;
		return ID;
	}

	int CalculateID_Local(glm::vec3 minXYZ, float halfMaxEntend)
	{
		glm::vec3 localPoint = minXYZ + halfMaxEntend;
		//xxx,yyy,zzz; 
		//float boxWidth = 10.0f;		// Nudge nudge, wink wink
		unsigned int zPart = (unsigned int)(localPoint.z) * 1;
		unsigned int yPart = (unsigned int)(localPoint.y) * 1000;
		unsigned int xPart = (unsigned int)(localPoint.x) * 1000000;
		unsigned int ID = xPart + yPart + zPart;
		return ID;
	}



	unsigned int ID;		// 32 bit int  ( 1,xxx,yyy,zzz)
	// Up to 1000x1000x1000 AABBs
	std::vector< unsigned int > vecChildAABBs;

	//CAABB* pVecAABBS[100][100][100] = {0};	// 4 Mbytes

	//std::map< > mapObjectIDsInsideMeNow;

	//pVecAABBS[40][54][50] = new CAABB();
};

#endif
