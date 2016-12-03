#ifndef _CCAMERA_HG_
#define _CCAMERA_HG_

#include "externals.h"
#include "iGameObject.h"

class cCamera
{
public:
	cCamera();
	~cCamera();

	void GetProjectionMatrix(glm::mat4& projOut);
	void GetViewMatrix(glm::mat4& viewOut);
	void GetEyePosition(glm::vec4& eyeOut);
	void SetEyePosition(glm::vec4& eyeIn);
	void WindowResize(int width, int height);

	void Update(float deltaTime);
	void SetTargetObject(iGameObject* newTargetObject);
	void SetOffset(glm::vec3& offset);
private:
	iGameObject* targetObject;

	int width;
	int height;
	float maxFollowSpeed;
	float followDistanceMin;
	float followDistanceMax;
	glm::vec3 eye;
	glm::vec3 at;
	glm::mat4 targetTranform;

	glm::vec3 offset;
	float zoomSpeed;
	float panSpeed;
	float pan;
	float maxPan;
	float minPan;
	float distance;
	float maxDistance;
	float minDistance;
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;
	void cCamera::UpdateView(float deltaTime);
};

#endif // !_CCAMERA_HG_

