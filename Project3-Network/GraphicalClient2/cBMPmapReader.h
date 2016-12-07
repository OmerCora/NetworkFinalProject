#pragma once

/*
* Program Name : INFO6017_AI_Project4 - cBMPmapReader.h
* Purpose      : INFO6017 - Artificial Intelligence Project #4
* Description  : getValuesFromBMPFile function will be called by SetUpTheScene
*			   : and it will use functions from cBMPLoader to read the bitmap information
* Date         : December 8, 2016
* Coder        : Jaehuhn Park
*/

#include "cBMPLoader.h"
#include <vector>

struct sLocation
{
	float x;
	float z;
};

class cBMPmapReader
{
public:
	bool getValuesFromBMPFile(std::string fileName);

	std::vector<int> vec_GreyID; //for Maze layout
    std::vector<int> vec_PurpleID; //for Player position
	std::vector<int> vec_YellowID; //for Goal Postion

	std::vector<sLocation> vec_BlockLocXZ;
	std::vector<sLocation> vec_PlayerLocXZ;
	std::vector<sLocation> vec_GoalLocXZ;

};

