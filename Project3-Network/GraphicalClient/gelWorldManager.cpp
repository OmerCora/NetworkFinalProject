#include "gelWorldManager.h"

#include "managers.h"
#include "GelsenizeCore.h"
#include "iWorldObject.h"

#include <sstream>
#include <vector>
extern int g_tickCount;
extern bool g_simulationOver;

void gelWorldManager::Tick(float deltaTime)
{
	tickCount++;
	std::vector<iWorldObject*> animals;
	std::vector<iWorldObject*> plants;

	std::vector<iGameObject*> gameObjects = gelObjectManager::sharedInstance().GetGameObjects();
	std::vector<iWorldObject*> worldObjects = gelObjectManager::sharedInstance().GetWorldObjects(); //mirror
	for (int index = 0; index != worldObjects.size(); index++)
	{
		iWorldObject* gameObject = (worldObjects[index]);

		std::string type = gameObject->GetObjectType();

		//maybe turn this into keys of the map of core
		if (
			type == "strawberry" ||
			type == "pineapple" ||
			type == "watermelon" ||
			type == "bunny" ||
			type == "fox" ||
			type == "tiger" ||
			type == "lettuce" ||
			type == "unicorn") //this needs to reuse code, or read from file
		{
			//for question 2 we only implement plant behaviour
			(worldObjects[index])->Tick(deltaTime);
		}
		else
		{
			//not world object
		}
	}

	//at every tick:
	// 1- animals lose calories
	// 2- for animals below 50%, 
	//		find food-> 
	//		move to food-> 
	//		eat food-> 
	//		absorb calories->
	//		remove eaten plants&animals->
	//  
	// 3- Animals can reproduce. deliver tick to all world objects

	// 4- Plants can grow fruits.    


}

void gelWorldManager::Setup()
{
	totalCaloriesConsumed = 0;
	totalEaten = 0;
	tickCount = 0;
}


void gelWorldManager::Update(float deltaTime)
{
	bunnyCount = 0;
	foxCount = 0;
	tigerCount = 0;
	unicornCount = 0;
	std::vector<iWorldObject*> worldObjects = gelObjectManager::sharedInstance().GetWorldObjects();
	float minDistance = 10000.0f;
	for (int index = 0; index != worldObjects.size(); index++)
	{
		float a;
		iWorldObject* worldObject = worldObjects[index];
		std::string type = worldObject->GetObjectType();
		if (worldObject->AskQuestion("alive", a))
		{
			if (type == "bunny")
			{
				bunnyCount++;
			}
			else if (type == "fox")
			{
				foxCount++;
			}
			else if (type == "tiger")
			{
				tigerCount++;
			}
			else if (type == "unicorn")
			{
				unicornCount++;
			}
		}
	}
	//somehow the stats are bugged
	if (unicornCount + tigerCount + foxCount + bunnyCount == 0)
	{
		//g_simulationOver = true;
	}

	std::stringstream ssTitle;
	ssTitle << "Ticks Elapsed: "
		<<  tickCount//g_tickCount
		<< "  Bunny: " << bunnyCount
		<< "  Fox: " << foxCount
		<< "  Tiger: " << tigerCount
		<< "  Unicorn: " << unicornCount
		<< "  Total Eaten: " << totalEaten
		<< "  Total Calories: " << totalCaloriesConsumed;
	glfwSetWindowTitle(GelsenizeCore::sharedInstance().GetWindow(), ssTitle.str().c_str());
}

bool gelWorldManager::FindNearestPriorityFood(int ownerID, std::string type, glm::vec3 &foodPosition)
{
	bool found = false;
	glm::vec3 foundPosition;

	iGameObject* ownerObject = gelObjectManager::sharedInstance().FindGameObjectWithID(ownerID);
	//std::vector<iGameObject*> gameObjects = gelObjectManager::sharedInstance().GetGameObjects();
	iWorldObject* eatenObject = 0;
	std::vector<iWorldObject*> worldObjects = gelObjectManager::sharedInstance().GetWorldObjects();
	float minDistance = 10000.0f;
	for (int index = 0; index != worldObjects.size(); index++)
	{
		iWorldObject* worldObject = worldObjects[index];
		//int objectID = worldObject->getObjectID();

		//get both position
		glm::vec3 position = worldObject->GetPosition();
		glm::vec3 ownerPosition = glm::vec3(ownerObject->GetRender().position.x, ownerObject->GetRender().position.y, ownerObject->GetRender().position.z);

		if (worldObject->GetObjectType() == type)
		{
			float askFor = 0.0f;
			if (worldObject->AskQuestion("edible", askFor))
			{
				//wanted type, calc distance
				float distance = glm::distance(position, ownerPosition);
				if (distance < minDistance)
				{
					//new min distance
					minDistance = distance;
					foundPosition = position;
					found = true;
					eatenObject = worldObject;
				}
			}
		}
	}
	if (found)
	{
		foodPosition = foundPosition;
		eatenObject->ReceiveMessage("eaten", 0.0f);
		totalEaten++;
		float calories = 0.0f;
		if (eatenObject->AskQuestion("calories", calories))
		{
			dynamic_cast<iWorldObject*>(ownerObject)->ReceiveMessage("consume", calories);
			totalCaloriesConsumed += (int)calories;
		}
	}

	//found id
	//ReceiveMessage


	return found;
}