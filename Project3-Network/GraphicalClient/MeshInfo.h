#ifndef _MESHINFO_HG_
#define _MESHINFO_HG_

#include "GelStructures.h"
#include <string>

class MeshInfo
{
public:
	MeshInfo();
	~MeshInfo();

	int ID;				     	// Which model we are drawing
	std::string name;		// "bunny"
	std::string textureName_0;
	std::string textureName_1;
	std::string textureName_2;
	std::string textureName_3;
	int usedTextureNum;
	////this will go to its own class in the future
	//float soundCooldown;
	//float soundCooldown_delta;

private:

};



#endif // !_MESHINFO_HG_
