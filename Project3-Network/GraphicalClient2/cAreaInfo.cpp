#include "cAreaInfo.h"
//#include "GlobalStuff.h"

cAreaInfo::cAreaInfo()
{
	//initialize the member variables with information from external file
	this->Max.x = 25.0f;
	this->Max.y = 10.0f;
	this->Max.z = 25.0f;
	this->Min.x = -25.0f;
	this->Min.y = -10.0f;
	this->Min.z = -25.0f;
}

cAreaInfo::~cAreaInfo()
{

}

float cAreaInfo::getLocationRange()
{
	float widthOfOneSide = this->Max.x - this->Min.x;
	int numOfLocsInOneSide = 13; // 9 locations + 2 coners(x2 width) 
	float locationRange = widthOfOneSide / numOfLocsInOneSide;

	return locationRange;
}
