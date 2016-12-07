/*
* Program Name : INFO6017_AI_Project4 - cCamera.cpp
* Purpose      : INFO-6017	Artificial Intelligence Poject3
* Description  : cCamera class implementation
* Date         : December 8, 2016
* Coder        : Jaehuhn Park
*/

#include "cCamera.h"
#include "GlobalStuff.h"
#include <sstream>

//cCamera constructor
cCamera::cCamera()
{
	mZoomSpeed = 20.f;
	mPanSpeed = 20.0f;
	mDistance = 60.0f;
	mOffset.x = g_pValueReader->InitalCameraX; //read from the external text file
	mOffset.y = g_pValueReader->InitalCameraY;
	mOffset.z = g_pValueReader->InitalCameraZ;
	mOffset = glm::normalize(mOffset);
}

//cCamera destructor
cCamera::~cCamera() { }

void cCamera::GetProjectionMatrix(glm::mat4& projOut)
{
	projOut = glm::perspective(glm::radians(50.0f), gWindowRatio, 0.01f, 1000.0f);
}

void cCamera::GetViewMatrix(glm::mat4& viewOut)
{
	viewOut = mViewMatrix;
}

void cCamera::SetTargetTransform(glm::mat4& targetTransform)
{
	mTargetTranform = targetTransform;
}

void cCamera::Update(float dt)
{

	bool pressUp = glfwGetKey(gWindow, GLFW_KEY_UP) == GLFW_PRESS;
	bool pressDown = glfwGetKey(gWindow, GLFW_KEY_DOWN) == GLFW_PRESS;
	if (pressDown != pressUp)
	{
		float zoomSpeed = mZoomSpeed;
		if (pressUp) zoomSpeed *= -1.f;
		mDistance += zoomSpeed * dt;
	}
	
	// key left / right = pan left / pan right
	pressLeft = glfwGetKey(gWindow, GLFW_KEY_LEFT) == GLFW_PRESS;
	pressRight = glfwGetKey(gWindow, GLFW_KEY_RIGHT) == GLFW_PRESS;
	if (pressLeft != pressRight)
	{
		glm::quat cameraOrientation = gPlayer->mPhysicsInfo.Orientation;
		float viewAngle, panSpeed = mZoomSpeed;
		if (pressLeft)
		{
			viewAngle = glm::radians(-90.0f); 
			panSpeed *= -1.f;
		}
		if (pressRight)
		{
			viewAngle = glm::radians(90.0f); 
			panSpeed *= 1.f;
		}
		cameraOrientation = glm::rotate(cameraOrientation, viewAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		mTargetTranform = glm::mat4_cast(cameraOrientation);
		//move the camera along with the player's forwading or backwarding.
		mTargetTranform[3] = glm::vec4(gPlayer->mPhysicsInfo.Position, 1.0f);
	}

	bool pressPitchUp = glfwGetKey(gWindow, GLFW_KEY_1) == GLFW_PRESS;
	bool pressPitchDown = glfwGetKey(gWindow, GLFW_KEY_2) == GLFW_PRESS;
	if (pressPitchDown != pressPitchUp)
	{
		float pitchSpeed = 0.8f;
		float zoomSpeed = 30.0f;
		if (pressPitchDown)
		{
			zoomSpeed *= -1.0f;
			pitchSpeed *= -1.f;
		}
		mDistance += zoomSpeed * dt;
		mOffset.y += pitchSpeed * dt;
	}



	float pitchAngle = 0.0f;
	float YawAngle = 0.0f;

	float eyeDistance = 50.0f;

	double posX = 0.0f;
	double posY = 0.0f;
	float lastPosX = 0.0f;
	float lastPosY = 0.0f;

	glfwGetCursorPos(gWindow, &posX, &posY);

	glm::vec3 targetToEye;
	pitchAngle += (posY - lastPosY)*0.005;
	YawAngle -= (posX - lastPosX)*0.005;

	if (pitchAngle > 1.5) pitchAngle = 1.5f;
	if (pitchAngle < -1.5) pitchAngle = -1.5f;

	lastPosX = posX;
	lastPosY = posY;

	targetToEye.x = glm::cos(pitchAngle);
	targetToEye.y = glm::sin(pitchAngle);
	targetToEye.z = 0.0f;

	targetToEye.z = -glm::sin(YawAngle) * targetToEye.x;
	targetToEye.x = glm::cos(YawAngle) * targetToEye.x;

	targetToEye *= eyeDistance;

	g_CameraX = targetToEye.x + g_TargetX;
	g_CameraY = targetToEye.y + g_TargetY;
	g_CameraZ = targetToEye.z + g_TargetZ;


	std::stringstream ssTitle;
	ssTitle << "Camera: "
		<< ::g_CameraX << ", "
		<< ::g_CameraY << ", "
		<< ::g_CameraZ;
	glfwSetWindowTitle(gWindow, ssTitle.str().c_str());



	UpdateView();
}

void cCamera::UpdateView()
{
	//up vector for lookAt
	glm::vec3 up(0.f, 1.f, 0.f);

	// set up the View matrix
	glm::vec3 target(mTargetTranform[3].x, mTargetTranform[3].y, mTargetTranform[3].z);
	glm::vec4 offset(mOffset.x, mOffset.y, mOffset.z, 0.f);
	offset = mTargetTranform * offset;
	offset = glm::normalize(offset) * mDistance;
	glm::vec3 eye(target.x + offset.x, target.y + offset.y, target.z + offset.z);

	mViewMatrix = glm::lookAtRH(eye, target, up);
}


