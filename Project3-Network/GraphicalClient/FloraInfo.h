#ifndef _FAUNAINFO_HG_
#define _FAUNAINFO_HG_

#include <string>
#include "externals.h"

class FloraInfo
{
public:
	FloraInfo();
	~FloraInfo();
	glm::vec4 fruitColor;
	bool hasFruit;
	float calories;
	float minSpawnDuration;
	float maxSpawnDuration;
	float spoilDuration;
	std::string type;

	void generateRandomSpawnDuration();

	float acc_spawn;
	float acc_spoil;
	float spawnDuration;

private:

};


#endif // !_FAUNEINFO_HG_
