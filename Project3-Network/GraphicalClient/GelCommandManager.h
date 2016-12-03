#ifndef _GELCOMMANDMANAGER_HG_
#define _GELCOMMANDMANAGER_HG_

#include "iManager.h"
#include "cCommandGroup.h"
#include "iGameObject.h"

#include <queue>

class GelCommandManager : public iManager
{
public:
	//Singleton 
	static GelCommandManager& sharedInstance()
	{
		static GelCommandManager instance;
		return instance;
	}

	void AddCommandGroup(cCommandGroup CG);
	
	virtual void Update(float deltaTime);


private:
	GelCommandManager() {};
	GelCommandManager(GelCommandManager const&); //dont implement - to build
	void operator=(GelCommandManager const&); //dont implement - to build

	std::queue<cCommandGroup> commandGroupQueue;
};

#endif // !_GELCAMERAMANAGER_HG_
