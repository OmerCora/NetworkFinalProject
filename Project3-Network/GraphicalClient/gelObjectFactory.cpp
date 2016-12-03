#include "gelObjectFactory.h"

//game objects
#include "cGameObject.h"
#include "cPlayer.h"
#include "cSeeker.h"
#include "cLightSource.h"

//#include "cParticle.h"

//new objects
#include "cAnimal.h"
#include "cPlant.h"

iGameObject * gelObjectFactory::createGameObjectForWorld(std::string objectType)
{
	iGameObject* gameObject = 0;
	if (objectType == "animal")
	{
		gameObject = new cAnimal();
	}
	else if (objectType == "plant")
	{
		gameObject = new cPlant();
	}
	return gameObject;
}

iGameObject * gelObjectFactory::createGameObject(OBJECT_TYPE objectType)
{
	iGameObject* gameObject = 0;
	if (objectType == OBJECT_TYPE::GAME_OBJECT)
	{
		gameObject = new cGameObject();
	}
	else if (objectType == OBJECT_TYPE::SEEKER)
	{
		gameObject = new cSeeker();
	}
	else if (objectType == OBJECT_TYPE::PLAYER)
	{
		gameObject = new cPlayer();
	}
	else if (objectType == OBJECT_TYPE::LIGHT_SOURCE)
	{
		gameObject = new cLightSource();
	}
	/*else if (objectType == OBJECT_TYPE::PARTICLE)
	{
		gameObject = new cParticle();
	}*/
	return gameObject;
}

iGameObject * gelObjectFactory::createGameObject(std::string objectType)
{
	iGameObject* gameObject = 0;
	if (objectType == "object")
	{
		gameObject = new cGameObject();
	}
	else if (objectType == "seeker")
	{
		gameObject = new cSeeker();
	}
	else if (objectType == "player")
	{
		gameObject = new cPlayer();
	}
	else if (objectType == "light_source")
	{
		gameObject = new cLightSource();
	}
	/*else if (objectType == "particle")
	{
		gameObject = new cParticle();
	}*/
	return gameObject;
}

