/*
* Program Name : Project02 - cValueReader.cpp
* Purpose      : INFO-6028 Graphics 1 Project2
* Description  : To read the values from the description text file 
*                to be applied for graphic attributions.
* Date         : Nov. 17, 2016
* Coder        : Jaehuhn Park
*/

#include "cValueReader.h"
#include <fstream>
#include <iostream>

bool cValueReader::getValuesFromTxtFile(std::string fileName)
{
	std::ifstream theFile(fileName);

	if (!theFile.is_open())
	{
		std::cout << "Can't find the file..." << std::endl;
		return false;
	}

	std::string tempString;
	bool bKeepReading;

	/*********Reading the values of the scene object1 **********/
	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj1PlyfileName:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj1PlyfileName;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj1TexturefileName:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj1TexturefileName;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj1ScaleXYZvalue:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj1ScaleXYZ;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj1PositionX:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj1PositionX;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj1PositionY:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj1PositionY;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj1PositionZ:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj1PositionZ;

	/*********Reading the values of the scene object2 **********/
	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj2PlyfileName:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj2PlyfileName;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj2TexturefileName:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj2TexturefileName;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj2ScaleXYZvalue:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj2ScaleXYZ;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj2PositionX:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj2PositionX;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj2PositionY:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj2PositionY;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj2PositionZ:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj2PositionZ;

	/*********Reading the values of the scene object3 **********/
	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj3PlyfileName:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj3PlyfileName;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj3TexturefileName:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj3TexturefileName;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj3ScaleXYZvalue:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj3ScaleXYZ;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj3PositionX:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj3PositionX;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj3PositionY:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj3PositionY;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj3PositionZ:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj3PositionZ;

	/*********Reading the values of the scene object4 **********/
	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj4PlyfileName:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj4PlyfileName;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj4TexturefileName:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj4TexturefileName;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj4ScaleXYZvalue:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj4ScaleXYZ;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj4PositionX:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj4PositionX;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj4PositionY:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj4PositionY;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj4PositionZ:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj4PositionZ;

	/*********Reading the values of the scene object5 **********/
	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj5PlyfileName:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj5PlyfileName;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj5TexturefileName:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj5TexturefileName;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj5ScaleXYZvalue:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj5ScaleXYZ;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj5PositionX:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj5PositionX;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj5PositionY:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj5PositionY;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj5PositionZ:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj5PositionZ;


	/*********Reading the values of the scene object6 **********/
	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj6PlyfileName:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj6PlyfileName;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj6TexturefileName:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj6TexturefileName;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj6ScaleXYZvalue:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj6ScaleXYZ;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj6PositionX:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj6PositionX;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj6PositionY:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj6PositionY;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj6PositionZ:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj6PositionZ;
	

	/*********Reading the values of the scene object7 **********/
	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj7PlyfileName:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj7PlyfileName;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj7TexturefileName:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj7TexturefileName;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj7ScaleXYZvalue:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj7ScaleXYZ;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj7PositionX:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj7PositionX;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj7PositionY:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj7PositionY;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "Obj7PositionZ:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->Obj7PositionZ;


	/*************Reading the values of the camera inital position ***************/
	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "InitalCameraX:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->InitalCameraX;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "InitalCameraY:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->InitalCameraY;

	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> tempString;
		if (tempString == "InitalCameraZ:")
		{
			bKeepReading = false;
		}
	}
	theFile >> this->InitalCameraZ;


	/******From now on just reading the values of the Lights positions **********/

	for (int i = 0; i < 12; i++) //total number of lights = 12
	{
		theFile >> tempString;
		theFile >> this->LightPositionX[i];
		theFile >> tempString;
		theFile >> this->LightPositionY[i];
		theFile >> tempString;
		theFile >> this->LightPositionZ[i];
	}

	

	return true;

}//end of getValuesFromTxtFile function
