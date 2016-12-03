#ifndef _GELCAMERAMANAGER_HG_
#define _GELCAMERAMANAGER_HG_

#include "iManager.h"
#include "cCamera.h"
#include "iGameObject.h"

class gelCameraManager : public iManager
{
public:
	//Singleton 
	static gelCameraManager& sharedInstance()
	{
		static gelCameraManager instance;
		return instance;
	}
	
	void AttachToGameObject(iGameObject* gameObject);

	virtual void Update(float deltaTime);

	cCamera *mainCamera;

private:
	gelCameraManager() {};
	gelCameraManager(gelCameraManager const&); //dont implement - to build
	void operator=(gelCameraManager const&); //dont implement - to build


};

#endif // !_GELCAMERAMANAGER_HG_
