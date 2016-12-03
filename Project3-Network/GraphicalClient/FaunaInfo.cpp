#include "FaunaInfo.h"

FaunaInfo::FaunaInfo()
{
	acc_reproduce = 0.0f;
	dead = false;
}

FaunaInfo::~FaunaInfo()
{
}


/* overload the less-than operator so priority queues know how to compare two Height objects */
bool FaunaInfo::FoodPreference::operator<(const FoodPreference& right) const
{
	return this->priority > right.priority;
}