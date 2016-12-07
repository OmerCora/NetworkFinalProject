/*
* Program Name : Project02 - cValueReader.h
* Purpose      : INFO-6028 Graphics 1 Project2
* Description  : Object to read the values from the description text file
*                to be applied for graphic attributions.
* Date         : Nov. 17, 2016
* Coder        : Jaehuhn Park
*/
#ifndef _cValueReader_HG_
#define _cValueReader_HG_

#include <string>

class cValueReader 
{
public:
	bool getValuesFromTxtFile(std::string fileName);

	std::string Obj1PlyfileName;
	std::string Obj2PlyfileName;
	std::string Obj3PlyfileName;
	std::string Obj4PlyfileName;
	std::string Obj5PlyfileName;
	std::string Obj6PlyfileName;
	std::string Obj7PlyfileName;

	std::string Obj1TexturefileName;
	std::string Obj2TexturefileName;
	std::string Obj3TexturefileName;
	std::string Obj4TexturefileName;
	std::string Obj5TexturefileName;
	std::string Obj6TexturefileName;
	std::string Obj7TexturefileName;

	float Obj1ScaleXYZ;
	float Obj2ScaleXYZ;
	float Obj3ScaleXYZ;
	float Obj4ScaleXYZ;
	float Obj5ScaleXYZ;
	float Obj6ScaleXYZ;
	float Obj7ScaleXYZ;

	float Obj1PositionX;
	float Obj1PositionY;
	float Obj1PositionZ;
	float Obj2PositionX;
	float Obj2PositionY;
	float Obj2PositionZ;
	float Obj3PositionX;
	float Obj3PositionY;
	float Obj3PositionZ;
	float Obj4PositionX;
	float Obj4PositionY;
	float Obj4PositionZ;
	float Obj5PositionX;
	float Obj5PositionY;
	float Obj5PositionZ;
	float Obj6PositionX;
	float Obj6PositionY;
	float Obj6PositionZ;
	float Obj7PositionX;
	float Obj7PositionY;
	float Obj7PositionZ;

	float InitalCameraX;
	float InitalCameraY;
	float InitalCameraZ;

	float LightPositionX[24];
	float LightPositionY[24];
	float LightPositionZ[24];

};



#endif // !_cValueReader_HG_
