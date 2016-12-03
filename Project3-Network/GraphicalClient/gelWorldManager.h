#ifndef _GELWORLDMANAGER_HG_
#define _GELWORLDMANAGER_HG_

#include "iManager.h"
#include "iGameObject.h"

class gelWorldManager : public iManager
{
public:
	//Singleton 
	static gelWorldManager& sharedInstance()
	{
		static gelWorldManager instance;
		return instance;
	}

	//move to the next "frame" of simulation which is a single discreet slice of the "world".
	void Tick(float deltaTime);

	//update non-simulation related maybe
	virtual void Update(float deltaTime);
	void Setup();
	bool FindNearestPriorityFood(int ownerID, std::string type, glm::vec3 &foodPosition);

private:
	gelWorldManager() {};
	gelWorldManager(gelWorldManager const&); //dont implement - to build
	void operator=(gelWorldManager const&); //dont implement - to build

	int tickCount;
	int bunnyCount;
	int foxCount;
	int tigerCount;
	int unicornCount;
	int totalEaten;
	int totalCaloriesConsumed;
};

#endif // !_GELWORLDMANAGER_HG_
