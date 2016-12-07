// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.on.ca
// It may be distributed under the terms of the General Public License:
// http://www.fsf.org/licenses/gpl.html
// Use this code at your own risk. It is indented only as a learning aid.
//
#include "CPlyFile5nt.h"

#include "../CHRTimer.h"

#include <fstream>
// *********************
// **** If you are using the DirectX methods, add these... ****
//#include <d3d9.h>
//#include <D3dx9math.h>
//#include <d3d10.h>
//#include <d3dx10.h>
//#include <D3D11.h>
//#include <xnamath.h>

#include <sstream>
#include <math.h>
#include <algorithm>

// Written by Michael Feeney, Fanshawe College, 2009
// mfeeney@fanshawec.ca
// It may be distributed under the terms of the General Public License:
// http://www.fsf.org/licenses/gpl.html
// Use this code at your own risk. It is indented only as a learning aid.

// Very simply ply reader that also calculates the normal for the model.
//
// See Greg Turk's paper and details of the bunny here:
// http://www.cc.gatech.edu/~turk/bunny/bunny.html

const float PI = 3.141592653589793238462643383279502884197f;	// Yes, yes, I *know* it's too big... whatever.

CPlyFile5nt::CPlyFile5nt() 
{
	this->m_minX = this->m_maxX = this->m_deltaX = 0.0f;
	this->m_minY = this->m_maxY = this->m_deltaY = 0.0f;
	this->m_minZ = this->m_maxZ = this->m_deltaZ = 0.0f;
	this->m_centreX = this->m_centreY = this->m_centreZ = 0.0f;
	//
	this->m_ID = statNextID++;

	this->m_b_ScaleRGBA_OneByteValuesToFloatZeroToOne = true;
	float m_minRoundToZero = CDataReader::DEFAULTROUNDSMALLFLOATTOZEROVALUE;
	this->m_bNewRoundToZeroFlag = true; 

	//
	//this->m_numberOfVertices = 0;
	//this->m_numberOfElements = 0;
	//
	//// Added March 14, 2012
	//this->m_bHadNormalsInFile = false;
	//this->m_bHadTextureCoordinatesInFile = false;
	//this->m_totalProperties = INT_MAX;
	//this->m_x_propertyIndex = INT_MAX;
	//this->m_y_propertyIndex = INT_MAX;
	//this->m_z_propertyIndex = INT_MAX;
	//this->m_normx_propertyIndex = INT_MAX;
	//this->m_normy_propertyIndex = INT_MAX;
	//this->m_normz_propertyIndex = INT_MAX;
	//this->m_red_propertyIndex = INT_MAX;
	//this->m_green_propertyIndex = INT_MAX;
	//this->m_blue_propertyIndex = INT_MAX;
	//this->m_tex0u_propertyIndex = INT_MAX;
	//this->m_tex0v_propertyIndex = INT_MAX;
	//this->m_tex1u_propertyIndex = INT_MAX;
	//this->m_tex1v_propertyIndex = INT_MAX;
	// End of Added

	this->m_fileInformation.fileType = CFileInfo::MODEL_FILE_TYPE_UNKNOWN;
}


//static 
unsigned int CPlyFile5nt::statNextID = 1;

CPlyFile5nt::~CPlyFile5nt()
{
	return;
}

CPlyFile5nt::CPlyFile5nt( const CPlyFile5nt &rhs )	// Copy constructor
{
	this->m_verticies = rhs.m_verticies;
	this->m_elements = rhs.m_elements;

	this->m_minX = rhs.m_minX;
	this->m_minY = rhs.m_minY;
	this->m_minZ = rhs.m_minZ;

	this->m_maxX = rhs.m_maxX;
	this->m_maxY = rhs.m_maxY;
	this->m_maxZ = rhs.m_maxZ;

	this->m_deltaX = rhs.m_deltaX;
	this->m_deltaY = rhs.m_deltaY;
	this->m_deltaZ = rhs.m_deltaZ;

	this->m_centreX = rhs.m_centreX;
	this->m_centreY = rhs.m_centreY;
	this->m_centreZ = rhs.m_centreZ;

	this->m_maxExtent = 0.0f;

	// Added: November 2, 2014
	this->m_PlyHeaderInfo = rhs.m_PlyHeaderInfo;


	//this->m_totalProperties = rhs.m_totalProperties;
	//this->m_x_propertyIndex = rhs.m_x_propertyIndex;
	//this->m_y_propertyIndex = rhs.m_y_propertyIndex;
	//this->m_z_propertyIndex = rhs.m_z_propertyIndex;
	//this->m_normx_propertyIndex = rhs.m_normx_propertyIndex;
	//this->m_normy_propertyIndex = rhs.m_normy_propertyIndex;
	//this->m_normz_propertyIndex = rhs.m_normz_propertyIndex;
	//this->m_red_propertyIndex = rhs.m_red_propertyIndex;
	//this->m_green_propertyIndex = rhs.m_green_propertyIndex;
	//this->m_blue_propertyIndex = rhs.m_blue_propertyIndex;

	//// Added March 14, 2012
	//this->m_bHadTextureCoordinatesInFile = rhs.m_bHadTextureCoordinatesInFile;
	//this->m_bHadNormalsInFile = rhs.m_bHadNormalsInFile;
	//this->m_tex0u_propertyIndex = rhs.m_tex0u_propertyIndex;
	//this->m_tex0v_propertyIndex = rhs.m_tex0v_propertyIndex;
	//this->m_tex1u_propertyIndex = rhs.m_tex1u_propertyIndex;
	//this->m_tex1v_propertyIndex = rhs.m_tex1v_propertyIndex;
	// End of Added
}

CPlyFile5nt& CPlyFile5nt::operator=( const CPlyFile5nt &rhs )
{
	// Check for self-assignment *IMPORTANT*
	if ( this == &rhs )	return *this;
	// Else...copy values
	this->m_verticies = rhs.m_verticies;
	this->m_elements = rhs.m_elements;

	this->m_minX = rhs.m_minX;
	this->m_minY = rhs.m_minY;
	this->m_minZ = rhs.m_minZ;

	this->m_maxX = rhs.m_maxX;
	this->m_maxY = rhs.m_maxY;
	this->m_maxZ = rhs.m_maxZ;

	this->m_deltaX = rhs.m_deltaX;
	this->m_deltaY = rhs.m_deltaY;
	this->m_deltaZ = rhs.m_deltaZ;

	this->m_centreX = rhs.m_centreX;
	this->m_centreY = rhs.m_centreY;
	this->m_centreZ = rhs.m_centreZ;

	// Added: November 2, 2014
	this->m_PlyHeaderInfo = rhs.m_PlyHeaderInfo;


	// Added: June 8, 2015
	this->m_maxExtent = rhs.m_maxExtent;


	//this->m_totalProperties = rhs.m_totalProperties;
	//this->m_x_propertyIndex = rhs.m_x_propertyIndex;
	//this->m_y_propertyIndex = rhs.m_y_propertyIndex;
	//this->m_z_propertyIndex = rhs.m_z_propertyIndex;
	//this->m_normx_propertyIndex = rhs.m_normx_propertyIndex;
	//this->m_normy_propertyIndex = rhs.m_normy_propertyIndex;
	//this->m_normz_propertyIndex = rhs.m_normz_propertyIndex;
	//this->m_red_propertyIndex = rhs.m_red_propertyIndex;
	//this->m_green_propertyIndex = rhs.m_green_propertyIndex;
	//this->m_blue_propertyIndex = rhs.m_blue_propertyIndex;

	return *this;
}

unsigned int CPlyFile5nt::GetID( void ) 
{
	return this->m_ID;
}

bool CPlyFile5nt::ReadPLYFileHeader(std::wstring fileName, std::wstring &error)
{
		error = L"OK";

	std::wifstream thePlyFile(fileName.c_str());
	if (!thePlyFile.is_open())
	{
		error = L"Can't open the file. Sorry it didn't work out.";
		return false;
	}

	std::wstring tempString;
	this->m_PlyHeaderInfo.numberOfVertices = 0;
	this->m_PlyHeaderInfo.numberOfElements = 0;

	try
	{
		bool bItsABunnyFile = true;
		// ply
		thePlyFile >> tempString;	if ( tempString != L"ply")		{ bItsABunnyFile = false; error = L"No a ply file."; throw; }
		// format ascii 1.0
		thePlyFile >> tempString;	if ( tempString != L"format")	{ bItsABunnyFile = false; throw; }
		thePlyFile >> tempString;	
		// Added: November 2, 2014
		if ( tempString == L"ascii" )
		{
			this->m_PlyHeaderInfo.plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_IS_ASCII;
		}
		else if ( tempString == L"binary_little_endian" )
		{
			bItsABunnyFile = false; 
			this->m_PlyHeaderInfo.plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_IS_BINARY_LITTLE_ENDIAN;
		}
		else if ( tempString == L"binary_big_endian" )
		{
			bItsABunnyFile = false; 
			this->m_PlyHeaderInfo.plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_IS_BINARY_BIG_ENDIAN;
		}
		else 
		{
			error = L"Format must be ascii, binary_little_endian, or binary_big_endian."; 
			throw; 
		}
		// End of: Added: November 2, 2014
		thePlyFile >> tempString;	if ( tempString != L"1.0")		{ bItsABunnyFile = false; throw; }

		// Find the number of verticies...
		// Item after "vertex" is number of verticies
		while ( thePlyFile >> tempString )
		{
			if ( tempString == L"vertex" )	break;
		}			
		thePlyFile >> this->m_PlyHeaderInfo.numberOfVertices;	// 453

		// Read the properties and note the index locations of them...
		int currentIndex = 0;
		while ( true )
		{
			thePlyFile >> tempString; 
			if ( tempString == L"element" ) { break; /* on to next part of file */ }
			// property float x
			if ( tempString == L"property") 
			{
				thePlyFile >> tempString; 
				if ( ( tempString == L"float") | ( tempString == L"uchar") )	
				{	// Figure out which index to set
					thePlyFile >> tempString;
					this->m_setIndexBasedOnPropertyName( currentIndex, tempString );
				}
				else 
				{ 
					error = L"expected property type missing"; throw; 
				} // if ( tempString == L"float")	
			}
			else
			{ 
				error = L"expected property"; throw; 
			} // if ( tempString == L"property") 
			currentIndex++;
		}
		this->m_PlyHeaderInfo.totalProperties = currentIndex;
		// 
		thePlyFile >> tempString;	if ( tempString != L"face")	{ bItsABunnyFile = false; error = L"expected element face."; throw; }
		thePlyFile >> this->m_PlyHeaderInfo.numberOfElements;	

		while ( thePlyFile >> tempString )
		{ 
			if ( tempString == L"end_header" )	break;
		}
	}
	catch (...)
	{
		error = L"Something went wrong while reading the file.";
		return false;
	}

	return true;
}


bool CPlyFile5nt::OpenPLYFile(std::wstring fileName, std::wstring &error)
{
	error = L"OK";

	std::wifstream thePlyFile(fileName.c_str());
	if (!thePlyFile.is_open())
	{
		error = L"Can't open the file. Sorry it didn't work out.";
		return false;
	}

	std::wstring tempString;
	this->m_PlyHeaderInfo.numberOfVertices = 0;
	this->m_PlyHeaderInfo.numberOfElements = 0;

	CHRTimer timer;
	timer.Reset();
	timer.Start();

	try
	{
		bool bItsABunnyFile = true;
		// ply
		thePlyFile >> tempString;	if ( tempString != L"ply")		{ bItsABunnyFile = false; error = L"No a ply file."; throw; }
		// format ascii 1.0
		thePlyFile >> tempString;	if ( tempString != L"format")	{ bItsABunnyFile = false; throw; }
		thePlyFile >> tempString;	

		// Added: November 2, 2014
		if ( tempString == L"ascii" )
		{
			this->m_PlyHeaderInfo.plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_IS_ASCII;
		}
		else if ( tempString == L"binary_little_endian" )
		{
			bItsABunnyFile = false; 
			this->m_PlyHeaderInfo.plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_IS_BINARY_LITTLE_ENDIAN;
		}
		else if ( tempString == L"binary_big_endian" )
		{
			bItsABunnyFile = false; 
			this->m_PlyHeaderInfo.plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_IS_BINARY_BIG_ENDIAN;
		}
		else 
		{
			error = L"Format must be ascii, binary_little_endian, or binary_big_endian."; 
			throw; 
		}
		// End of: Added: November 2, 2014		
		
		thePlyFile >> tempString;	if ( tempString != L"1.0")		{ bItsABunnyFile = false; throw; }

		// Find the number of verticies...
		// Item after "vertex" is number of verticies
		while ( thePlyFile >> tempString )
		{
			if ( tempString == L"vertex" )	break;
		}			
		thePlyFile >> this->m_PlyHeaderInfo.numberOfVertices;	// 453

		// Read the properties and note the index locations of them...
		int currentIndex = 0;
		while ( true )
		{
			thePlyFile >> tempString; 
			if ( tempString == L"element" ) { break; /* on to next part of file */ }
			// property float x
			if ( tempString == L"property") 
			{
				thePlyFile >> tempString; 
				if ( ( tempString == L"float") | ( tempString == L"uchar") )	
				{	// Figure out which index to set
					thePlyFile >> tempString;
					this->m_setIndexBasedOnPropertyName( currentIndex, tempString );
				}
				else 
				{ 
					error = L"expected property type missing"; throw; 
				} // if ( tempString == L"float")	
			}
			else
			{ 
				error = L"expected property"; throw; 
			} // if ( tempString == L"property") 
			currentIndex++;
		}
		this->m_PlyHeaderInfo.totalProperties = currentIndex;
		// 
		thePlyFile >> tempString;	if ( tempString != L"face")	{ bItsABunnyFile = false; error = L"expected element face."; throw; }
		thePlyFile >> this->m_PlyHeaderInfo.numberOfElements;	

		while ( thePlyFile >> tempString )
		{ 
			if ( tempString == L"end_header" )	break;
		}

		// The header is read, so determine what "type" of header it is. 
		// Note: this isn't really used for this method (but IS used for OpenPLYFile2), but hey, 
		//	why not live a little, right? 
		this->m_PlyHeaderInfo.DeterminePlyFileType();


		// Now the real work begins...
		//... a bunch of vertices...
		//-0.0312216 0.126304 0.00514924 0.850855 0.5 

		for (int vertexCount = 0; vertexCount != this->m_PlyHeaderInfo.numberOfVertices; vertexCount++)
		{
			PlyVertex tempVertex;
			// Place the data into the array...
			// Read through the properties and place them into the appropriate part of the vertex...
			for (int curPropIndex = 0; curPropIndex != this->m_PlyHeaderInfo.totalProperties; curPropIndex++)
			{
				std::wstring tempString;
				thePlyFile >> tempString;
				// Load this item based on index
				this->m_placeItemInAppropriateVertexLocation( tempString, curPropIndex, tempVertex );
			}

			// Scaling the RGBA values? 
			if ( this->m_b_ScaleRGBA_OneByteValuesToFloatZeroToOne )
			{
				tempVertex.red /= 255.0f;
				tempVertex.green /= 255.0f;
				tempVertex.blue /= 255.0f;
				tempVertex.alpha /= 255.0f;
			}//if ( this->m_b_ScaleRGBA_OneByteValuesToFloatZeroToOne )

			////thePlyFile >> tempVertex.x;
			////thePlyFile >> tempVertex.y;
			////thePlyFile >> tempVertex.z;
			////thePlyFile >> tempVertex.confidence;
			////thePlyFile >> tempVertex.intensity;
			//// Check bounding box limits...
			//if ( tempVertex.xyz.x < this->m_minX )	this->m_minX = tempVertex.xyz.x;
			//if ( tempVertex.xyz.x > this->m_maxX )	this->m_maxX = tempVertex.xyz.x;
			//if ( tempVertex.xyz.y < this->m_minY )	this->m_minY = tempVertex.xyz.y;
			//if ( tempVertex.xyz.y > this->m_maxY )	this->m_maxY = tempVertex.xyz.y;
			//if ( tempVertex.xyz.z < this->m_minZ )	this->m_minZ = tempVertex.xyz.z;
			//if ( tempVertex.xyz.z > this->m_maxZ )	this->m_maxZ = tempVertex.xyz.z;
			//this->m_deltaX = this->m_maxX - this->m_minX;
			//this->m_deltaY = this->m_maxY - this->m_minY;
			//this->m_deltaZ = this->m_maxZ - this->m_minZ;
			//// 
			//if ( this->m_deltaX > this->m_maxExtent )	this->m_maxExtent = this->m_deltaX;
			//if ( this->m_deltaY > this->m_maxExtent )	this->m_maxExtent = this->m_deltaY;
			//if ( this->m_deltaZ > this->m_maxExtent )	this->m_maxExtent = this->m_deltaZ;

			this->m_verticies.push_back(tempVertex);
		}

		timer.UpdateLongDuration();
		
		this->calcualteExtents();

		timer.UpdateLongDuration();

		// Now read the elements (the triangles...)
		//... a bunch of elements that refer to the verticies
		//3 435 423 430 
		for (int elementCount = 0; elementCount != this->m_PlyHeaderInfo.numberOfElements; elementCount++)
		{
			PlyElement tempElement;
			thePlyFile >> tempString;	// 3 
			thePlyFile >> tempElement.vertex_index_1;
			thePlyFile >> tempElement.vertex_index_2;
			thePlyFile >> tempElement.vertex_index_3;
			this->m_elements.push_back( tempElement );
		}
	}
	catch (...)
	{
		error = L"Something went wrong while reading the file.";
		return false;
	}

	this->m_fileInformation.fileName = fileName;
	this->m_fileInformation.fileType = CFileInfo::MODEL_FILE_TYPE_PLY;
	
	this->m_lastLoadOrSaveTime = static_cast<float>( timer.get_dLondDurationTotalSeconds() );

	return true;
}

void CPlyFile5nt::SetMinFloatRoundToZero(float minRoundToZero )
{
	this->m_minRoundToZero = minRoundToZero;
	return;
}

float CPlyFile5nt::GetMinFloatRoundToZeroValue(void)
{
	return this->m_minRoundToZero;
}

void CPlyFile5nt::SetRoundTinyFloatsToZeroOnLoadFlag(bool bNewRoundToZeroFlag )
{
	this->m_bNewRoundToZeroFlag = bNewRoundToZeroFlag;
	return;
}

bool CPlyFile5nt::GetRoundTinyFloatsToZeroOnLoadFlag(void)
{
	return this->m_bNewRoundToZeroFlag;
}

void CPlyFile5nt::SetScaleRGBA_OneByteValuesToFloatZeroToOne(bool bEnabled)
{
	this->m_b_ScaleRGBA_OneByteValuesToFloatZeroToOne = bEnabled;
	return;
}

bool CPlyFile5nt::GetScaleRGBA_OneByteValuesToFloatZeroToOne(void)
{
	return this->m_b_ScaleRGBA_OneByteValuesToFloatZeroToOne;
}

void CPlyFile5nt::scaleVertices( float scaleFactor )
{
	std::vector<PlyVertex>::iterator itVertex = this->m_verticies.begin();
	for ( ; itVertex != this->m_verticies.end(); itVertex++ )
	{
		itVertex->xyz.x *= scaleFactor;
		itVertex->xyz.y *= scaleFactor;
		itVertex->xyz.z *= scaleFactor;
	}
	this->calcualteExtents();
	return;
}

void CPlyFile5nt::calcualteExtents(void)
{
	this->m_minX = this->m_verticies.begin()->xyz.x;
	this->m_maxX = this->m_verticies.begin()->xyz.x;		// Fixed
	this->m_minY = this->m_verticies.begin()->xyz.y;
	this->m_maxY = this->m_verticies.begin()->xyz.y;
	this->m_minZ = this->m_verticies.begin()->xyz.z;
	this->m_maxZ = this->m_verticies.begin()->xyz.z;

	this->m_deltaX = this->m_maxX - this->m_minX;
	this->m_deltaY = this->m_maxY - this->m_minY;
	this->m_deltaZ = this->m_maxZ - this->m_minZ;

	this->m_maxExtent = this->m_deltaX;
	if ( this->m_deltaY > this->m_maxExtent )	this->m_maxExtent = this->m_deltaY;
	if ( this->m_deltaZ > this->m_maxExtent )	this->m_maxExtent = this->m_deltaZ;

	std::vector<PlyVertex>::iterator itVertex = this->m_verticies.begin();
	for ( ; itVertex != this->m_verticies.end(); itVertex++ )
	{
		// Check bounding box limits...
		if ( itVertex->xyz.x < this->m_minX )	this->m_minX = itVertex->xyz.x;
		if ( itVertex->xyz.x > this->m_maxX )	this->m_maxX = itVertex->xyz.x;
		if ( itVertex->xyz.y < this->m_minY )	this->m_minY = itVertex->xyz.y;
		if ( itVertex->xyz.y > this->m_maxY )	this->m_maxY = itVertex->xyz.y;
		if ( itVertex->xyz.z < this->m_minZ )	this->m_minZ = itVertex->xyz.z;
		if ( itVertex->xyz.z > this->m_maxZ )	this->m_maxZ = itVertex->xyz.z;
		this->m_deltaX = this->m_maxX - this->m_minX;
		this->m_deltaY = this->m_maxY - this->m_minY;
		this->m_deltaZ = this->m_maxZ - this->m_minZ;
		// 
		if ( this->m_deltaX > this->m_maxExtent )	this->m_maxExtent = this->m_deltaX;
		if ( this->m_deltaY > this->m_maxExtent )	this->m_maxExtent = this->m_deltaY;
		if ( this->m_deltaZ > this->m_maxExtent )	this->m_maxExtent = this->m_deltaZ;
	}
	return;
}


void NormalizeVector(float &x, float &y, float &z)
{
	// Convert to double so there is no loss...
	double x2 = static_cast<double>(x) * static_cast<double>(x);
	double y2 = static_cast<double>(y) * static_cast<double>(y);
	double z2 = static_cast<double>(z) * static_cast<double>(z);
	double length = x2 + y2 + z2;
	if ( length == 0 )
	{
		x = y = z = 0.0f;
	} 
	else
	{	// Multiply by reciprical as multiplies are faster than divides (usually)
		double reciprical = 1 / sqrt( length );
	x = static_cast<float>( static_cast<double>(x) * reciprical );
	y = static_cast<float>( static_cast<double>(y) * reciprical  );
	z = static_cast<float>( static_cast<double>(z) * reciprical  );
	}
}


// Takes cross of vec0 (x0, y0, z0) and vec1 (x1, y1, z1) and 
//	returns difference (xSub, ySub, zSub)
void VectorSubtract(float &x0, float &y0, float &z0, 
					float &x1, float &y1, float &z1,
					float &xSub, float &ySub, float &zSub)
{
	xSub = x0 - x1;
	ySub = y0 - y1;
	zSub = z0 - z1;
}

// Takes cross of vec0 (x0, y0, z0) and vec1 (x1, y1, z1) and 
//	returns cross (xCross, yCross, zCross)
void CrossProduct(float &x0, float &y0, float &z0, 
				  float &x1, float &y1, float &z1,
				  float &xCross, float &yCross, float &zCross)
{
    /* find cross-product between these vectors */
    xCross = y0 * z1 - z0 * y1;
    yCross = z0 * x1 - x0 * z1;
    zCross = x0 * y1 - y0 * x1;
}

// ******************************************************************************************
// ******************************************************************************************
// ******************************************************************************************
//                         _ _        _____ _        __  __        _     _ ___      _         
// _ _  ___ _ _ _ __  __ _| (_)______|_   _| |_  ___|  \/  |___ __| |___| | _ )__ _| |__ _  _ 
//| ' \/ _ \ '_| '  \/ _` | | |_ / -_) | | | ' \/ -_) |\/| / _ | _` / -_) | _ | _` | '_ \ || |
//|_||_\___/_| |_|_|_\__,_|_|_/__\___| |_| |_||_\___|_|  |_\___|__,_\___|_|___|__,_|_.__/\_, |
//                                                                                       |__/ 
void CPlyFile5nt::normalizeTheModelBaby(void)
{
	// The first part of this is the same as the 'rock-n-roll' versions,
	//	but instead of just overwriting the vertices at each face, the
	//	normalized normal is ADDED to the existing normal at that face.
	// Once we are all done, we go through each vertex an normalize them.
	// What this effectively does is get the AVERAGE of all the normals 
	//	of the faces around each vertes.
	// It's only slightly more complicated, and is more 'proper.'
	// LONG WAY version
	std::vector<PlyElement>::iterator itVecFace;
	for ( itVecFace = this->m_elements.begin(); itVecFace != this->m_elements.end(); itVecFace++)
	{	// A simple, local struct to help...
		//struct SexyVector
		//{
		//	SexyVector() : x(0.0f), y(0.0f), z(0.0f) {};
		//	float x, y, z;
		//};
		// Get the three corners (verticies) of the triangle
		//SexyVector vectorA, vectorB, vectorC;
		CVector3f vectorA, vectorB, vectorC;
		vectorA.x = this->m_verticies.at( itVecFace->vertex_index_1 ).xyz.x;
		vectorA.y = this->m_verticies.at( itVecFace->vertex_index_1 ).xyz.y;
		vectorA.z = this->m_verticies.at( itVecFace->vertex_index_1 ).xyz.z;
		vectorB.x = this->m_verticies.at( itVecFace->vertex_index_2 ).xyz.x;
		vectorB.y = this->m_verticies.at( itVecFace->vertex_index_2 ).xyz.y;
		vectorB.z = this->m_verticies.at( itVecFace->vertex_index_2 ).xyz.z;
		vectorC.x = this->m_verticies.at( itVecFace->vertex_index_3 ).xyz.x;
		vectorC.y = this->m_verticies.at( itVecFace->vertex_index_3 ).xyz.y;
		vectorC.z = this->m_verticies.at( itVecFace->vertex_index_3 ).xyz.z;
		// calculate the vectors for the cross...
		//SexyVector vecAB;// = vecB - vecA
		CVector3f vecAB;
		//::VectorSubtract( vectorB.x, vectorB.y, vectorB.z,
		//	              vectorA.x, vectorA.y, vectorA.z,
		//				  vecAB.x, vecAB.y, vecAB.z );		
		vecAB = vectorB - vectorA;
		
		//SexyVector vecBC;// = vecC - vecB
		CVector3f vecBC;
		//::VectorSubtract( vectorC.x, vectorC.y, vectorC.z,
		//	              vectorB.x, vectorB.y, vectorB.z,
		//				  vecBC.x, vecBC.y, vecBC.z );
		vecBC = vectorC - vectorB;

		//SexyVector normal;
		CVector3f normal;
		//::CrossProduct( vecAB.x, vecAB.y, vecAB.z, 
		//	            vecBC.x, vecBC.y, vecBC.z, 
		//				normal.x, normal.y, normal.z );
		normal.CrossProduct( vecAB, vecBC, normal );

		//::NormalizeVector( normal.x, normal.y, normal.z );
		normal.Normalize();

		// Load the normals onto the verticies
		// NOTE: We are ADDING instead of OVERWRITING...
		this->m_verticies.at( itVecFace->vertex_index_1 ).nx += normal.x;  //****
		this->m_verticies.at( itVecFace->vertex_index_1 ).ny += normal.y;  //****
		this->m_verticies.at( itVecFace->vertex_index_1 ).nz += normal.z;  //****
		this->m_verticies.at( itVecFace->vertex_index_2 ).nx += normal.x;  //****
		this->m_verticies.at( itVecFace->vertex_index_2 ).ny += normal.y;  //****
		this->m_verticies.at( itVecFace->vertex_index_2 ).nz += normal.z;  //****
		this->m_verticies.at( itVecFace->vertex_index_3 ).nx += normal.x;  //****
		this->m_verticies.at( itVecFace->vertex_index_3 ).ny += normal.y;  //****
		this->m_verticies.at( itVecFace->vertex_index_3 ).nz += normal.z;  //****
	}
	// Now go through all the vertices and normalize (average) them...
	std::vector<PlyVertex>::iterator itVert;
	for ( itVert = this->m_verticies.begin(); itVert != this->m_verticies.end(); itVert++)
	{	
		::NormalizeVector( itVert->nx, itVert->ny, itVert->nz );
	}
}

////                         _ _        _____ _        __  __        _     _ ___      _         ___  _            _  __  ____   __            _          
//// _ _  ___ _ _ _ __  __ _| (_)______|_   _| |_  ___|  \/  |___ __| |___| | _ )__ _| |__ _  _|   \(_)_ _ ___ __| |_\ \/ /\ \ / /___ _ _ ___(_)___ _ _  
////| ' \/ _ \ '_| '  \/ _` | | |_ / -_) | | | ' \/ -_) |\/| / _ | _` / -_) | _ | _` | '_ \ || | |) | | '_/ -_) _|  _|>  <  \ V // -_) '_(_-<| / _ \ ' \ 
////|_||_\___/_| |_|_|_\__,_|_|_/__\___| |_| |_||_\___|_|  |_\___|__,_\___|_|___|__,_|_.__/\_, |___/|_|_| \___\__|\__/_/\_\  \_/ \___|_| /__/|_\___/_||_|
////                                                                                       |__/                                                          
//void CPlyFile5nt_DX11::normalizeTheModelBabyDirectXVersion(void)
//{
//	// The first part of this is the same as the 'rock-n-roll' versions,
//	//	but instead of just overwriting the vertices at each face, the
//	//	normalized normal is ADDED to the existing normal at that face.
//	// Once we are all done, we go through each vertex an normalize them.
//	// What this effectively does is get the AVERAGE of all the normals 
//	//	of the faces around each vertes.
//	// It's only slightly more complicated, and is more 'proper.'
//	// DIRECTX Version
//	std::vector<PLYELEMENT>::iterator itVecFace;
//	for ( itVecFace = this->m_elements.begin(); itVecFace != this->m_elements.end(); itVecFace++)
//	{	
//		// Get the three corners (verticies) of the triangle
//		PLYVERTEX vertexA = this->m_verticies.at( itVecFace->vertex_index_1 );
//		PLYVERTEX vertexB = this->m_verticies.at( itVecFace->vertex_index_2 );
//		PLYVERTEX vertexC = this->m_verticies.at( itVecFace->vertex_index_3 );
//		// Now calculate the vectors...
//		XMVECTOR vectorA = { vertexA.xyz.x, vertexA.xyz.y, vertexA.xyz.z };		//D3DXVECTOR3 vectorA( vertexA.xyz.x, vertexA.xyz.y, vertexA.xyz.z );
//		XMVECTOR vectorB = { vertexB.xyz.x, vertexB.xyz.y, vertexB.xyz.z };		//D3DXVECTOR3 vectorB( vertexB.xyz.x, vertexB.xyz.y, vertexB.xyz.z );
//		XMVECTOR vectorC = { vertexC.xyz.x, vertexC.xyz.y, vertexC.xyz.z };		//D3DXVECTOR3 vectorC( vertexC.xyz.x, vertexC.xyz.y, vertexC.xyz.z );
//		// calculate the vectors for the cross...
//		XMVECTOR vecAB = XMVectorSubtract( vectorB, vectorA );		//D3DXVECTOR3 vecAB = vectorB - vectorA;
//		XMVECTOR vecBC = XMVectorSubtract( vectorC, vectorB );		//D3DXVECTOR3 vecBC = vectorC - vectorB;
//		XMVECTOR normal;		//D3DXVECTOR3 normal;
//		normal = XMVector3Cross( vecAB, vecBC );	// D3DXVec3Cross( &normal, &vecAB, &vecBC );
//		normal = XMVector3Normalize( normal );				// D3DXVec3Normalize( &normal, &normal );
//		XMFLOAT3 normalXMFLOAT3;
//		XMStoreFloat3( &normalXMFLOAT3, normal );
//		// Load the normals onto the verticies
//		// NOTE: We are ADDING instead of OVERWRITING...
//		this->m_verticies.at( itVecFace->vertex_index_1 ).nx += normalXMFLOAT3.x;  //****
//		this->m_verticies.at( itVecFace->vertex_index_1 ).ny += normalXMFLOAT3.y;  //****
//		this->m_verticies.at( itVecFace->vertex_index_1 ).nz += normalXMFLOAT3.z;  //****
//		this->m_verticies.at( itVecFace->vertex_index_2 ).nx += normalXMFLOAT3.x;  //****
//		this->m_verticies.at( itVecFace->vertex_index_2 ).ny += normalXMFLOAT3.y;  //****
//		this->m_verticies.at( itVecFace->vertex_index_2 ).nz += normalXMFLOAT3.z;  //****
//		this->m_verticies.at( itVecFace->vertex_index_3 ).nx += normalXMFLOAT3.x;  //****
//		this->m_verticies.at( itVecFace->vertex_index_3 ).ny += normalXMFLOAT3.y;  //****
//		this->m_verticies.at( itVecFace->vertex_index_3 ).nz += normalXMFLOAT3.z;  //****
//	}
//	// Now go through all the vertices and normalize (average) them...
//	std::vector<PLYVERTEX>::iterator itVert;
//	for ( itVert = this->m_verticies.begin(); itVert != this->m_verticies.end(); itVert++)
//	{	
//		XMVECTOR normal = { itVert->nx, itVert->ny, itVert->nz };	// D3DXVECTOR3 normal( itVert->nx, itVert->ny, itVert->nz );
//		normal = XMVector3Normalize( normal );		// D3DXVec3Normalize( &normal, &normal );
//		XMFLOAT3 normalXMFLOAT3;
//		XMStoreFloat3( &normalXMFLOAT3, normal );
//		itVert->nx = normalXMFLOAT3.x;
//		itVert->ny = normalXMFLOAT3.y;
//		itVert->nz = normalXMFLOAT3.z;
//	}
//}

//                         _ _        _____ _        __  __        _     _ ___      _             ___        _                ___     _ _ 
// _ _  ___ _ _ _ __  __ _| (_)______|_   _| |_  ___|  \/  |___ __| |___| | _ )__ _| |__ _  _    | _ \___ __| |__    _ _     | _ \___| | |
//| ' \/ _ \ '_| '  \/ _` | | |_ / -_) | | | ' \/ -_) |\/| / _ | _` / -_) | _ | _` | '_ \ || |   |   / _ | _| / /   | ' \    |   / _ \ | |
//|_||_\___/_| |_|_|_\__,_|_|_/__\___| |_| |_||_\___|_|  |_\___|__,_\___|_|___|__,_|_.__/\_, |___|_|_\___|__|_\_\___|_||_|___|_|_\___/_|_|
//                                                                                       |__/|___|              |___|    |___|            
void CPlyFile5nt::normalizeTheModelBaby_Rock_n_Roll(void)
{
	// Go through all the faces, calculate the normal and 
	//	save (overwrite) the normals at that faces vertices.
	// LONG WAY version
	std::vector<PlyElement>::iterator itVecFace;
	for ( itVecFace = this->m_elements.begin(); itVecFace != this->m_elements.end(); itVecFace++)
	{	// A simple, local struct to help...
		//struct SexyVector
		//{
		//	SexyVector() : x(0.0f), y(0.0f), z(0.0f) {};
		//	float x, y, z;
		//};
		// Get the three corners (verticies) of the triangle
		//SexyVector vectorA, vectorB, vectorC;
		CVector3f vectorA, vectorB, vectorC;
		vectorA.x = this->m_verticies.at( itVecFace->vertex_index_1 ).xyz.x;
		vectorA.y = this->m_verticies.at( itVecFace->vertex_index_1 ).xyz.y;
		vectorA.z = this->m_verticies.at( itVecFace->vertex_index_1 ).xyz.z;
		vectorB.x = this->m_verticies.at( itVecFace->vertex_index_2 ).xyz.x;
		vectorB.y = this->m_verticies.at( itVecFace->vertex_index_2 ).xyz.y;
		vectorB.z = this->m_verticies.at( itVecFace->vertex_index_2 ).xyz.z;
		vectorC.x = this->m_verticies.at( itVecFace->vertex_index_3 ).xyz.x;
		vectorC.y = this->m_verticies.at( itVecFace->vertex_index_3 ).xyz.y;
		vectorC.z = this->m_verticies.at( itVecFace->vertex_index_3 ).xyz.z;
		// calculate the vectors for the cross...
		//SexyVector vecAB;// = vecB - vecA
		CVector3f vecAB;
		//::VectorSubtract( vectorB.x, vectorB.y, vectorB.z,
		//	              vectorA.x, vectorA.y, vectorA.z,
		//				  vecAB.x, vecAB.y, vecAB.z );		
		vecAB = vectorB - vectorA;
		
		//SexyVector vecBC;// = vecC - vecB
		CVector3f vecBC;
		//::VectorSubtract( vectorC.x, vectorC.y, vectorC.z,
		//	              vectorB.x, vectorB.y, vectorB.z,
		//				  vecBC.x, vecBC.y, vecBC.z );
		vecBC = vectorC - vectorB;

		//SexyVector normal;
		CVector3f normal;
		//::CrossProduct( vecAB.x, vecAB.y, vecAB.z, 
		//	            vecBC.x, vecBC.y, vecBC.z, 
		//				normal.x, normal.y, normal.z );
		normal.CrossProduct( vecAB, vecBC, normal );

		//::NormalizeVector( normal.x, normal.y, normal.z );
		normal.Normalize();

		// Load the normals onto the verticies
		this->m_verticies.at( itVecFace->vertex_index_1 ).nx = normal.x;
		this->m_verticies.at( itVecFace->vertex_index_1 ).ny = normal.y;
		this->m_verticies.at( itVecFace->vertex_index_1 ).nz = normal.z;
		this->m_verticies.at( itVecFace->vertex_index_2 ).nx = normal.x;
		this->m_verticies.at( itVecFace->vertex_index_2 ).ny = normal.y;
		this->m_verticies.at( itVecFace->vertex_index_2 ).nz = normal.z;
		this->m_verticies.at( itVecFace->vertex_index_3 ).nx = normal.x;
		this->m_verticies.at( itVecFace->vertex_index_3 ).ny = normal.y;
		this->m_verticies.at( itVecFace->vertex_index_3 ).nz = normal.z;
	}
} 

// Generates texture coordinates based on a shperical projection from the 
// origin (0,0,0) location of the model. 
// NOTE: Will overwrite any existing texture coordinates 
// Inspired by: http://www.mvps.org/directx/articles/spheremap.htm
// 
//	basedOnNormals = true --> uses normals for calculation, so good for reflections
//	basedOnNormals = false --> uses model coordinates, so more 'shrink wrapped'
//  fast -> approximate or not (not uses sin() )
//  scale -> normally 1.0, but can be the limits of the texture coord
//
// The 'bias' is what coordinates are used for the generation. 
// For example, if POSITIVE_X is for u and POSITIVE_Y is for v, then:
//	u coords: based on -x to +x
//  v coords: based on -y to +y
//
void CPlyFile5nt::GenTextureCoordsSpherical( enumTEXCOORDBIAS uBias, enumTEXCOORDBIAS vBias, bool basedOnNormals, float scale, bool fast )
{
	// determine the 'remaining' coordinate...
	bool xUsed = false;
	bool yUsed = false;
	bool zUsed = false;
	if ( uBias == POSITIVE_X || vBias == POSITIVE_X )	xUsed = true;
	if ( uBias == POSITIVE_Y || vBias == POSITIVE_Y )	yUsed = true;
	if ( uBias == POSITIVE_Z || vBias == POSITIVE_Z )	yUsed = true;
	
	std::vector<PlyVertex>::iterator itVert;
	for ( itVert = this->m_verticies.begin(); itVert != this->m_verticies.end(); itVert++ )
	{
		CVector3f xyz;
		if ( basedOnNormals )
		{
			if ( uBias == POSITIVE_X )		xyz.x = itVert->nx;
			else if ( uBias == POSITIVE_Y )	xyz.x = itVert->ny;
			else if ( uBias == POSITIVE_Z )	xyz.x = itVert->nz;

			if ( vBias == POSITIVE_X )		xyz.y = itVert->nx;
			else if ( vBias == POSITIVE_Y )	xyz.y = itVert->ny;
			else if ( vBias == POSITIVE_Z )	xyz.y = itVert->nz;

			// Fill in the remaining coordinate...
			if ( !xUsed )	xyz.z = itVert->nx;
			if ( !yUsed )	xyz.z = itVert->ny;
			if ( !zUsed )	xyz.z = itVert->nz;
		}
		else
		{
			if ( uBias == POSITIVE_X )		xyz.x = itVert->xyz.x;
			else if ( uBias == POSITIVE_Y )	xyz.x = itVert->xyz.y;
			else if ( uBias == POSITIVE_Z )	xyz.x = itVert->xyz.z;

			if ( vBias == POSITIVE_X )		xyz.y = itVert->xyz.x;
			else if ( vBias == POSITIVE_Y )	xyz.y = itVert->xyz.y;
			else if ( vBias == POSITIVE_Z )	xyz.y = itVert->xyz.z;

			// Fill in the remaining coordinate...
			if ( !xUsed )	xyz.z = itVert->xyz.x;
			if ( !yUsed )	xyz.z = itVert->xyz.y;
			if ( !zUsed )	xyz.z = itVert->xyz.z;
		}

		xyz.Normalize();

		if ( fast )
		{
			itVert->tex0u = ( ( xyz.x / 2.0f) + 0.5f ) * scale;
			itVert->tex0v = ( ( xyz.y / 2.0f) + 0.5f ) * scale;
		}
		else
		{
			itVert->tex0u = ( ( asin(xyz.x) / PI ) + 0.5f ) * scale;
			itVert->tex0v = ( ( asin(xyz.y) / PI ) + 0.5f ) * scale;
		}
		itVert->tex1u = itVert->tex0u;
		itVert->tex1v = itVert->tex0v;
	}
}


//                         _ _        _____ _        __  __        _     _ ___      _         ___  _            _  __  ____   __            _              ___        _                ___     _ _ 
// _ _  ___ _ _ _ __  __ _| (_)______|_   _| |_  ___|  \/  |___ __| |___| | _ )__ _| |__ _  _|   \(_)_ _ ___ __| |_\ \/ /\ \ / /___ _ _ ___(_)___ _ _     | _ \___ __| |__    _ _     | _ \___| | |
//| ' \/ _ \ '_| '  \/ _` | | |_ / -_) | | | ' \/ -_) |\/| / _ | _` / -_) | _ | _` | '_ \ || | |) | | '_/ -_) _|  _|>  <  \ V // -_) '_(_-<| / _ \ ' \    |   / _ | _| / /   | ' \    |   / _ \ | |
//|_||_\___/_| |_|_|_\__,_|_|_/__\___| |_| |_||_\___|_|  |_\___|__,_\___|_|___|__,_|_.__/\_, |___/|_|_| \___\__|\__/_/\_\  \_/ \___|_| /__/|_\___/_||_|___|_|_\___|__|_\_\___|_||_|___|_|_\___/_|_|
//                                                                                       |__/                                                         |___|              |___|    |___|            
//void CPlyFile5nt::normalizeTheModelBabyDirectXVersion_Rock_n_Roll(void)
//{
	//// Go through all the faces, calculate the normal and 
	////	save (overwrite) the normals at that faces vertices.
	//// DIRECTX Version
	//std::vector<PLYELEMENT>::iterator itVecFace;
	//for ( itVecFace = this->m_elements.begin(); itVecFace != this->m_elements.end(); itVecFace++)
	//{	
	//	// Get the three corners (verticies) of the triangle
	//	PLYVERTEX vertexA = this->m_verticies.at( itVecFace->vertex_index_1 );
	//	PLYVERTEX vertexB = this->m_verticies.at( itVecFace->vertex_index_2 );
	//	PLYVERTEX vertexC = this->m_verticies.at( itVecFace->vertex_index_3 );
	//	// Now calculate the vectors...
	//	D3DXVECTOR3 vectorA( vertexA.xyz.x, vertexA.xyz.y, vertexA.xyz.z );
	//	D3DXVECTOR3 vectorB( vertexB.xyz.x, vertexB.xyz.y, vertexB.xyz.z );
	//	D3DXVECTOR3 vectorC( vertexC.xyz.x, vertexC.xyz.y, vertexC.xyz.z );
	//	// calculate the vectors for the cross...
	//	D3DXVECTOR3 vecAB = vectorB - vectorA;
	//	D3DXVECTOR3 vecBC = vectorC - vectorB;
	//	D3DXVECTOR3 normal;
	//	D3DXVec3Cross( &normal, &vecAB, &vecBC );
	//	D3DXVec3Normalize( &normal, &normal );
	//	// Load the normals onto the verticies
	//	this->m_verticies.at( itVecFace->vertex_index_1 ).nx = normal.x;
	//	this->m_verticies.at( itVecFace->vertex_index_1 ).ny = normal.y;
	//	this->m_verticies.at( itVecFace->vertex_index_1 ).nz = normal.z;
	//	this->m_verticies.at( itVecFace->vertex_index_2 ).nx = normal.x;
	//	this->m_verticies.at( itVecFace->vertex_index_2 ).ny = normal.y;
	//	this->m_verticies.at( itVecFace->vertex_index_2 ).nz = normal.z;
	//	this->m_verticies.at( itVecFace->vertex_index_3 ).nx = normal.x;
	//	this->m_verticies.at( itVecFace->vertex_index_3 ).ny = normal.y;
	//	this->m_verticies.at( itVecFace->vertex_index_3 ).nz = normal.z;
	//}
//}
// ******************************************************************************************

PlyVertex CPlyFile5nt::getVertex_at(std::vector<PlyVertex>::size_type index)
{
	PlyVertex x = this->m_verticies.at( index );
	return x;
}

PlyElement CPlyFile5nt::getElement_at(std::vector<PlyElement>::size_type index)
{
	PlyElement x = this->m_elements.at( index );
	return x;
}


void CPlyFile5nt::m_setIndexBasedOnPropertyName(int curIndex, std::wstring propName)
{
	if ( propName == L"x")  { this->m_PlyHeaderInfo.x_propertyIndex = curIndex; return; }
	if ( propName == L"y")	{ this->m_PlyHeaderInfo.y_propertyIndex = curIndex; return; }
	if ( propName == L"z")	{ this->m_PlyHeaderInfo.z_propertyIndex = curIndex; return; }

	// Added: March 14, 2012
	if ( propName == L"nx" ) { this->m_PlyHeaderInfo.normx_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasNormalsInFile = true; return; }
	if ( propName == L"ny" ) { this->m_PlyHeaderInfo.normy_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasNormalsInFile = true; return; }
	if ( propName == L"nz" ) { this->m_PlyHeaderInfo.normz_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasNormalsInFile = true; return; }
	// End of Added

	if ( propName == L"r")	{ this->m_PlyHeaderInfo.red_propertyIndex = curIndex; return; }
	if ( propName == L"red")	{ this->m_PlyHeaderInfo.red_propertyIndex = curIndex; return; }
	if ( propName == L"g")	{ this->m_PlyHeaderInfo.green_propertyIndex = curIndex; return; }
	if ( propName == L"green")	{ this->m_PlyHeaderInfo.green_propertyIndex = curIndex; return; }
	if ( propName == L"b")	{ this->m_PlyHeaderInfo.blue_propertyIndex = curIndex; return; }
	if ( propName == L"blue")	{ this->m_PlyHeaderInfo.blue_propertyIndex = curIndex; return; }
	if ( propName == L"a" ) { this->m_PlyHeaderInfo.alpha_propertyIndex = curIndex; return; }
	if ( propName == L"alpha")	{ this->m_PlyHeaderInfo.alpha_propertyIndex = curIndex; return; }
	
	// Added: March 14, 2012
	// Textures... note: ply only defines one texture coord...
	if ( propName == L"u" )	{ this->m_PlyHeaderInfo.tex0u_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	if ( propName == L"v" ) { this->m_PlyHeaderInfo.tex0v_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	if ( propName == L"s" )	{ this->m_PlyHeaderInfo.tex0u_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	if ( propName == L"t" ) { this->m_PlyHeaderInfo.tex0v_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	// Added: November 1, 2014
	if ( propName == L"texture_u" )	{ this->m_PlyHeaderInfo.tex0u_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	if ( propName == L"texture_v" ) { this->m_PlyHeaderInfo.tex0v_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	// ...so these ones aren't in any 'normal' ply file
	if ( propName == L"tex0u" ) { this->m_PlyHeaderInfo.tex0u_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	if ( propName == L"tex0v" ) { this->m_PlyHeaderInfo.tex0v_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	if ( propName == L"tex1u" ) { this->m_PlyHeaderInfo.tex1u_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	if ( propName == L"tex1v" ) { this->m_PlyHeaderInfo.tex1v_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }

	if ( propName == L"tx" ) { this->m_PlyHeaderInfo.tangentX_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTangentsInFile = true; return; }
	if ( propName == L"ty" ) { this->m_PlyHeaderInfo.tangentY_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTangentsInFile = true; return; }
	if ( propName == L"tz" ) { this->m_PlyHeaderInfo.tangentZ_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTangentsInFile = true; return; }
	if ( propName == L"bx" ) { this->m_PlyHeaderInfo.binormalX_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasBiNormalsInFile = true; return; }
	if ( propName == L"by" ) { this->m_PlyHeaderInfo.binormalY_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasBiNormalsInFile = true; return; }
	if ( propName == L"bz" ) { this->m_PlyHeaderInfo.binormalZ_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasBiNormalsInFile = true; return; }

	// End of Added

	return;
}

void CPlyFile5nt::m_setIndexBasedOnPropertyNameASCII(int curIndex, std::string propName)
{
	if ( propName == "x")  { this->m_PlyHeaderInfo.x_propertyIndex = curIndex; return; }
	if ( propName == "y")	{ this->m_PlyHeaderInfo.y_propertyIndex = curIndex; return; }
	if ( propName == "z")	{ this->m_PlyHeaderInfo.z_propertyIndex = curIndex; return; }

	// Added: March 14, 2012
	if ( propName == "nx" ) { this->m_PlyHeaderInfo.normx_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasNormalsInFile = true; return; }
	if ( propName == "ny" ) { this->m_PlyHeaderInfo.normy_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasNormalsInFile = true; return; }
	if ( propName == "nz" ) { this->m_PlyHeaderInfo.normz_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasNormalsInFile = true; return; }
	// End of Added

	if ( propName == "r")	{ this->m_PlyHeaderInfo.red_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasColourRGBAInFile = true; return; }
	if ( propName == "red")	{ this->m_PlyHeaderInfo.red_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasColourRGBAInFile = true; return; }
	if ( propName == "g")	{ this->m_PlyHeaderInfo.green_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasColourRGBAInFile = true; return; }
	if ( propName == "green")	{ this->m_PlyHeaderInfo.green_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasColourRGBAInFile = true; return; }
	if ( propName == "b")	{ this->m_PlyHeaderInfo.blue_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasColourRGBAInFile = true; return; }
	if ( propName == "blue")	{ this->m_PlyHeaderInfo.blue_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasColourRGBAInFile = true; return; }
	if ( propName == "a")	{ this->m_PlyHeaderInfo.alpha_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasColourRGBAInFile = true; return; }
	if ( propName == "alpha")	{ this->m_PlyHeaderInfo.alpha_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasColourRGBAInFile = true; return; }
	
	// Added: March 14, 2012
	// Textures... note: ply only defines one texture coord...
	if ( propName == "u" )	{ this->m_PlyHeaderInfo.tex0u_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	if ( propName == "v" ) { this->m_PlyHeaderInfo.tex0v_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	if ( propName == "s" )	{ this->m_PlyHeaderInfo.tex0u_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	if ( propName == "t" ) { this->m_PlyHeaderInfo.tex0v_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	// Added: November 1, 2014
	if ( propName == "texture_u" )	{ this->m_PlyHeaderInfo.tex0u_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	if ( propName == "texture_v" ) { this->m_PlyHeaderInfo.tex0v_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	// ...so these ones aren't in any 'normal' ply file
	if ( propName == "tex0u" ) { this->m_PlyHeaderInfo.tex0u_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	if ( propName == "tex0v" ) { this->m_PlyHeaderInfo.tex0v_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	if ( propName == "tex1u" ) { this->m_PlyHeaderInfo.tex1u_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }
	if ( propName == "tex1v" ) { this->m_PlyHeaderInfo.tex1v_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = true; return; }

	if ( propName == "tx" ) { this->m_PlyHeaderInfo.tangentX_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTangentsInFile = true; return; }
	if ( propName == "ty" ) { this->m_PlyHeaderInfo.tangentY_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTangentsInFile = true; return; }
	if ( propName == "tz" ) { this->m_PlyHeaderInfo.tangentZ_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasTangentsInFile = true; return; }
	if ( propName == "bx" ) { this->m_PlyHeaderInfo.binormalX_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasBiNormalsInFile = true; return; }
	if ( propName == "by" ) { this->m_PlyHeaderInfo.binormalY_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasBiNormalsInFile = true; return; }
	if ( propName == "bz" ) { this->m_PlyHeaderInfo.binormalZ_propertyIndex = curIndex; this->m_PlyHeaderInfo.bHasBiNormalsInFile = true; return; }

	// End of Added

	return;
}

bool CPlyFile5nt::m_placeItemInAppropriateVertexLocation( std::wstring tempString, int propertyIndex, PlyVertex &tempVertex )
{
	// Convert val to float
	std::wstringstream ss;	
	ss << tempString;
	float valAsFloat = 0.0f;
	ss >> valAsFloat;
	if ( propertyIndex == this->m_PlyHeaderInfo.x_propertyIndex) { tempVertex.xyz.x = valAsFloat; return true;}
	if ( propertyIndex == this->m_PlyHeaderInfo.y_propertyIndex) { tempVertex.xyz.y = valAsFloat; return true; }
	if ( propertyIndex == this->m_PlyHeaderInfo.z_propertyIndex) { tempVertex.xyz.z = valAsFloat; return true; }
	if ( propertyIndex == this->m_PlyHeaderInfo.red_propertyIndex) { tempVertex.red = valAsFloat; return true; }
	if ( propertyIndex == this->m_PlyHeaderInfo.green_propertyIndex) { tempVertex.green = valAsFloat; return true; }
	if ( propertyIndex == this->m_PlyHeaderInfo.blue_propertyIndex) { tempVertex.blue = valAsFloat; return true; }
	
	// Added March 14, 2012
	if ( propertyIndex == this->m_PlyHeaderInfo.normx_propertyIndex ) { tempVertex.nx = valAsFloat; return true; }
	if ( propertyIndex == this->m_PlyHeaderInfo.normy_propertyIndex ) { tempVertex.ny = valAsFloat; return true; }
	if ( propertyIndex == this->m_PlyHeaderInfo.normz_propertyIndex ) { tempVertex.nz = valAsFloat; return true; }
	if ( propertyIndex == this->m_PlyHeaderInfo.tex0u_propertyIndex ) { tempVertex.tex0u = valAsFloat; return true; }
	if ( propertyIndex == this->m_PlyHeaderInfo.tex0v_propertyIndex ) { tempVertex.tex0v = valAsFloat; return true; }
	if ( propertyIndex == this->m_PlyHeaderInfo.tex1u_propertyIndex ) { tempVertex.tex1u = valAsFloat; return true; }
	if ( propertyIndex == this->m_PlyHeaderInfo.tex1v_propertyIndex ) { tempVertex.tex1v = valAsFloat; return true; }
	
	if ( propertyIndex == this->m_PlyHeaderInfo.tangentX_propertyIndex ) 
	{ 
		tempVertex.tangent.x = valAsFloat; return true; 
	}
	if ( propertyIndex == this->m_PlyHeaderInfo.tangentY_propertyIndex ) 
	{ 
		tempVertex.tangent.y = valAsFloat; return true; 
	}
	if ( propertyIndex == this->m_PlyHeaderInfo.tangentZ_propertyIndex ) 
	{ 
		tempVertex.tangent.z = valAsFloat; return true; 
	}
	if ( propertyIndex == this->m_PlyHeaderInfo.binormalX_propertyIndex ) 
	{ 
		tempVertex.binormal.x = valAsFloat; return true; 
	}
	if ( propertyIndex == this->m_PlyHeaderInfo.binormalY_propertyIndex ) 
	{ 
		tempVertex.binormal.y = valAsFloat; return true; 
	}
	if ( propertyIndex == this->m_PlyHeaderInfo.binormalZ_propertyIndex ) 
	{ 
		tempVertex.binormal.z = valAsFloat; return true; 
	}

	// End of Added

	// Didn't find a match
	return false;
}

// Added March 14, 2012
bool CPlyFile5nt::bHasTextureCoordinatesInFile(void)
{
	return this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile;
}

bool CPlyFile5nt::bHasNormalsInFile(void)
{
	return this->m_PlyHeaderInfo.bHasNormalsInFile;
}

//// Added June 8, 2013
//void CPlyFile5nt_DX11::normlizeExistingNomrals(void)
//{
//	// Now go through all the vertices and normalize (average) them...
//	std::vector<PLYVERTEX>::iterator itVert;
//	for ( itVert = this->m_verticies.begin(); itVert != this->m_verticies.end(); itVert++)
//	{	
//		XMVECTOR normal = { itVert->nx, itVert->ny, itVert->nz };	// D3DXVECTOR3 normal( itVert->nx, itVert->ny, itVert->nz );
//		normal = XMVector3Normalize( normal );		// D3DXVec3Normalize( &normal, &normal );
//		XMFLOAT3 normalXMFLOAT3;
//		XMStoreFloat3( &normalXMFLOAT3, normal );
//		itVert->nx = normalXMFLOAT3.x;
//		itVert->ny = normalXMFLOAT3.y;
//		itVert->nz = normalXMFLOAT3.z;
//	}
//}
//// End of Added

// Added June 8, 2013
void CPlyFile5nt::normlizeExistingNomrals(void)
{
	// Now go through all the vertices and normalize (average) them...
	std::vector<PlyVertex>::iterator itVert;
	for ( itVert = this->m_verticies.begin(); itVert != this->m_verticies.end(); itVert++)
	{	
		CVector3f normal( itVert->nx, itVert->ny, itVert->nz );
		normal.Normalize();
		itVert->nx = normal.x;
		itVert->ny = normal.y;
		itVert->nz = normal.z;
	}
}
// End of Added
int CPlyFile5nt::GetNumberOfVerticies(void)
{
	//int x = static_cast<int>( this->m_verticies.size() );
	return this->m_PlyHeaderInfo.numberOfVertices;
}

int CPlyFile5nt::GetNumberOfElements(void)
{
	//int x = static_cast<int>( this->m_elements.size() );
	return this->m_PlyHeaderInfo.numberOfElements;
}

float CPlyFile5nt::getMaxX(void)
{
	return this->m_maxX;
}

float CPlyFile5nt::getMinX(void)
{
	return this->m_minX;
}

float CPlyFile5nt::getDeltaX(void)
{
	return this->m_deltaX;
}

float CPlyFile5nt::getMaxY(void)
{
	return this->m_maxY;
}

float CPlyFile5nt::getMinY(void)
{
	return this->m_minY;
}

float CPlyFile5nt::getDeltaY(void)
{
	return this->m_deltaY;
}

float CPlyFile5nt::getMaxZ(void)
{
	return this->m_maxZ;
}

float CPlyFile5nt::getMinZ(void)
{
	return this->m_minZ;
}

float CPlyFile5nt::getDeltaZ(void)
{
	return this->m_deltaZ;
}

float CPlyFile5nt::getMaxExtent(void)
{
	return this->m_maxExtent;
}

float CPlyFile5nt::getMaxExtent(bool bRecalculateExtents)
{
	if ( bRecalculateExtents )
	{
		this->calcualteExtents();
	}
	return this->m_maxExtent;
}

// These were added (again, maybe - I thought I already did this) on June 9, 2015
void CPlyFile5nt::Scale( float scale )
{
	for ( std::vector<PlyVertex>::iterator itVert = this->m_verticies.begin(); itVert != this->m_verticies.end(); itVert++ )
	{
		itVert->xyz.x *= scale;
		itVert->xyz.y *= scale;
		itVert->xyz.z *= scale;
	}
	return;
}

void CPlyFile5nt::ScaleToFit( float boundingBoxSize )
{
	// Get the extents
	this->calcualteExtents();
	// What's the best scale for this?
	float neededScale = boundingBoxSize / this->getMaxExtent();
	// Scale it, baby
	this->Scale( neededScale );
	// Recalculate the extents as they've likely changed
	this->calcualteExtents();

	return;
}

bool CPlyFile5nt::bHasRGBAColoursInFile(void)
{
	return this->m_PlyHeaderInfo.bHasColourRGBAInFile;
}

bool CPlyFile5nt::bHasTagentsInFile(void)
{
	return this->m_PlyHeaderInfo.bHasTangentsInFile;
}

bool CPlyFile5nt::bHasBinormalsInFile(void)
{
	return this->m_PlyHeaderInfo.bHasBiNormalsInFile;
}


void CPlyFile5nt::Translate( CVector3f trans )
{
	for ( std::vector<PlyVertex>::iterator itVec = this->m_verticies.begin(); itVec != this->m_verticies.end(); itVec++ )
	{
		itVec->xyz += trans;
	}// for ( std::vector<PlyVertex>::iterator itVec ...
	return;
}

void CPlyFile5nt::AlignMinXToPlane( float xMinAxisPlane )
{
	// Shift along x to the minium x
	this->Translate( CVector3f( -this->m_minX, 0.0f, 0.0f ) );
	return;
}

void CPlyFile5nt::AlignMinYToPlane( float yMinAxisPlane )
{
	// Shift along y to the minium y
	this->Translate( CVector3f( 0.0f, -this->m_minY, 0.0f ) );
	return;
}

void CPlyFile5nt::AlignMinZToPlane( float zMinAxisPlane )
{
	// Shift along z to the minium z
	this->Translate( CVector3f( 0.0f, 0.0f, -this->m_minZ ) );
	return;
}

void CPlyFile5nt::AlignMaxXToPlane( float xMaxAxisPlane )
{
	// Shift along x to the maximum x
	this->Translate( CVector3f( -this->m_maxX, 0.0f, 0.0f ) );
	return;

}

void CPlyFile5nt::AlignMaxYToPlane( float yMaxAxisPlane )
{
	// Shift along y to the maximum y
	this->Translate( CVector3f( 0.0f, -this->m_maxY, 0.0f ) );
	return;
}

void CPlyFile5nt::AlignMaxZToPlane( float zMaxAxisPlane )
{
	// Shift along z to the maximum z
	this->Translate( CVector3f( 0.0f, 0.0f, -this->m_maxZ ) );
	return;
}


void CPlyFile5nt::ShiftToCentreOfVertices(void)
{
	this->Translate( CVector3f( -this->m_centreX, -this->m_centreY, -this->m_centreZ ) );
	return;
}

