#ifndef _GELOBJECTFACTORY_HG_
#define _GELOBJECTFACTORY_HG_

#include "iGameObject.h"

class gelObjectFactory
{
public:
	//SIGNLETON
	static gelObjectFactory& sharedInstance()
	{
		static gelObjectFactory instance;
		return instance;
	}

	typedef enum {
		GAME_OBJECT,
		PLAYER,
		SEEKER,
		LIGHT_SOURCE,
		//PARTICLE //maybe emitter
	} OBJECT_TYPE;
	
	//new method for simulation
	iGameObject* createGameObjectForWorld(std::string objectType);

	iGameObject* createGameObject(OBJECT_TYPE objectType);
	iGameObject* createGameObject(std::string objectType);
private:
	gelObjectFactory() {};
	gelObjectFactory(gelObjectFactory const&); //dont implement - to build
	void operator=(gelObjectFactory const&); //dont implement - to build

};

#endif // !_GELOBJECTFACTORY_HG_
