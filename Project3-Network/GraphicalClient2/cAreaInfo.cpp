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