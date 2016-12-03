//Omer Cora, 2016

#include "GelsenizeCore.h"
#include "managers.h"

int main(void)
{
	//Lazy initialize main game object
	GelsenizeCore::sharedInstance();
	//Lazy initialize managers
	gelMeshManager      ::sharedInstance();
	gelRenderManager    ::sharedInstance();
	gelObjectManager    ::sharedInstance();
	gelCameraManager    ::sharedInstance();
	gelShaderManager    ::sharedInstance();
	gelSoundManager     ::sharedInstance();
	gelLightManager     ::sharedInstance();
	GelTextureManager   ::sharedInstance(); //all will look like this 'g' -> 'G'
	GelCommandManager   ::sharedInstance();
	GelCollisionManager ::sharedInstance();

	//Start game loop
	GelsenizeCore::sharedInstance().StartGameLoop();

	return 0; //unreachable
}