#ifndef _GELOBJECT_BUILDER_HG_
#define _GELOBJECT_BUILDER_HG_

#include "iGameObject.h"

class gelObjectBuilder
{
public:
	//SIGNLETON
	static gelObjectBuilder& sharedInstance()
	{
		static gelObjectBuilder instance;
		return instance;
	}

	typedef enum {
		GAME_OBJECT,
		PLAYER,
		SEEKER,
		LIGHT_SOURCE
	} OBJECT_TYPE;

	iGameObject* buildSeekerObject(std::string mood, float avrgSpd);

	//not used
	iGameObject* buildWorldObject(std::string objectType);
private:
	gelObjectBuilder() {};
	gelObjectBuilder(gelObjectBuilder const&); //dont implement - to build
	void operator=(gelObjectBuilder const&); //dont implement - to build

	iGameObject* buildPlantForWorld(std::string objectType);
	iGameObject* buildAnimalForWorld(std::string objectType);
};

#endif // !_GELOBJECT_BUILDER_HG_
