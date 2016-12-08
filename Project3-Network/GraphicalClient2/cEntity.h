/*
* Program Name : INFO6017_AI_Project4 - cEntity.h
* Purpose      : INFO-6017	Artificial Intelligence Poject3
* Description  : the base class of all entities on scene
* Date         : December 8, 2016
* Coder        : Jaehuhn Park
*/
#pragma once

#include <string>
#include "sPhysicsInfo.h"

struct sRenderingInfo
{
	sRenderingInfo() :Color(1, 1, 1), Scale(1,1,1) {}
	glm::vec3 Color;
	glm::vec3 Scale;
	glm::mat4 modelMatrix;
	std::string meshName;
	std::string textureName;
};

class cEntity
{
public:
	cEntity();
	virtual ~cEntity() {}	
	virtual void GetRenderingInfo(sRenderingInfo& renderingInfo);
	virtual void GetPhysicsInfo(sPhysicsInfo & physicsInfo);
	virtual void Update(float dt);

	void preRenderer();

	virtual const glm::vec3& GetForward();

	bool mbForwardable;

	//std::vector<cPathNode*> mVec_Paths;

	float collisonDetectPoint;

	sRenderingInfo mRenderingInfo;
	sPhysicsInfo mPhysicsInfo;
	bool isVisible;
};

