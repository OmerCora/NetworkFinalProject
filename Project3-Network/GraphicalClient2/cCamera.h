/*
* Program Name : INFO6017_AI_Project4 - cCamera.h
* Purpose      : INFO-6017	Artificial Intelligence Poject3
* Description  : cCamera class 
* Date         : December 8, 2016
* Coder        : Jaehuhn Park
*/
#pragma once

#include <glm\fwd.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\random.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>



class cCamera
{
public:
	cCamera();
	~cCamera();

	void GetProjectionMatrix(glm::mat4& projOut);
	void GetViewMatrix(glm::mat4& viewOut);

	void Update(float dt);
	void SetTargetTransform(glm::mat4& targetTransform);

	//member variables for camera panning
	bool pressLeft;
	bool pressRight;

private:
	void UpdateView();

	glm::mat4 mTargetTranform;

	glm::vec3 mOffset;
	float mZoomSpeed;
	float mPanSpeed;
	float mDistance;
	float mMaxDistance;
	float mMinDistance;
	glm::mat4 mViewMatrix;

};