#include "FloraInfo.h"

#include "OCHelper.h"

FloraInfo::FloraInfo()
{
	hasFruit = false;
	calories = 0.0f;
	minSpawnDuration = 0.0f;
	maxSpawnDuration = 0.0f;
	spoilDuration = 0.0f;
	type = "n/a";

	spawnDuration = 0.0f;
	acc_spawn = 0.0f;
	acc_spoil = 0.0f;
}

FloraInfo::~FloraInfo()
{
}

void FloraInfo::generateRandomSpawnDuration()
{
	this->acc_spawn = 0.0f;
	this->spawnDuration = OCHelper::getRandFloat(this->minSpawnDuration, this->maxSpawnDuration);
}
