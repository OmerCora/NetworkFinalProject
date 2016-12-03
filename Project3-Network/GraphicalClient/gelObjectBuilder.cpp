#include "gelObjectBuilder.h"
#include "gelObjectFactory.h"
#include "iWorldObjectAssign.h"
#include "iGameObjectAssign.h"

//QUESTION 5
//new types added

iGameObject * gelObjectBuilder::buildPlantForWorld(std::string objectType)
{
	iGameObject* gameObject = 0;
	if (objectType == "strawberry")
	{
		gameObject = gelObjectFactory::sharedInstance().createGameObjectForWorld("plant");

		MeshInfo mesh;
		mesh.name = "isosphere";
		gameObject->SetMesh(mesh);

		FloraInfo info;
		info.fruitColor.r = 1.0f;
		info.fruitColor.g = 0.0f;
		info.fruitColor.b = 0.0f;
		info.fruitColor.a = 1.0f;
		info.minSpawnDuration = 2.0f;
		info.maxSpawnDuration = 5.0f;
		info.generateRandomSpawnDuration();
		info.spoilDuration = 20.0f;
		info.type = objectType;
		info.calories = 100;


		dynamic_cast<iPlantAssign*>(gameObject)->AssignFloraInfo(info);

	}
	else if (objectType == "pineapple")
	{
		gameObject = gelObjectFactory::sharedInstance().createGameObjectForWorld("plant");

		MeshInfo mesh;
		mesh.name = "isosphere";
		gameObject->SetMesh(mesh);

		FloraInfo info;
		info.fruitColor.r = 1.0f;
		info.fruitColor.g = 1.0f;
		info.fruitColor.b = 0.0f;
		info.fruitColor.a = 1.0f;
		info.minSpawnDuration = 5.0f;
		info.maxSpawnDuration = 15.0f;
		info.generateRandomSpawnDuration();
		info.spoilDuration = 75.0f;
		info.type = objectType;
		info.calories = 1000;

		dynamic_cast<iPlantAssign*>(gameObject)->AssignFloraInfo(info);
	}
	else if (objectType == "watermelon")
	{
		gameObject = gelObjectFactory::sharedInstance().createGameObjectForWorld("plant");

		MeshInfo mesh;
		mesh.name = "isosphere";
		gameObject->SetMesh(mesh);

		FloraInfo info;
		info.fruitColor.r = 0.0f;
		info.fruitColor.g = 1.0f;
		info.fruitColor.b = 0.0f;
		info.fruitColor.a = 1.0f;
		info.minSpawnDuration = 10.0f;
		info.maxSpawnDuration = 25.0f;
		info.generateRandomSpawnDuration();
		info.spoilDuration = 60.0f;
		info.type = objectType;
		info.calories = 10000;

		dynamic_cast<iPlantAssign*>(gameObject)->AssignFloraInfo(info);
	}
	else if (objectType == "lettuce")
	{
		gameObject = gelObjectFactory::sharedInstance().createGameObjectForWorld("plant");

		MeshInfo mesh;
		mesh.name = "isosphere";
		gameObject->SetMesh(mesh);

		FloraInfo info;
		info.fruitColor.r = 0.0f;
		info.fruitColor.g = 0.9f;
		info.fruitColor.b = 0.1f;
		info.fruitColor.a = 1.0f;
		info.minSpawnDuration = 1.5f;
		info.maxSpawnDuration = 7.5f;
		info.generateRandomSpawnDuration();
		info.spoilDuration = 40.0f;
		info.type = objectType;
		info.calories = 250;

		dynamic_cast<iPlantAssign*>(gameObject)->AssignFloraInfo(info);
	}

	if (gameObject != 0)
	{
		RenderInfo render = gameObject->GetRender();
		render.scale = 8.0f;
		gameObject->SetRender(render);
	}

	return gameObject;
}

iGameObject * gelObjectBuilder::buildAnimalForWorld(std::string objectType)
{
	iGameObject* gameObject = 0;
	if (objectType == "bunny")
	{
		gameObject = gelObjectFactory::sharedInstance().createGameObjectForWorld("animal");

		MeshInfo mesh;
		mesh.name = "bunny";
		gameObject->SetMesh(mesh);

		RenderInfo render = gameObject->GetRender();
		render.color.r = 0.51f;
		render.color.g = 0.44f;
		render.color.b = 1.0f;
		render.color.a = 1.0f;
		render.scale = 12.0f;
		gameObject->SetRender(render);

		FaunaInfo info;
		info.initialCalories = 10000.0f;
		info.currentCalories = info.initialCalories;
		info.reproductionDuration = 60.0f;
		info.type = objectType;

		info.foodPreference.push_back(FaunaInfo::FoodPreference("strawberry", 1));
		info.foodPreference.push_back(FaunaInfo::FoodPreference("pineapple", 1));
		info.foodPreference.push_back(FaunaInfo::FoodPreference("watermelon", 1));

		dynamic_cast<iAnimalAssign*>(gameObject)->AssignFaunaInfo(info);
	}
	else if (objectType == "fox")
	{
		gameObject = gelObjectFactory::sharedInstance().createGameObjectForWorld("animal");

		MeshInfo mesh;
		mesh.name = "bunny";
		gameObject->SetMesh(mesh);

		RenderInfo render = gameObject->GetRender();
		render.color.r = 0.89f;
		render.color.g = 0.47f;
		render.color.b = 0.20f;
		render.color.a = 1.0f;
		render.scale = 12.0f;
		gameObject->SetRender(render);

		FaunaInfo info;
		info.initialCalories = 25000.0f;
		info.currentCalories = info.initialCalories;
		info.reproductionDuration = 75.0f;
		info.type = objectType;

		info.foodPreference.push_back(FaunaInfo::FoodPreference("bunny", 1));
		info.foodPreference.push_back(FaunaInfo::FoodPreference("pineapple", 2));

		dynamic_cast<iAnimalAssign*>(gameObject)->AssignFaunaInfo(info);

	}
	else if (objectType == "tiger")
	{
		gameObject = gelObjectFactory::sharedInstance().createGameObjectForWorld("animal");

		MeshInfo mesh;
		mesh.name = "bunny";
		gameObject->SetMesh(mesh);

		RenderInfo render = gameObject->GetRender();
		render.color.r = 0.0f;
		render.color.g = 0.0f;
		render.color.b = 1.0f;
		render.color.a = 1.0f;
		render.scale = 12.0f;
		gameObject->SetRender(render);

		FaunaInfo info;
		info.initialCalories = 500000.0f;
		info.currentCalories = info.initialCalories;
		info.reproductionDuration = 200.0f;
		info.type = objectType;

		info.foodPreference.push_back(FaunaInfo::FoodPreference("bunny", 1));
		info.foodPreference.push_back(FaunaInfo::FoodPreference("fox", 2));
		info.foodPreference.push_back(FaunaInfo::FoodPreference("strawberry", 3));

		dynamic_cast<iAnimalAssign*>(gameObject)->AssignFaunaInfo(info);

	}
	else if (objectType == "unicorn")
	{
		gameObject = gelObjectFactory::sharedInstance().createGameObjectForWorld("animal");

		MeshInfo mesh;
		mesh.name = "bunny";
		gameObject->SetMesh(mesh);

		RenderInfo render = gameObject->GetRender();
		render.color.r = 1.0f;
		render.color.g = 1.0f;
		render.color.b = 1.0f;
		render.color.a = 1.0f;
		render.scale = 15.0f;
		gameObject->SetRender(render);

		FaunaInfo info;
		info.initialCalories = 300000.0f;
		info.currentCalories = info.initialCalories;
		info.reproductionDuration = 125.0f;
		info.type = objectType;

		info.foodPreference.push_back(FaunaInfo::FoodPreference("lettuce", 1));
		info.foodPreference.push_back(FaunaInfo::FoodPreference("pineapple", 1));

		dynamic_cast<iAnimalAssign*>(gameObject)->AssignFaunaInfo(info);

	}


	return gameObject;
}

iGameObject* gelObjectBuilder::buildSeekerObject(std::string mood, float avrgSpd)
{
	iGameObject* gameObject = 0;
	gameObject = gelObjectFactory::sharedInstance().createGameObject("seeker");

	dynamic_cast<iGameObjectAssign*>(gameObject)->AssignMoodAndSpeed(mood, avrgSpd);

	//additional setup maybe
	if (mood == "curious")
	{
		
	}
	else if (mood == "angry")
	{

	}
	else
	{

	}
	return gameObject;
}

//for outside loop easier to use
iGameObject * gelObjectBuilder::buildWorldObject(std::string objectType)
{
	if (
		objectType == "strawberry" ||
		objectType == "pineapple" ||
		objectType == "watermelon" ||
		objectType == "lettuce")
	{
		return this->buildPlantForWorld(objectType);
	}
	else if (
		objectType == "bunny" ||
		objectType == "fox" ||
		objectType == "tiger" ||
		objectType == "unicorn")
	{
		return this->buildAnimalForWorld(objectType);
	}
	return 0;
}
