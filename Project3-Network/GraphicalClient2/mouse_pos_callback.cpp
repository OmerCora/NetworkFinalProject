#include "GlobalStuff.h"

#include <sstream>

float pitchAngle = 0.0f;
float YawAngle = 0.0f;

float eyeDistance = 50.0f;

float lastPosX = 0.0f;
float lastPosY = 0.0f;

void mouseposcallback(GLFWwindow* window, double posX, double posY)
{
	

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
	glfwSetWindowTitle( window, ssTitle.str().c_str() );


	return;
}
