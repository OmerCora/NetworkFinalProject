#include "cCamera.h"

#include "GelsenizeCore.h"
#include "managers.h"

#include "OCHelper.h"

cCamera::cCamera()
{
	targetObject = 0;
	width = 1440;
	height = 960;

	zoomSpeed = 40.0f;
	minDistance = 10.0f;
	distance = 25.0f;
	maxDistance = 70.0f;

	maxFollowSpeed = 24.0f;
	followDistanceMax = 0.8f;
	followDistanceMin = 0.2f;

	pan = 0.0f;
	maxPan = 0.7f;
	minPan = -0.7f;
	panSpeed = 1.4f;

	offset = glm::vec3(0.f, 2.5f, -4.f);
	offset = glm::normalize(offset);

	projMatrix = glm::perspective(0.8f, (float)width / (float)height, 0.1f, 10000.f);
}

cCamera::~cCamera()
{
}

void cCamera::GetProjectionMatrix(glm::mat4& projOut)
{
	// set up the projection matrix
	projOut = projMatrix;
}

void cCamera::GetViewMatrix(glm::mat4& viewOut)
{
	viewOut = viewMatrix;
}

void cCamera::SetEyePosition(glm::vec4& eyeIn)
{
	this->eye = eyeIn;
}

void cCamera::GetEyePosition(glm::vec4& eyeOut)
{
	eyeOut.x = this->eye.x;
	eyeOut.y = this->eye.y;
	eyeOut.z = this->eye.z;
	eyeOut.w = 1.0f;
}

void cCamera::WindowResize(int width, int height)
{
	width = width;
	height = height;
}

void cCamera::Update(float deltaTime)
{
	bool pressUp = glfwGetKey(GelsenizeCore::sharedInstance().GetWindow(), GLFW_KEY_UP) == GLFW_PRESS;
	bool pressDown = glfwGetKey(GelsenizeCore::sharedInstance().GetWindow(), GLFW_KEY_DOWN) == GLFW_PRESS;
	bool pressLeft = glfwGetKey(GelsenizeCore::sharedInstance().GetWindow(), GLFW_KEY_LEFT) == GLFW_PRESS;
	bool pressRight = glfwGetKey(GelsenizeCore::sharedInstance().GetWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS;

	bool pressI = glfwGetKey(GelsenizeCore::sharedInstance().GetWindow(), GLFW_KEY_I) == GLFW_PRESS;
	bool pressK = glfwGetKey(GelsenizeCore::sharedInstance().GetWindow(), GLFW_KEY_K) == GLFW_PRESS;

	/*bool pressA = glfwGetKey(GelsenizeCore::sharedInstance().GetWindow(), GLFW_KEY_A) == GLFW_PRESS;
	bool pressD = glfwGetKey(GelsenizeCore::sharedInstance().GetWindow(), GLFW_KEY_D) == GLFW_PRESS;

	if (pressA != pressD)
	{
		float newPanSpeed = panSpeed;
		if (pressLeft) newPanSpeed *= -1.0f;
		pan += newPanSpeed *deltaTime;
		pan = glm::clamp(pan, minPan, maxPan);
	}*/

	//mOffset = glm::vec3(0.f, 0.5f, -1.f);
	if (pressI != pressK)
	{
		float dirZoomSpeed = zoomSpeed  * 4.0f;
		if (pressK) dirZoomSpeed *= -1.0f;
		dirZoomSpeed *= 0.2f;
		offset.y += dirZoomSpeed * deltaTime;
		offset.y = glm::clamp(offset.y, -4.0f, +4.0f);
		//if (offset.y < -minDistance) offset.y = -minDistance;
		//if (offset.y > maxDistance) offset.y = maxDistance;
	}

	if (pressDown != pressUp)
	{
		float dirZoomSpeed = zoomSpeed * 4.0f;
		if (pressUp) dirZoomSpeed *= -1.0f;
		distance += dirZoomSpeed * deltaTime;
		if (distance < minDistance) distance = minDistance;
		if (distance > maxDistance) distance = maxDistance;
	}

	if ((pressLeft != pressRight))
	{
		//not used
		float newPanSpeed = panSpeed;
		if (pressLeft) newPanSpeed *= -1.0f;
		pan += newPanSpeed *deltaTime;
		pan = glm::clamp(pan, minPan, maxPan);
	}
	else
	{
		if (pan != 0.0f)
		{
			if (pan > 0.0f)
			{
				pan -= 2 * panSpeed *deltaTime;
				if (pan < 0)
				{
					//it went too far on its own, set it to 0
					pan = 0;
				}
			}
			else
			{
				pan += 2 * panSpeed *deltaTime;
				if (pan > 0)
				{
					//it went too far on its own, set it to 0
					pan = 0;
				}
			}
		}
	}

	UpdateView(deltaTime);
}

void cCamera::SetTargetObject(iGameObject * newTargetObject)
{
	this->targetObject = newTargetObject;
	RenderInfo targetRender = this->targetObject->GetRender();
	glm::vec3 desiredEye;
	glm::mat4x4 m = glm::mat4x4(1.0f);
	m = glm::translate(targetRender.transform, glm::vec3(offset.x, offset.y, offset.z));
	desiredEye = m[3];
	this->eye = glm::vec3(desiredEye.x, desiredEye.y, desiredEye.z);
}

void cCamera::UpdateView(float deltaTime)
{
	if (!this->targetObject)
	{
		//do free mode on player

		// set up the view matrix
		RenderInfo playerRender = gelObjectManager::sharedInstance().GetMainPlayer()->GetRender();
		glm::vec4 playerPosition = playerRender.transform[3];
		glm::vec3 up(0.f, 1.f, 0.f);
		
		glm::vec4 newoffset(offset.x, offset.y, offset.z, 1.f);
		//newoffset = glm::rotate(targetTranform, pan, up) * newoffset;
		//newoffset = glm::normalize(newoffset) * distance;

		this->at = playerPosition;
		//this->eye = glm::vec3(playerPosition.x + newoffset.x, playerPosition.y + newoffset.y, playerPosition.z + newoffset.z);

		glm::mat4x4 m = glm::mat4x4(1.0f);
		m = glm::translate(m, glm::vec3(playerPosition.x, playerPosition.y, playerPosition.z));
		glm::mat4 matRotation(playerRender.orientation);
		m = m * matRotation;
		m = glm::scale(m, glm::vec3(playerRender.lastDrawnActualScale, playerRender.lastDrawnActualScale, playerRender.lastDrawnActualScale));

		glm::vec4 desiredEye = m * newoffset;

		this->eye = glm::vec3(desiredEye.x, desiredEye.y, desiredEye.z);

		viewMatrix = glm::lookAtRH(eye, at, up); // at is target

												 //is this correct?
		glUniform3f(GelsenizeCore::sharedInstance().EyeLocationID(), eye.x, eye.y, eye.z);

		//we can move this from here and only update when we change the aspect ratio or sizew of screen.
		glUniformMatrix4fv(GelsenizeCore::sharedInstance().GetProjLocation(), 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(projMatrix));

		glUniformMatrix4fv(GelsenizeCore::sharedInstance().GetViewLocation(), 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(viewMatrix));

		return;
	}

	RenderInfo targetRender = this->targetObject->GetRender();

	glm::vec4 playerPosition = glm::vec4(targetRender.transform[3]);// targetRender.position, 0.0f);
	glm::vec3 up(0.f, 1.f, 0.f);

	glm::vec4 newoffset(offset.x, offset.y, offset.z, 1.f);
	newoffset = glm::normalize(newoffset) * distance;

	this->at = playerPosition;
	//this->at.y += distance;
	
	glm::mat4x4 m = glm::mat4x4(1.0f);
	m = glm::translate(targetRender.transform, glm::vec3(newoffset.x, newoffset.y, newoffset.z));

	glm::vec4 desiredEye = m[3];

	glm::vec3 distToTarget;
	distToTarget.x = std::abs(desiredEye.x - this->eye.x);
	distToTarget.y = std::abs(desiredEye.y - this->eye.y);
	distToTarget.z = std::abs(desiredEye.z - this->eye.z);

	// Calculate the relative speeds for each axis (could be moving fast on one axis and slow on another)
	//	using the smooth step calcualtion (GT max distance = max speed, LT min distance = 0);
	glm::vec3 moveStep;
	moveStep.x = OCHelper::smootherStep(this->followDistanceMin, this->followDistanceMax, distToTarget.x);
	moveStep.y = OCHelper::smootherStep(this->followDistanceMin, this->followDistanceMax, distToTarget.y);
	moveStep.z = OCHelper::smootherStep(this->followDistanceMin, this->followDistanceMax, distToTarget.z);

	// Scale for time step
	moveStep.x *= this->maxFollowSpeed *deltaTime;
	moveStep.y *= this->maxFollowSpeed *deltaTime;
	moveStep.z *= this->maxFollowSpeed *deltaTime;

	// If camera is LT position, reverse the moving direction (it's positive because of ABS distance)
	if (desiredEye.x < this->eye.x) { moveStep.x = -moveStep.x; }
	if (desiredEye.y < this->eye.y) { moveStep.y = -moveStep.y; }
	if (desiredEye.z < this->eye.z) { moveStep.z = -moveStep.z; }

	// If we are outside of the min distance, move the camera
	if (1 || distToTarget.x > this->followDistanceMin) { this->eye.x += moveStep.x; }
	if (1 || distToTarget.y > this->followDistanceMin) { this->eye.y += moveStep.y; }
	if (1 || distToTarget.z > this->followDistanceMin) { this->eye.z += moveStep.z; }
	
	// set up the view matrix
	
	//glm::vec4 offset(offset.x, offset.y, offset.z, 0.f);
	//offset = glm::rotate(targetTranform, pan, up) * offset;
	//offset = glm::normalize(offset) * distance;
	//glm::vec3 eye(target.x + offset.x, target.y + offset.y, target.z + offset.z);

	viewMatrix = glm::lookAtRH(eye, at, up); // at is target

	//is this correct?
	glUniform3f(GelsenizeCore::sharedInstance().EyeLocationID(), eye.x,eye.y,eye.z);

	//we can move this from here and only update when we change the aspect ratio or sizew of screen.
	glUniformMatrix4fv(GelsenizeCore::sharedInstance().GetProjLocation(), 1, GL_FALSE,
		(const GLfloat*)glm::value_ptr(projMatrix));

	glUniformMatrix4fv(GelsenizeCore::sharedInstance().GetViewLocation(), 1, GL_FALSE,
		(const GLfloat*)glm::value_ptr(viewMatrix));
}

//void cCamera::SetTargetTransform(glm::mat4& newTargetTransform)
//{
//	this->targetTranform = newTargetTransform;
//	this->at = glm::vec3(targetTranform[3].x, targetTranform[3].y, targetTranform[3].z);
//	this->eye = this->at + glm::vec3(40.0f, 30.0f, -20.0f);
//}

void cCamera::SetOffset(glm::vec3& newOffset)
{
	this->offset = newOffset;//glm::normalize(newOffset);
}

