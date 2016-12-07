/*
* Program Name : INFO6017_AI_Project4 - cBMPmapReader.cpp
* Purpose      : INFO6017 - Artificial Intelligence Project #4
* Description  : getValuesFromBMPFile function is implemented
*			   : The bitmap information read will be converted to 
*              :  x and z coordinates of certain objects in the scene judged by color 
* Date         : December 8, 2016
* Coder        : Jaehuhn Park
*/
#include "cBMPmapReader.h"
#include <iostream>

bool cBMPmapReader::getValuesFromBMPFile(std::string fileName)
{
	//instantiate a cBMPLoader object to call LoadBMP function
	//LoadBMP will read the BMP file and store the pixel information
	cBMPLoader bmpLoader;
	if (!bmpLoader.LoadBMP(fileName))
	{
		std::cout << "Something went wrong with the file " << fileName << std::endl;
		return false;
	}

	int mumOfPixel = bmpLoader.m_NumPixel;

	for (int index = 0; index < mumOfPixel; index++)
	{
		if ((int)bmpLoader.m_p_theImages[index].bluePixel == 128)
			vec_GreyID.push_back(index);
		if ((int)bmpLoader.m_p_theImages[index].greenPixel == 0)
			vec_PurpleID.push_back(index);
		if ((int)bmpLoader.m_p_theImages[index].bluePixel == 0)
			vec_YellowID.push_back(index);
	}

	//for curious enemy locaion
	for (size_t i = 0; i < vec_GreyID.size(); i++)
	{
		sLocation tempLoc;
		//the area is 50X50 wide and the BMP file is 50X50 pixels. 
		//x value is multiplied by negative 1 because the camera is looking at it from negative position
		tempLoc.x = static_cast<float>((vec_GreyID[i] % 50 - 25) * -1);
		tempLoc.z = static_cast<float>((vec_GreyID[i] - 1200) / 50);
		vec_BlockLocXZ.push_back(tempLoc);
	}

	//for Player position
	for (size_t i = 0; i < vec_PurpleID.size(); i++)
	{
		sLocation tempLoc;
		//50 pixels in a row and 25 is the center(0 x-coordinate)
		tempLoc.x = static_cast<float>((vec_PurpleID[i] % 50 - 25) * -1);
		//50 pixels in column way and 1225 is at the center(0,0)
		tempLoc.z = static_cast<float>((vec_PurpleID[i] - 1200) / 50);
		vec_PlayerLocXZ.push_back(tempLoc);
	}

	//for Goal Postion
	for (size_t i = 0; i < vec_YellowID.size(); i++)
	{
		sLocation tempLoc;
		tempLoc.x = static_cast<float>((vec_YellowID[i] % 50 - 25) * -1); 
		tempLoc.z = static_cast<float>((vec_YellowID[i] - 1200)/50);
		vec_GoalLocXZ.push_back(tempLoc);
	}


	
	return true;
}
