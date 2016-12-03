#ifndef _IBUILDERASSIGN_HG_
#define _IBUILDERASSIGN_HG_

#include "FaunaInfo.h"
#include "FloraInfo.h"

class iAnimalAssign
{
public:
	virtual ~iAnimalAssign() {}

	virtual void AssignFaunaInfo(FaunaInfo info) = 0;
private:

};

class iPlantAssign
{
public:
	virtual ~iPlantAssign() {}

	virtual void AssignFloraInfo(FloraInfo info) = 0;
private:

};


#endif // !_IBUILDERASSIGN_HG_
