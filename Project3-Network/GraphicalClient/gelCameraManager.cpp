#include "gelCameraManager.h"

void gelCameraManager::AttachToGameObject(iGameObject * gameObject)
{
	//this->mainCamera->SetTargetTransform(gameObject->GetRender().transform);
	this->mainCamera->SetTargetObject(gameObject);
}

void gelCameraManager::Update(float deltaTime)
{
	//update camera
	this->mainCamera->Update(deltaTime);
}
