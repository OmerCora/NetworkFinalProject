#include "CPlyFile5nt.h"

#include <cstdlib>	// for atof, atoi, atol, e
#include <cctype>	// for isspace()
// For the "C" locale, white-space characters are any of:
//	' '	(0x20)	space (SPC)
//	'\t'	(0x09)	horizontal tab (TAB)
//	'\n'	(0x0a)	newline (LF)
//	'\v'	(0x0b)	vertical tab (VT)
//	'\f'	(0x0c)	feed (FF)
//	'\r'	(0x0d)	carriage return (CR)tc.

#include <fstream>
#include <math.h>	// for fabs()

#include <sstream>
#include <algorithm>
#include <cctype>

#include "../CHRTimer.h"

//static 
const float CPlyFile5nt::CDataReader::DEFAULTROUNDSMALLFLOATTOZEROVALUE = FLT_MIN;

CPlyFile5nt::CDataReader::CDataReader()
{
	this->m_bRoundSmallFloatToZeroFlag = true;
	this->m_roundToZeroValue = CPlyFile5nt::CDataReader::DEFAULTROUNDSMALLFLOATTOZEROVALUE;
	return;
}

// Added to deal with REALLY small float values screwing up rendering
void CPlyFile5nt::CDataReader::SetMinFloatRoundToZero(float minRoundToZero )
{
	this->m_roundToZeroValue = minRoundToZero;
	return;
}

float CPlyFile5nt::CDataReader::GetMinFloatRoundToZeroValue(void)
{
	return this->m_roundToZeroValue;
}

void CPlyFile5nt::CDataReader::SetRoundTinyFloatsToZeroOnLoadFlag(bool bNewRoundToZeroFlag )
{
	this->m_bRoundSmallFloatToZeroFlag = bNewRoundToZeroFlag;
	return;
}

bool CPlyFile5nt::CDataReader::GetRoundTinyFloatsToZeroOnLoadFlag(void)
{
	return this->m_bRoundSmallFloatToZeroFlag;
}


// Reads utill the next 
std::string CPlyFile5nt::CDataReader::ASCIIReadNextString( char* pData, unsigned int &curIndex, const unsigned int &arraySize )
{
	std::string returnString; char curChar = 0;
	// Gone to far in main array?
	while ( curIndex < arraySize )
	{
		curChar = static_cast<char>( pData[curIndex] );
		if ( isspace(curChar) )
		{
			curIndex++;			
			// There could be additional whitespace (like CR + LF or something)
			// Keep reading white space until we come to a non-whitespace character
			while ( (curIndex < arraySize) && isspace(pData[curIndex]) )
			{
				// "consume" this whitespace character
				curIndex++;
			}
			// Either we're off the end of the array (perhaps that's bad)...
			// ...or we've hit the first non-white space
			return returnString;
		}
		returnString += curChar;
		curIndex++;
	};
	return returnString;
}

int CPlyFile5nt::CDataReader::ASCIIReadNextInt( char* pData, unsigned int &curIndex, const unsigned int &arraySize )
{
	std::string tempString = this->ASCIIReadNextString( pData, curIndex, arraySize );
	int returnInt = atoi( tempString.c_str() );
	return returnInt;
}

float CPlyFile5nt::CDataReader::ASCIIReadNextFloat( char* pData, unsigned int &curIndex, const unsigned int &arraySize )
{
	std::string tempString = this->ASCIIReadNextString( pData, curIndex, arraySize );
	// Has an "e" in there?
	//bool bSexyBreakpoint = false;
	//if ( tempString.find('e') != std::string::npos )
	//{
	//	bSexyBreakpoint = true;
	//}
	float returnFloat = static_cast<float>( atof( tempString.c_str() ) );
	if ( this->m_bRoundSmallFloatToZeroFlag )
	{
		if ( fabs(returnFloat) < this->m_roundToZeroValue )
		{
			returnFloat = 0.0f;
		}
	}
	return returnFloat;
}




bool CPlyFile5nt::OpenPLYFile2(std::wstring fileName, std::wstring &error)
{
	std::ifstream thePlyFile(fileName.c_str(), std::ios::binary );
	if (!thePlyFile.is_open())
	{
		error = L"Can't open the file. Sorry it didn't work out.";
		return false;
	}

	thePlyFile.seekg(0, std::ios::end);
	// reutrns "std::ios::pos_type" type (which seems to be int in VC++)
	unsigned int fileSize = static_cast<unsigned int>( thePlyFile.tellg() );	
	// Return to start
	thePlyFile.seekg(0, std::ios::beg);
		
	//std::vector<char> data( static_cast<unsigned int>(fileSize), 0);
	//this->m_vecRawData.resize( static_cast<unsigned int>(fileSize), 0);

	char* pRawData = new char[ fileSize ];
	unsigned int curIndex = 0;				// Location in the array
	
	// read the file
	//std::vector<char> data;
	//thePlyFile.read( &data[0], fileSize);
	//thePlyFile.read( &(this->m_vecRawData[0]), fileSize);
	thePlyFile.read( pRawData, fileSize );
	thePlyFile.close();

	// *****************************************************************************
	// Process the header information
	CPlyFile5nt::CDataReader reader;

	std::string tempString;
	this->m_PlyHeaderInfo.numberOfVertices = 0;
	this->m_PlyHeaderInfo.numberOfElements = 0;


	try
	{
		// ply
		tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );	
		if ( tempString != "ply")		{ error = L"No a ply file."; throw; }
		
		// format ascii 1.0
		tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );	if ( tempString != "format")	{ throw; }
		
		tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );

		// Added: November 2, 2014
		if ( tempString == "ascii" )
		{
			this->m_PlyHeaderInfo.plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_IS_ASCII;
		}
		else if ( tempString == "binary_little_endian" )
		{
			this->m_PlyHeaderInfo.plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_IS_BINARY_LITTLE_ENDIAN;
		}
		else if ( tempString == "binary_big_endian" )
		{
			this->m_PlyHeaderInfo.plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_IS_BINARY_BIG_ENDIAN;
		}
		else 
		{
			error = L"Format must be ascii, binary_little_endian, or binary_big_endian."; 
			throw; 
		}
		// End of: Added: November 2, 2014		
		
		tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );	if ( tempString != "1.0") { throw; }

		// Find the number of verticies...
		// Item after "vertex" is number of verticies
		do
		{
			tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );
		} while ( tempString != "vertex" );


		this->m_PlyHeaderInfo.numberOfVertices = reader.ASCIIReadNextInt( pRawData, curIndex, fileSize );

		// Read the properties and note the index locations of them...
		int currentIndex = 0;
		while ( true )
		{
			tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );
			if ( tempString == "element" ) { break; /* on to next part of file */ }
			// property float x
			if ( tempString == "property") 
			{
				tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );
				if ( ( tempString == "float") | ( tempString == "uchar") )	
				{	// Figure out which index to set
					tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );
					this->m_setIndexBasedOnPropertyNameASCII( currentIndex, tempString );
				}
				else 
				{ 
					error = L"expected property type missing"; throw; 
				} // if ( tempString == L"float")	
			}//if ( tempString == "property") 
			else
			{ 
				error = L"expected property"; throw; 
			} // if ( tempString == L"property") 
			currentIndex++;
		}

		this->m_PlyHeaderInfo.totalProperties = currentIndex;
		 
		tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );
		if ( tempString != "face")	{ error = L"expected element face."; throw; }

		this->m_PlyHeaderInfo.numberOfElements = reader.ASCIIReadNextInt( pRawData, curIndex, fileSize );

		do
		{
			tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );
		} while ( tempString != "end_header" );




		// The header is read, so determine what "type" of header it is. 
		// Note: this isn't really used for this method (but IS used for OpenPLYFile2), but hey, 
		//	why not live a little, right? 
		this->m_PlyHeaderInfo.DeterminePlyFileType();
	}
	catch (...)
	{
		// Something is wrong
		delete [] pRawData;
		return false;
	}

	// *****************************************************************************

	// Determine the type of file... so we can find the right Vertex adapter
	this->m_PlyHeaderInfo.DeterminePlyFileType();

	// Find an appropriate vertex reader
	IVertexReader* pVertReader;
	// 
	switch ( this->m_PlyHeaderInfo.plyHeaderLayout )
	{
	case CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ:
		pVertReader = new CVertexReader_ASCII_fXYZ();
		break;
	case CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ:
		pVertReader = new CVertexReader_ASCII_fXYZ_nXYZ();
		break;
	case CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_vertUV:
		pVertReader = new CVertexReader_ASCII_fXYZ_nXYZ_vertUV();
		break;
	case CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_vertUV:
		pVertReader = new CVertexReader_ASCII_fXYZ_vertUV();
		break;
	case CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_ucharRGBA_vertUV:
		pVertReader = new CVertexReader_ASCII_XYZ_nXYZ_ucharRGBA_vertUV();
		break;
	case CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_ucharRGB:
		pVertReader = new CVertexReader_ASCII_XYZ_nXYZ_ucharRGB();
		break;
	case CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_ucharRGBA:
		pVertReader = new CVertexReader_ASCII_XYZ_nXYZ_ucharRGBA();
		break;
	default:
		// There is no reader that can handle this format
		pVertReader = 0;
		error = L"Error: No vertex reader to handle this format";
		return false;
	};

	// Double-check if we got a valid reader
	if ( ( pVertReader == 0 ) && !pVertReader->IsReaderValid( this->m_PlyHeaderInfo.plyHeaderLayout ) )
	{
		// No good
		delete [] pRawData;
		return false;
	}

	pVertReader->SetScaleRGBA_OneByteValuesToFloatZeroToOne( this->m_b_ScaleRGBA_OneByteValuesToFloatZeroToOne );

	// We have a valid vertex reader, so read the vertices
	this->m_verticies.reserve( this->m_PlyHeaderInfo.numberOfVertices );
	for ( unsigned int vertCount = 0; vertCount != this->m_PlyHeaderInfo.numberOfVertices; vertCount++ )
	{
		PlyVertex tempVertex;
		pVertReader->ProcessNextVertex( tempVertex, pRawData, curIndex, fileSize );
		this->m_verticies.push_back( tempVertex );
	}

	this->calcualteExtents();

	// Now read the elements...
	IElementReader* pElementReader = new CPlyFile5nt::CElementReader_3intVert();

	this->m_elements.reserve( this->m_PlyHeaderInfo.numberOfElements );
	for ( unsigned int elementCount = 0; elementCount != this->m_PlyHeaderInfo.numberOfElements; elementCount++ )
	{	
		PlyElement tempElement;
		pElementReader->ProcessNextElement( tempElement, pRawData, curIndex, fileSize );
		this->m_elements.push_back( tempElement );
	}
		
	this->m_fileInformation.fileName = fileName;
	this->m_fileInformation.fileType = CFileInfo::MODEL_FILE_TYPE_PLY;

	delete [] pRawData;

	return true;
}

CPlyFile5nt::CPlyHeaderDescription::CPlyHeaderDescription()
{
	this->bHasNormalsInFile = false;
	this->bHasTextureCoordinatesInFile = false;
	this->totalProperties = INT_MAX; 	
	this->x_propertyIndex = INT_MAX; 	this->y_propertyIndex = INT_MAX; 	this->z_propertyIndex = INT_MAX;	
	this->normx_propertyIndex = INT_MAX; this->normy_propertyIndex = INT_MAX; this->normz_propertyIndex = INT_MAX; 
	this->red_propertyIndex = INT_MAX; this->green_propertyIndex = INT_MAX; this->blue_propertyIndex = INT_MAX;	this->alpha_propertyIndex = INT_MAX;
	this->tex0u_propertyIndex = INT_MAX; this->tex0v_propertyIndex = INT_MAX; 
	this->tex1u_propertyIndex = INT_MAX; this->tex1v_propertyIndex = INT_MAX; 
	this->tangentX_propertyIndex = INT_MAX;	this->tangentY_propertyIndex = INT_MAX; this->tangentZ_propertyIndex = INT_MAX;
	this->binormalX_propertyIndex = INT_MAX; this->binormalY_propertyIndex = INT_MAX; this->binormalZ_propertyIndex = INT_MAX;

	this->plyHeaderLayout = CPlyHeaderDescription::HEADER_LAYOUT_UNKNOWN;
	this->numberOfElements = 0;
	this->numberOfVertices = 0;
	this->plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_UNKNOWN;

	// Mainly used for the GDP file format
	this->bHasTangentsInFile = false;
	this->bHasBiNormalsInFile = false;
	this->bHasColourRGBAInFile = false;

	return;
}

bool CPlyFile5nt::CPlyHeaderDescription::bIsThisMachineIsBigEndian(void)
{
	// from: http://www.ibm.com/developerworks/aix/library/au-endianc/
	// [Code] tests the first byte of the multi-byte integer i to determine if it is 0 or 1. 
	// If it is 1, the running platform is assumed to be little-endian. 
	// If it is 0, it is assumed to be big-endian.
	const int i = 1;
	//#define is_bigendian() ( (*(char*)&i) == 0 )
	return ( (*(char*)&i) == 0 );
}

bool CPlyFile5nt::bIsThisMachineIsBigEndian(void)
{
	return this->m_PlyHeaderInfo.bIsThisMachineIsBigEndian();
}

bool CPlyFile5nt::CPlyHeaderDescription::bIsHeader_XYZ(void)
{
	// ASCII_XYZ: 
	// property float x		<-- at location 0
	// property float y		<-- at location 1
	// property float z		<-- at location 2
	// All other properties aren't defined (INT_MAX)
	if ( this->x_propertyIndex != 0 ) { return false; }
	if ( this->y_propertyIndex != 1 ) { return false; }
	if ( this->z_propertyIndex != 2 ) { return false; }
	// 
	// All the rest shouldn't be defined
	if ( this->normx_propertyIndex != INT_MAX ) { return false; }
	if ( this->normy_propertyIndex != INT_MAX ) { return false; }
	if ( this->normz_propertyIndex != INT_MAX ) { return false; }
	if ( this->red_propertyIndex != INT_MAX ) { return false; }
	if ( this->green_propertyIndex != INT_MAX ) { return false; }
	if ( this->blue_propertyIndex != INT_MAX ) { return false; }
	if ( this->alpha_propertyIndex != INT_MAX ) { return false; } 
	if ( this->tex0u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex0v_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1v_propertyIndex != INT_MAX ) { return false; }

	// So we're good to go; this has to be the interface
	return true;
}

bool CPlyFile5nt::CPlyHeaderDescription::bIsHeader_XYZ_nXYZ(void)
{
	// ASCII_XYZ_nXYZ: 
	// property float x		<-- at location 0
	// property float y		<-- at location 1
	// property float z		<-- at location 2
	// property float nx	<-- at location 3
	// property float ny	<-- at location 4
	// property float nz	<-- at location 5
	// All other properties aren't defined (INT_MAX)
	if ( this->x_propertyIndex != 0 ) { return false; }
	if ( this->y_propertyIndex != 1 ) { return false; }
	if ( this->z_propertyIndex != 2 ) { return false; }
	if ( this->normx_propertyIndex != 3 ) { return false; }
	if ( this->normy_propertyIndex != 4 ) { return false; }
	if ( this->normz_propertyIndex != 5 ) { return false; }
	// 
	// All the rest shouldn't be defined
	if ( this->red_propertyIndex != INT_MAX ) { return false; }
	if ( this->green_propertyIndex != INT_MAX ) { return false; }
	if ( this->blue_propertyIndex != INT_MAX ) { return false; }
	if ( this->alpha_propertyIndex != INT_MAX ) { return false; } 
	if ( this->tex0u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex0v_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1v_propertyIndex != INT_MAX ) { return false; }

	// So we're good to go; this has to be the interface
	return true;
}

bool CPlyFile5nt::CPlyHeaderDescription::bIsHeader_XYZ_nXYZ_vertUV(void)
{
	// ASCII_XYZ_nXYZ: 
	// property float x		<-- at location 0
	// property float y		<-- at location 1
	// property float z		<-- at location 2
	// property float nx	<-- at location 3
	// property float ny	<-- at location 4
	// property float nz	<-- at location 5
	// property float texture_u		<-- at location 6
	// property float texture_v		<-- at location 7

	// All other properties aren't defined (INT_MAX)
	if ( this->x_propertyIndex != 0 ) { return false; }
	if ( this->y_propertyIndex != 1 ) { return false; }
	if ( this->z_propertyIndex != 2 ) { return false; }
	if ( this->normx_propertyIndex != 3 ) { return false; }
	if ( this->normy_propertyIndex != 4 ) { return false; }
	if ( this->normz_propertyIndex != 5 ) { return false; }
	if ( this->tex0u_propertyIndex != 6 ) { return false; }
	if ( this->tex0v_propertyIndex != 7 ) { return false; }
	// 
	// All the rest shouldn't be defined
	if ( this->red_propertyIndex != INT_MAX ) { return false; }
	if ( this->green_propertyIndex != INT_MAX ) { return false; }
	if ( this->blue_propertyIndex != INT_MAX ) { return false; }
	if ( this->alpha_propertyIndex != INT_MAX ) { return false; } 
	if ( this->tex1u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1v_propertyIndex != INT_MAX ) { return false; }

	// So we're good to go; this has to be the interface
	return true;
}

bool CPlyFile5nt::CPlyHeaderDescription::bIsHeader_XYZ_vertUV(void)
{
	// ASCII_XYZ_nXYZ: 
	// property float x		<-- at location 0
	// property float y		<-- at location 1
	// property float z		<-- at location 2
	// property float texture_u		<-- at location 3
	// property float texture_v		<-- at location 4
	// All other properties aren't defined (INT_MAX)
	if ( this->x_propertyIndex != 0 ) { return false; }
	if ( this->y_propertyIndex != 1 ) { return false; }
	if ( this->z_propertyIndex != 2 ) { return false; }
	if ( this->tex0u_propertyIndex != 3 ) { return false; }
	if ( this->tex0v_propertyIndex != 4 ) { return false; }
	// 
	// All the rest shouldn't be defined
	if ( this->normx_propertyIndex != INT_MAX ) { return false; }
	if ( this->normy_propertyIndex != INT_MAX ) { return false; }
	if ( this->normz_propertyIndex != INT_MAX ) { return false; }
	if ( this->red_propertyIndex != INT_MAX ) { return false; }
	if ( this->green_propertyIndex != INT_MAX ) { return false; }
	if ( this->blue_propertyIndex != INT_MAX ) { return false; }
	if ( this->alpha_propertyIndex != INT_MAX ) { return false; } 
	if ( this->tex1u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1v_propertyIndex != INT_MAX ) { return false; }

	// So we're good to go; this has to be the interface
	return true;
}

bool CPlyFile5nt::CPlyHeaderDescription::bIsHeader_XYZ_nXYZ_ucharRGBA_vertUV(void)
{
	// ASCII_XYZ_nXYZ_ucharRGBA_vertUV: 
	// property float x		<-- at location 0
	// property float y		<-- at location 1
	// property float z		<-- at location 2
	// property float nx	<-- at location 3
	// property float ny	<-- at location 4
	// property float nz	<-- at location 5
	// property uchar red	<-- at location 6
	// property uchar green		<-- at location 7
	// property uchar blue		<-- at location 8
	// property uchar alpha		<-- at location 9
	// property float texture_u		<-- at location 10
	// property float texture_v		<-- at location 11

	// All other properties aren't defined (INT_MAX)
	if ( this->x_propertyIndex != 0 ) { return false; }
	if ( this->y_propertyIndex != 1 ) { return false; }
	if ( this->z_propertyIndex != 2 ) { return false; }
	if ( this->normx_propertyIndex != 3 ) { return false; }
	if ( this->normy_propertyIndex != 4 ) { return false; }
	if ( this->normz_propertyIndex != 5 ) { return false; }
	if ( this->red_propertyIndex != 6 ) { return false; }
	if ( this->green_propertyIndex != 7 ) { return false; }
	if ( this->blue_propertyIndex != 8 ) { return false; }
	if ( this->alpha_propertyIndex != 9 ) { return false; } 
	if ( this->tex0u_propertyIndex != 10 ) { return false; }
	if ( this->tex0v_propertyIndex != 11 ) { return false; }
	// 
	// All the rest shouldn't be defined
	if ( this->tex1u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1v_propertyIndex != INT_MAX ) { return false; }

	// So we're good to go; this has to be the interface
	return true;
}

// Added June 2016
bool CPlyFile5nt::CPlyHeaderDescription::bIsHeader_XYZ_nXYZ_ucharRGB(void)
{
	// ASCII_XYZ_nXYZ_ucharRGB: 
	// property float x		<-- at location 0
	// property float y		<-- at location 1
	// property float z		<-- at location 2
	// property float nx	<-- at location 3
	// property float ny	<-- at location 4
	// property float nz	<-- at location 5
	// property uchar red	<-- at location 6
	// property uchar green		<-- at location 7
	// property uchar blue		<-- at location 8

	// All other properties aren't defined (INT_MAX)
	if ( this->x_propertyIndex != 0 ) { return false; }
	if ( this->y_propertyIndex != 1 ) { return false; }
	if ( this->z_propertyIndex != 2 ) { return false; }
	if ( this->normx_propertyIndex != 3 ) { return false; }
	if ( this->normy_propertyIndex != 4 ) { return false; }
	if ( this->normz_propertyIndex != 5 ) { return false; }
	if ( this->red_propertyIndex != 6 ) { return false; }
	if ( this->green_propertyIndex != 7 ) { return false; }
	if ( this->blue_propertyIndex != 8 ) { return false; }
	if ( this->alpha_propertyIndex != 9 ) { return false; } 
	// 
	// All the rest shouldn't be defined
	if ( this->alpha_propertyIndex != INT_MAX ) { return false; } 
	if ( this->tex0u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex0v_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1v_propertyIndex != INT_MAX ) { return false; }

	// So we're good to go; this has to be the interface
	return true;
}

// Added June 2016
bool CPlyFile5nt::CPlyHeaderDescription::bIsHeader_XYZ_nXYZ_ucharRGBA(void)
{
	// ASCII_XYZ_nXYZ_ucharRGBA: 
	// property float x		<-- at location 0
	// property float y		<-- at location 1
	// property float z		<-- at location 2
	// property float nx	<-- at location 3
	// property float ny	<-- at location 4
	// property float nz	<-- at location 5
	// property uchar red	<-- at location 6
	// property uchar green		<-- at location 7
	// property uchar blue		<-- at location 8
	// property uchar alpha		<-- at location 9

	// All other properties aren't defined (INT_MAX)
	if ( this->x_propertyIndex != 0 ) { return false; }
	if ( this->y_propertyIndex != 1 ) { return false; }
	if ( this->z_propertyIndex != 2 ) { return false; }
	if ( this->normx_propertyIndex != 3 ) { return false; }
	if ( this->normy_propertyIndex != 4 ) { return false; }
	if ( this->normz_propertyIndex != 5 ) { return false; }
	if ( this->red_propertyIndex != 6 ) { return false; }
	if ( this->green_propertyIndex != 7 ) { return false; }
	if ( this->blue_propertyIndex != 8 ) { return false; }
	if ( this->alpha_propertyIndex != 9 ) { return false; } 
	// 
	// All the rest shouldn't be defined
	if ( this->tex0u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex0v_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1v_propertyIndex != INT_MAX ) { return false; }

	// So we're good to go; this has to be the interface
	return true;
}


// Based on the information contained inside, determine the header type
void CPlyFile5nt::CPlyHeaderDescription::DeterminePlyFileType(void)
{
	// Assume it's unknown
	this->plyHeaderLayout = CPlyHeaderDescription::HEADER_LAYOUT_UNKNOWN;

	// ASCII_XYZ: 
	if ( this->bIsHeader_XYZ() && ( this->plyFormatASCIIorBinary == CPlyHeaderDescription::FORMAT_IS_ASCII ) )
	{
		this->plyHeaderLayout = CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ;
	}
	else if ( this->bIsHeader_XYZ_nXYZ() && ( this->plyFormatASCIIorBinary == CPlyHeaderDescription::FORMAT_IS_ASCII ) )
	{
		this->plyHeaderLayout = CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ;
	}
	else if ( this->bIsHeader_XYZ_nXYZ_vertUV() && ( this->plyFormatASCIIorBinary == CPlyHeaderDescription::FORMAT_IS_ASCII ) )
	{
		this->plyHeaderLayout = CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_vertUV;
	}
	else if ( this->bIsHeader_XYZ_vertUV() && ( this->plyFormatASCIIorBinary == CPlyHeaderDescription::FORMAT_IS_ASCII ) )
	{
		this->plyHeaderLayout = CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_vertUV;
	}
	else if ( this->bIsHeader_XYZ_nXYZ_ucharRGBA_vertUV() && ( this->plyFormatASCIIorBinary == CPlyHeaderDescription::FORMAT_IS_ASCII ) )
	{
		this->plyHeaderLayout = CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_ucharRGBA_vertUV;
	}
	else if ( this->bIsHeader_XYZ_nXYZ_ucharRGB() && ( this->plyFormatASCIIorBinary == CPlyHeaderDescription::FORMAT_IS_ASCII ) )
	{
		this->plyHeaderLayout = CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_ucharRGB;
	}
	else if ( this->bIsHeader_XYZ_nXYZ_ucharRGBA() && ( this->plyFormatASCIIorBinary == CPlyHeaderDescription::FORMAT_IS_ASCII ) )
	{
		this->plyHeaderLayout = CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_ucharRGBA;
	}

	// else, we dunno what the heck the layout is (well, we can't process it, anyway)
	
	return;
}
		






 //__      __          _                             _                _                   
 //\ \    / /         | |                  /\       | |              | |                  
 // \ \  / /___  _ __ | |_  ___ __  __    /  \    __| |  __ _  _ __  | |_  ___  _ __  ___ 
 //  \ \/ // _ \| '__|| __|/ _ \\ \/ /   / /\ \  / _` | / _` || '_ \ | __|/ _ \| '__|/ __|
 //   \  /|  __/| |   | |_|  __/ >  <   / ____ \| (_| || (_| || |_) || |_|  __/| |   \__ \
 //    \/  \___||_|    \__|\___|/_/\_\ /_/    \_\\__,_| \__,_|| .__/  \__|\___||_|   |___/
 //                                                           | |                         
 //                                                           |_|                         



 //   _    ___   ___  ___  ___      __ __  ____   __ ____
 //  /_\  / __| / __||_ _||_ _|    / _|\ \/ /\ \ / /|_  /
 // / _ \ \__ \| (__  | |  | |    |  _| >  <  \ V /  / / 
 ///_/ \_\|___/ \___||___||___|___|_|  /_/\_\  |_|  /___|
 //                           |___|                      
bool CPlyFile5nt::CVertexReader_ASCII_fXYZ::IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType )
{
	if ( plyFileType == CPlyFile5nt::CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ )
	{
		return true;
	}	
	return false;
}

bool CPlyFile5nt::CVertexReader_ASCII_fXYZ::ProcessNextVertex( PlyVertex &vertex, char* pData, 
															   unsigned int &curIndex, const unsigned int &arraySize )
{
	//property float x
	//property float y
	//property float z
	vertex.xyz.x = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.y = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.z = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	return true;
}

 //  __ __  ____   __ ____         __  ____   __ ____                  _    _   _ __   __
 // / _|\ \/ /\ \ / /|_  /    _ _  \ \/ /\ \ / /|_  /  __ __ ___  _ _ | |_ | | | |\ \ / /
 //|  _| >  <  \ V /  / /    | ' \  >  <  \ V /  / /   \ V // -_)| '_||  _|| |_| | \ V / 
 //|_|  /_/\_\  |_|  /___|___|_||_|/_/\_\  |_|  /___|___\_/ \___||_|   \__| \___/   \_/  
 //                      |___|                      |___|                                
bool CPlyFile5nt::CVertexReader_ASCII_fXYZ_nXYZ_vertUV::IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType )
{
	if ( plyFileType == CPlyFile5nt::CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_vertUV )
	{
		return true;
	}
	return false;
}

bool CPlyFile5nt::CVertexReader_ASCII_fXYZ_nXYZ_vertUV::ProcessNextVertex( PlyVertex &vertex, char* pData, 
															   unsigned int &curIndex, const unsigned int &arraySize )
{
	//property float x
	//property float y
	//property float z
	//property float nx
	//property float ny
	//property float nz
	//property float texture_u
	//property float texture_v
	vertex.xyz.x = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.y = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.z = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.nx = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.ny = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.nz = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.tex0u = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.tex0v = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	return true;
}

 //  __ __  ____   __ ____                  _    _   _ __   __
 // / _|\ \/ /\ \ / /|_  /  __ __ ___  _ _ | |_ | | | |\ \ / /
 //|  _| >  <  \ V /  / /   \ V // -_)| '_||  _|| |_| | \ V / 
 //|_|  /_/\_\  |_|  /___|___\_/ \___||_|   \__| \___/   \_/  
 //                      |___|                                
bool CPlyFile5nt::CVertexReader_ASCII_fXYZ_vertUV::IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType )
{
	if ( plyFileType == CPlyFile5nt::CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_vertUV )
	{
		return true;
	}
	return false;
}

bool CPlyFile5nt::CVertexReader_ASCII_fXYZ_vertUV::ProcessNextVertex( PlyVertex &vertex, char* pData, 
															   unsigned int &curIndex, const unsigned int &arraySize )
{
	//property float x
	//property float y
	//property float z
	//property float texture_u
	//property float texture_v
	vertex.xyz.x = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.y = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.z = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.tex0u = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.tex0v = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	return true;
}

 //  __ __  ____   __ ____         __  ____   __ ____
 // / _|\ \/ /\ \ / /|_  /    _ _  \ \/ /\ \ / /|_  /
 //|  _| >  <  \ V /  / /    | ' \  >  <  \ V /  / / 
 //|_|  /_/\_\  |_|  /___|___|_||_|/_/\_\  |_|  /___|
 //                      |___|                       
bool CPlyFile5nt::CVertexReader_ASCII_fXYZ_nXYZ::IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType )
{
	if ( plyFileType == CPlyFile5nt::CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ )
	{
		return true;
	}
	return false;
}

bool CPlyFile5nt::CVertexReader_ASCII_fXYZ_nXYZ::ProcessNextVertex( PlyVertex &vertex, char* pData, 
															   unsigned int &curIndex, const unsigned int &arraySize )
{
	//property float x
	//property float y
	//property float z
	//property float nx
	//property float ny
	//property float nz
	vertex.xyz.x = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.y = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.z = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.nx = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.ny = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.nz = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	return true;
}


//    ____  ____   ______      __  ____   ______         _             ___  ___ ___   _                _   _   ___   __
//   / _\ \/ /\ \ / /_  /  _ _ \ \/ /\ \ / /_  / _  _ __| |_  __ _ _ _| _ \/ __| _ ) /_\  __ _____ _ _| |_| | | \ \ / /
//  |  _|>  <  \ V / / /  | ' \ >  <  \ V / / / | || / _| ' \/ _` | '_|   / (_ | _ \/ _ \ \ V / -_) '_|  _| |_| |\ V / 
//  |_| /_/\_\  |_| /___|_|_||_/_/\_\  |_| /___|_\_,_\__|_||_\__,_|_| |_|_\\___|___/_/ \_\_\_/\___|_|  \__|\___/  \_/  
//                     |___|                  |___|                                     |___|                          
bool CPlyFile5nt::CVertexReader_ASCII_XYZ_nXYZ_ucharRGBA_vertUV::IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType )
{
	if ( plyFileType == CPlyFile5nt::CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_ucharRGBA_vertUV )
	{
		return true;
	}
	return false;
}

bool CPlyFile5nt::CVertexReader_ASCII_XYZ_nXYZ_ucharRGBA_vertUV::ProcessNextVertex( PlyVertex &vertex, char* pData, 
															                        unsigned int &curIndex, const unsigned int &arraySize )
{
	//property float x
	//property float y
	//property float z
	//property float nx
	//property float ny
	//property float nz
	//property uchar red
	//property uchar green
	//property uchar blue
	//property uchar alpha
	//property float texture_u
	//property float texture_v
	vertex.xyz.x = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.y = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.z = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.nx = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.ny = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.nz = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.red = static_cast<float>( this->reader.ASCIIReadNextInt( pData, curIndex, arraySize ) );
	vertex.green = static_cast<float>( this->reader.ASCIIReadNextInt( pData, curIndex, arraySize ) );
	vertex.blue = static_cast<float>( this->reader.ASCIIReadNextInt( pData, curIndex, arraySize ) );
	vertex.alpha = static_cast<float>( this->reader.ASCIIReadNextInt( pData, curIndex, arraySize ) );
	if ( this->m_b_ScaleRGBA_OneByteValuesToFloatZeroToOne )
	{
		this->ScaleRGBA_OneByteValuesToZeroToOne( vertex );
	}
	vertex.tex0u = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.tex0v = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	return true;
}


// Added June 2016
//   ____  ____   ______      __  ____   ______         _             ___  ___ ___ 
//  / _\ \/ /\ \ / /_  /  _ _ \ \/ /\ \ / /_  / _  _ __| |_  __ _ _ _| _ \/ __| _ )
// |  _|>  <  \ V / / /  | ' \ >  <  \ V / / / | || / _| ' \/ _` | '_|   / (_ | _ \
// |_| /_/\_\  |_| /___|_|_||_/_/\_\  |_| /___|_\_,_\__|_||_\__,_|_|_|_|_\\___|___/
//                    |___|                  |___|                |___|            
bool CPlyFile5nt::CVertexReader_ASCII_XYZ_nXYZ_ucharRGB::IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType )
{
	if ( plyFileType == CPlyFile5nt::CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_ucharRGB )
	{
		return true;
	}
	return false;
}

bool CPlyFile5nt::CVertexReader_ASCII_XYZ_nXYZ_ucharRGB::ProcessNextVertex( PlyVertex &vertex, char* pData, 
															                        unsigned int &curIndex, const unsigned int &arraySize )
{
	//property float x
	//property float y
	//property float z
	//property float nx
	//property float ny
	//property float nz
	//property uchar red
	//property uchar green
	//property uchar blue
	//property uchar alpha
	vertex.xyz.x = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.y = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.z = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.nx = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.ny = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.nz = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.red = static_cast<float>( this->reader.ASCIIReadNextInt( pData, curIndex, arraySize ) );
	vertex.green = static_cast<float>( this->reader.ASCIIReadNextInt( pData, curIndex, arraySize ) );
	vertex.blue = static_cast<float>( this->reader.ASCIIReadNextInt( pData, curIndex, arraySize ) );
	if ( this->m_b_ScaleRGBA_OneByteValuesToFloatZeroToOne )
	{
		this->ScaleRGBA_OneByteValuesToZeroToOne( vertex );
	}
	return true;
}


//   ____  ____   ______      __  ____   ______         _             ___  ___ ___   _   
//  / _\ \/ /\ \ / /_  /  _ _ \ \/ /\ \ / /_  / _  _ __| |_  __ _ _ _| _ \/ __| _ ) /_\  
// |  _|>  <  \ V / / /  | ' \ >  <  \ V / / / | || / _| ' \/ _` | '_|   / (_ | _ \/ _ \ 
// |_| /_/\_\  |_| /___|_|_||_/_/\_\  |_| /___|_\_,_\__|_||_\__,_|_|_|_|_\\___|___/_/ \_\
//                    |___|                  |___|                |___|                  
bool CPlyFile5nt::CVertexReader_ASCII_XYZ_nXYZ_ucharRGBA::IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType )
{
	if ( plyFileType == CPlyFile5nt::CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_ucharRGBA )
	{
		return true;
	}
	return false;
}

bool CPlyFile5nt::CVertexReader_ASCII_XYZ_nXYZ_ucharRGBA::ProcessNextVertex( PlyVertex &vertex, char* pData, 
															                 unsigned int &curIndex, const unsigned int &arraySize )
{
	//property float x
	//property float y
	//property float z
	//property float nx
	//property float ny
	//property float nz
	//property uchar red
	//property uchar green
	//property uchar blue
	//property uchar alpha
	vertex.xyz.x = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.y = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.z = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.nx = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.ny = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.nz = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.red = static_cast<float>( this->reader.ASCIIReadNextInt( pData, curIndex, arraySize ) );
	vertex.green = static_cast<float>( this->reader.ASCIIReadNextInt( pData, curIndex, arraySize ) );
	vertex.blue = static_cast<float>( this->reader.ASCIIReadNextInt( pData, curIndex, arraySize ) );
	vertex.alpha = static_cast<float>( this->reader.ASCIIReadNextInt( pData, curIndex, arraySize ) );
	if ( this->m_b_ScaleRGBA_OneByteValuesToFloatZeroToOne )
	{
		this->ScaleRGBA_OneByteValuesToZeroToOne( vertex );
	}
	return true;
}



 //  ___  ___  _                        _    ___                _             ____ _       _  __   __          _   
 // / __|| __|| | ___  _ __   ___  _ _ | |_ | _ \ ___  __ _  __| | ___  _ _  |__ /(_) _ _ | |_\ \ / /___  _ _ | |_ 
 //| (__ | _| | |/ -_)| '  \ / -_)| ' \|  _||   // -_)/ _` |/ _` |/ -_)| '_|  |_ \| || ' \|  _|\ V // -_)| '_||  _|
 // \___||___||_|\___||_|_|_|\___||_||_|\__||_|_\\___|\__,_|\__,_|\___||_|___|___/|_||_||_|\__| \_/ \___||_|   \__|
 //                                                                      |___|                                     
bool CPlyFile5nt::CElementReader_3intVert::ProcessNextElement( PlyElement &element, char* pData, 
																 unsigned int &curIndex, const unsigned int &arraySize )
{
	// Discard first value (will be "3" for triangles
	this->reader.ASCIIReadNextInt( pData, curIndex, arraySize );
	element.vertex_index_1 = this->reader.ASCIIReadNextInt( pData, curIndex, arraySize );
	element.vertex_index_2 = this->reader.ASCIIReadNextInt( pData, curIndex, arraySize );
	element.vertex_index_3 = this->reader.ASCIIReadNextInt( pData, curIndex, arraySize );
	return true;
}

void CPlyFile5nt::m_gdp_StoreBoolToCharArray( char* pCharArrLoc, bool bValue )
{
	(*pCharArrLoc) = ( bValue ? CPlyHeaderDescription::GDP_TRUE : CPlyHeaderDescription::GDP_FALSE );
	return;
}

bool CPlyFile5nt::m_gdp_bReadBoolFromCharArray( char* pCharArrLoc )
{
	if ( (*pCharArrLoc) == CPlyHeaderDescription::GDP_FALSE )
	{
		return false;
	}
	return true;
}

void CPlyFile5nt::m_gdp_StoreInt32IntoCharArray( char* pCharArrayLoc, int theInt )
{
	//http://stackoverflow.com/questions/1522994/store-an-int-in-a-char-array
	pCharArrayLoc[0] = theInt & 0xff;			//LSD
	pCharArrayLoc[1] = (theInt>>8) & 0xff;
	pCharArrayLoc[2] = (theInt>>16) & 0xff;
	pCharArrayLoc[3] = (theInt>>24) & 0xff;		//MSD
	return;
}

int  CPlyFile5nt::m_gdp_ReadInt32FromCharArray( char* pCharArrLoc )
{
	int theInt = 0;
	theInt = ((pCharArrLoc[3] & 0xff) <<24) + 
		     ((pCharArrLoc[2] & 0xff) <<16) +
			 ((pCharArrLoc[1] & 0xff) <<8) +
			  (pCharArrLoc[0] & 0xff);

	return theInt;
}

void CPlyFile5nt::m_gdp_StoreFloat32IntoCharArray( char* pCharArrLoc, float fValue )
{
	char* pChar0 = reinterpret_cast<char*>(&fValue);	// Point to start of float in RAM
	pCharArrLoc[0] = *(pChar0);			//& 0xff;			//LSD
	pCharArrLoc[1] = *(pChar0 + 1);		//(fValue>>8) & 0xff;
	pCharArrLoc[2] = *(pChar0 + 2);		//(fValue>>16) & 0xff;
	pCharArrLoc[3] = *(pChar0 + 3);		//(fValue>>24) & 0xff;		//MSD
	return;
}

float CPlyFile5nt::m_gdp_ReadFloat32FromCharArray( char* pCharArrLoc )
{
	float theFloat = 0.0f;
	char* pChar0 = reinterpret_cast<char*>(&theFloat);	// Point to start of float
	pChar0[0] = pCharArrLoc[0];
	pChar0[1] = pCharArrLoc[1];
	pChar0[2] = pCharArrLoc[2];
	pChar0[3] = pCharArrLoc[3];

	return theFloat;
}


std::string CPlyFile5nt::m_gdp_ReadStringFromCharArray( char* pCharArrLoc, int numberOfChars )
{
	std::string theString;
	for ( int index = 0; index != numberOfChars; index++ )
	{
		// Stupid STD string appends with += operator (not + or append(), of course...)
		theString += pCharArrLoc[index];
	}
	return theString;
}

void CPlyFile5nt::m_gdp_StoreStringIntoCharArray( char* pCharArrLoc, std::string theString )
{
	for ( int index = 0; index != theString.size(); index++ )
	{
		char tempChar = static_cast<char>( theString.at(index) );
		pCharArrLoc[index] = static_cast<char>( theString.at(index) );
	}
	return;
}


bool CPlyFile5nt::SaveGDPFile(std::wstring fileName, bool bOverwrite, std::wstring &error)
{
	CHRTimer timer;
	timer.Reset();
	timer.Start();

	std::ofstream thePlyFile(fileName.c_str(), std::ios::binary );
	if (!thePlyFile.is_open())
	{
		error = L"Can't open the file. Sorry it didn't work out.";
		return false;
	}	

	char* gdpHeaderData = new char[GDPHEADERSIZE];
	memset( gdpHeaderData, 0, GDPHEADERSIZE * sizeof(char) );	// Clear to zeros

	// The header contains the following
	// char 0, 1, 2: "gdp"
	// char 3: version - this one loads only version 1
	// char 4: is little endian
	// char 5: has normals, in xyz format (0 or 1)
	// char 6: has UVs
	// char 7: has tangent XYZ
	// char 8: has binormal XYZ
	// char 9: has colour RGBA
	// char 10 to 31: NA
	// char 32 to 37: uint 0: number vertices
	// char 38 to 41: uint 1: number elements (triangles)
	// char 42 to 63: NA

	//gdpHeaderData[0] = 'g';	// char 0, 1, 2: "gdp"
	//gdpHeaderData[1] = 'd';
	//gdpHeaderData[2] = 'p';
	this->m_gdp_StoreStringIntoCharArray( &(gdpHeaderData[0]), "gdp" ); 

	gdpHeaderData[3] = 1;	// char 3: version - this one loads only version 1
	// char 4: is little endian (Do we even need this...?)
	this->m_gdp_StoreBoolToCharArray( &(gdpHeaderData[4]), this->bIsThisMachineIsBigEndian() );	
	// char 5: has normals, in xyz format (0 or 1)
	this->m_gdp_StoreBoolToCharArray( &(gdpHeaderData[5]), this->bHasNormalsInFile() );
	// char 6: has UVs
	this->m_gdp_StoreBoolToCharArray( &(gdpHeaderData[6]), this->bHasTextureCoordinatesInFile() );
	// char 7: has tangent XYZ
	this->m_gdp_StoreBoolToCharArray( &(gdpHeaderData[7]), this->bHasTagentsInFile() );
	// char 8: has binormal XYZ
	this->m_gdp_StoreBoolToCharArray( &(gdpHeaderData[8]), this->m_PlyHeaderInfo.bHasBiNormalsInFile );
	// char 9: has colour RGB
	this->m_gdp_StoreBoolToCharArray( &(gdpHeaderData[9]), this->m_PlyHeaderInfo.bHasColourRGBAInFile );
	
	// char 32 to 37: uint 0: number vertices
	this->m_gdp_StoreInt32IntoCharArray( &(gdpHeaderData[32]), this->GetNumberOfVerticies() );
	// char 38 to 41: uint 1: number elements (triangles)
	this->m_gdp_StoreInt32IntoCharArray( &(gdpHeaderData[38]), this->GetNumberOfElements() );


	// Write out the header
	thePlyFile.write( gdpHeaderData, GDPHEADERSIZE );

	// Write XYZ and ELEMENTs (ALWAYS has these), then 
	// Vertex: nxyz, UVs, colours, tangents, binormals (because they are optional)

	//unsigned int sizeOfVertArray = this->GetNumberOfVerticies() * 3 * sizeof(float);
	//char* tempVertArray = new char(sizeOfVertArray);

	// ALWAYS has XYZ...
	for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
	{
		thePlyFile.write( (const char*) &(this->m_verticies[index].xyz.x), sizeof(float) );
		thePlyFile.write( (const char*) &(this->m_verticies[index].xyz.y), sizeof(float) );
		thePlyFile.write( (const char*) &(this->m_verticies[index].xyz.z), sizeof(float) );
	}
	timer.UpdateLongDuration();

	// ALWAYS has elements
	for ( int index = 0; index != this->GetNumberOfElements(); index++ )
	{
		thePlyFile.write( (const char*) &(this->m_elements[index].vertex_index_1), sizeof(int) );
		thePlyFile.write( (const char*) &(this->m_elements[index].vertex_index_2), sizeof(int) );
		thePlyFile.write( (const char*) &(this->m_elements[index].vertex_index_3), sizeof(int) );
	}
	timer.UpdateLongDuration();

	// *** These are all optional items ***

	// Vertex Normals in the file?
	if ( this->bHasNormalsInFile() )
	{
		for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
		{
			thePlyFile.write( (const char*) &(this->m_verticies[index].nx), sizeof(float) );
			thePlyFile.write( (const char*) &(this->m_verticies[index].ny), sizeof(float) );
			thePlyFile.write( (const char*) &(this->m_verticies[index].nz), sizeof(float) );
		}
	}//if ( this->bHadNormalsInFile() )
	timer.UpdateLongDuration();

	// Vertex UVs in the file?
	if ( this->bHasTextureCoordinatesInFile() )
	{
		for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
		{
			thePlyFile.write( (const char*) &(this->m_verticies[index].tex0u), sizeof(float) );
			thePlyFile.write( (const char*) &(this->m_verticies[index].tex0v), sizeof(float) );
		}
	}//if ( this->bHadTextureCoordinatesInFile() )
	timer.UpdateLongDuration();

	// Vertex Colours in the file?
	if ( this->m_PlyHeaderInfo.bHasColourRGBAInFile )
	{
		for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
		{
			thePlyFile.write( (const char*) &(this->m_verticies[index].red), sizeof(float) );
			thePlyFile.write( (const char*) &(this->m_verticies[index].green), sizeof(float) );
			thePlyFile.write( (const char*) &(this->m_verticies[index].blue), sizeof(float) );
			thePlyFile.write( (const char*) &(this->m_verticies[index].alpha), sizeof(float) );
		}
	}// if ( this->m_PlyHeaderInfo.bHasColourRGBInFile )
	timer.UpdateLongDuration();

	// Vertex tangents in the file?
	if ( this->m_PlyHeaderInfo.bHasTangentsInFile )
	{
		for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
		{
			thePlyFile.write( (const char*) &(this->m_verticies[index].tangent.x), sizeof(float) );
			thePlyFile.write( (const char*) &(this->m_verticies[index].tangent.y), sizeof(float) );
			thePlyFile.write( (const char*) &(this->m_verticies[index].tangent.z), sizeof(float) );
		}
	}//if ( this->m_PlyHeaderInfo.bHasTangentsInFile )
	timer.UpdateLongDuration();

	// Vertex binormals in the file?
	if ( this->m_PlyHeaderInfo.bHasBiNormalsInFile )
	{
		for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
		{
			thePlyFile.write( (const char*) &(this->m_verticies[index].binormal.x), sizeof(float) );
			thePlyFile.write( (const char*) &(this->m_verticies[index].binormal.y), sizeof(float) );
			thePlyFile.write( (const char*) &(this->m_verticies[index].binormal.z), sizeof(float) );
		}
	}//if ( this->m_PlyHeaderInfo.bHasBiNormalsInFile )
	timer.UpdateLongDuration();

	thePlyFile.close();

	timer.UpdateLongDuration();
	this->m_lastLoadOrSaveTime = timer.get_fLondDurationTotalSeconds();

	return true;
}

bool CPlyFile5nt::OpenGDPFile(std::wstring fileName, std::wstring &error)	// GDP model format (basically a binary PLY version)
{
	CHRTimer timer;
	timer.Reset();
	timer.Start();

	std::ifstream thePlyFile(fileName.c_str(), std::ios::binary );
	if (!thePlyFile.is_open())
	{
		error = L"Can't open the file. Sorry it didn't work out.";
		return false;
	}

	timer.UpdateLongDuration();

	thePlyFile.seekg(0, std::ios::end);
	// reutrns "std::ios::pos_type" type (which seems to be int in VC++)
	unsigned int fileSize = static_cast<unsigned int>( thePlyFile.tellg() );	
	// Return to start
	thePlyFile.seekg(0, std::ios::beg);
		
	timer.UpdateLongDuration();

	//std::vector<char> data( static_cast<unsigned int>(fileSize), 0);
	//this->m_vecRawData.resize( static_cast<unsigned int>(fileSize), 0);

	char* pRawData = new char[ fileSize ];
	unsigned int curIndex = 0;				// Location in the array
	
	// read the file
	//std::vector<char> data;
	//thePlyFile.read( &data[0], fileSize);
	//thePlyFile.read( &(this->m_vecRawData[0]), fileSize);
	thePlyFile.read( pRawData, fileSize );
	thePlyFile.close();

	timer.UpdateLongDuration();

	this->m_fileInformation.fileName = fileName;

	// char 0, 1, 2: "gdp"
	std::string gdpHeaderName = this->m_gdp_ReadStringFromCharArray( &(pRawData[0]), 3 );
	if ( gdpHeaderName != "gdp" )
	{	// It's not a GDP file
		error = L"ERROR: Isn't a valid GDP file.";
		return false;
	}

	this->m_fileInformation.fileType = CFileInfo::MODEL_FILE_TYPE_GDP;


	// char 3: version - this one loads only version 1
	char gdpVersion = pRawData[3];

	// char 4: is little endian (Do we even need this...?)
	this->m_PlyHeaderInfo.plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_UNKNOWN;
	if ( this->m_gdp_bReadBoolFromCharArray( &(pRawData[4]) ) )
	{	
		this->m_PlyHeaderInfo.plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_IS_GDP_LITTLE_ENDIAN;	
	}
	else
	{	// This format is likely to end in tears as it hasn't been tested very much at all (like hardly at all)
		this->m_PlyHeaderInfo.plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_IS_GDP_BIG_ENDIAN;
	}

	// char 5: has normals, in xyz format (0 or 1)
	this->m_PlyHeaderInfo.bHasNormalsInFile = this->m_gdp_bReadBoolFromCharArray( &(pRawData[5]) );
	// char 6: has UVs
	this->m_PlyHeaderInfo.bHasTextureCoordinatesInFile = this->m_gdp_bReadBoolFromCharArray( &(pRawData[6]) );
	// char 7: has tangent XYZ
	this->m_PlyHeaderInfo.bHasTangentsInFile = this->m_gdp_bReadBoolFromCharArray( &(pRawData[7]) );
	// char 8: has binormal XYZ
	this->m_PlyHeaderInfo.bHasBiNormalsInFile = this->m_gdp_bReadBoolFromCharArray( &(pRawData[8]) );
	// char 9: has colour RGB
	this->m_PlyHeaderInfo.bHasColourRGBAInFile = this->m_gdp_bReadBoolFromCharArray( &(pRawData[9]) );

	this->m_PlyHeaderInfo.numberOfVertices = this->m_gdp_ReadInt32FromCharArray( &(pRawData[32]) );
	this->m_PlyHeaderInfo.numberOfElements = this->m_gdp_ReadInt32FromCharArray( &(pRawData[38]) );
	
	// Read 
	// Vertices XYZ and ELEMENTs (ALWAYS has these), then 
	// Vertex: nxyz, UVs, colours, tangents, binormals (because they are optional)
	
	// Allocate the number of vertices and elements and load empty values
	this->m_verticies.clear();
	this->m_verticies.reserve( this->m_PlyHeaderInfo.numberOfVertices );
	for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
	{
		this->m_verticies.push_back( PlyVertex() );
	}
	this->m_elements.clear();
	this->m_elements.reserve( this->m_PlyHeaderInfo.numberOfElements );
	for ( int index = 0; index != this->GetNumberOfElements(); index++ )
	{
		this->m_elements.push_back( PlyElement() );
	}
	timer.UpdateLongDuration();

	// Place the index at the end of the header:
	unsigned int arrayCurrentIndex = CPlyFile5nt::GDPHEADERSIZE;

	// ALWAYS has XYZ...
	// Now load the information from the file
	for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
	{
//		PlyVertex tempVert;
		//unsigned int indexOfFloat = CPlyFile5nt::GDPHEADERSIZE + ( index * sizeof(float) * 3 );
		//unsigned int indexOfFloat = arrayCurrentIndex + ( index * sizeof(float) * 3 );

		//tempVert.xyz.x = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[indexOfFloat + 0 * sizeof(float)] ) );
		//tempVert.xyz.y = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[indexOfFloat + 1 * sizeof(float)] ) );
		//tempVert.xyz.z = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[indexOfFloat + 2 * sizeof(float)] ) );
//		tempVert.xyz.x = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 0 * sizeof(float)] ) );
//		tempVert.xyz.y = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 1 * sizeof(float)] ) );
//		tempVert.xyz.z = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 2 * sizeof(float)] ) );
		this->m_verticies[index].xyz.x = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 0 * sizeof(float)] ) );
		this->m_verticies[index].xyz.y = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 1 * sizeof(float)] ) );
		this->m_verticies[index].xyz.z = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 2 * sizeof(float)] ) );

//		this->m_verticies[index] = tempVert;

		// Move pointer to next vertex (3 floats away)
		arrayCurrentIndex += ( 3 * sizeof(float) );
	}//for ( int index = 0;....
	timer.UpdateLongDuration();

	// 	ALWAYS has elements
	for ( int index = 0; index != this->GetNumberOfElements(); index++ )
	{
//		PlyElement tempElement;
		
//		tempElement.vertex_index_1 = this->m_gdp_ReadInt32FromCharArray( &(pRawData[arrayCurrentIndex + 0 * sizeof(int)] ) );
//		tempElement.vertex_index_2 = this->m_gdp_ReadInt32FromCharArray( &(pRawData[arrayCurrentIndex + 1 * sizeof(int)] ) );
//		tempElement.vertex_index_3 = this->m_gdp_ReadInt32FromCharArray( &(pRawData[arrayCurrentIndex + 2 * sizeof(int)] ) );
		this->m_elements[index].vertex_index_1 = this->m_gdp_ReadInt32FromCharArray( &(pRawData[arrayCurrentIndex + 0 * sizeof(int)] ) );
		this->m_elements[index].vertex_index_2 = this->m_gdp_ReadInt32FromCharArray( &(pRawData[arrayCurrentIndex + 1 * sizeof(int)] ) );
		this->m_elements[index].vertex_index_3 = this->m_gdp_ReadInt32FromCharArray( &(pRawData[arrayCurrentIndex + 2 * sizeof(int)] ) );

//		this->m_elements[index] = tempElement;

		// Move pointer to next element (3 ints away)
		arrayCurrentIndex += ( 3 * sizeof(int) );
	}//for ( int index = 0;...
	timer.UpdateLongDuration();

	// Vertex Normals in the file?
	if ( this->bHasNormalsInFile() )
	{
		for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
		{
			this->m_verticies[index].nx = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 0 * sizeof(float)] ) );
			this->m_verticies[index].ny = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 1 * sizeof(float)] ) );
			this->m_verticies[index].nz = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 2 * sizeof(float)] ) );
			// Move pointer to next vertex (3 floats away)
			arrayCurrentIndex += ( 3 * sizeof(float) );
		}//for ( int index = 0;....
	}//if ( this->bHadNormalsInFile() )
	timer.UpdateLongDuration();

	// Vertex UVs in the file?
	if ( this->bHasTextureCoordinatesInFile() )
	{
		for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
		{
			this->m_verticies[index].tex0u = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 0 * sizeof(float)] ) );
			this->m_verticies[index].tex0v = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 1 * sizeof(float)] ) );
			// I don't really remember why there are two sets of texture coords...
			this->m_verticies[index].tex1u = this->m_verticies[index].tex0u;
			this->m_verticies[index].tex1v = this->m_verticies[index].tex0v;
			// Move pointer to next vertex (3 floats away)
			arrayCurrentIndex += ( 2 * sizeof(float) );
		}//for ( int index = 0;....
	}//if ( this->bHadTextureCoordinatesInFile() )
	timer.UpdateLongDuration();

	// Vertex Colours in the file?
	if ( this->m_PlyHeaderInfo.bHasColourRGBAInFile )
	{
		for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
		{
			this->m_verticies[index].red = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 0 * sizeof(float)] ) );
			this->m_verticies[index].green = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 1 * sizeof(float)] ) );
			this->m_verticies[index].blue = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 2 * sizeof(float)] ) );
			this->m_verticies[index].alpha = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 3 * sizeof(float)] ) );
			// Move pointer to next vertex (4 floats away)
			arrayCurrentIndex += ( 4 * sizeof(float) );
		}//for ( int index = 0;....
	}//if ( this->m_PlyHeaderInfo.bHasColourRGBInFile )
	timer.UpdateLongDuration();


	// Vertex tangents in the file?
	if ( this->m_PlyHeaderInfo.bHasTangentsInFile )
	{
		for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
		{
			this->m_verticies[index].tangent.x = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 0 * sizeof(float)] ) );
			this->m_verticies[index].tangent.y = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 1 * sizeof(float)] ) );
			this->m_verticies[index].tangent.z = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 2 * sizeof(float)] ) );
			// Move pointer to next vertex (3 floats away)
			arrayCurrentIndex += ( 3 * sizeof(float) );
		}//for ( int index = 0;....
	}//if ( this->m_PlyHeaderInfo.bHasTangentsInFile )
	timer.UpdateLongDuration();

	// Vertex binormals in the file?
	if ( this->m_PlyHeaderInfo.bHasBiNormalsInFile )
	{
		for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
		{
			this->m_verticies[index].binormal.x = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 0 * sizeof(float)] ) );
			this->m_verticies[index].binormal.y = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 1 * sizeof(float)] ) );
			this->m_verticies[index].binormal.z = this->m_gdp_ReadFloat32FromCharArray( &(pRawData[arrayCurrentIndex + 2 * sizeof(float)] ) );
			// Move pointer to next vertex (3 floats away)
			arrayCurrentIndex += ( 3 * sizeof(float) );
		}//for ( int index = 0;....
	}//if ( this->m_PlyHeaderInfo.bHasBiNormalsInFile )
	timer.UpdateLongDuration();

	//PlyVertex debugVerted = this->m_verticies[this->GetNumberOfVerticies() -1];

	delete [] pRawData;

	timer.UpdateLongDuration();
	timer.Stop();

	this->m_lastLoadOrSaveTime = timer.get_fLondDurationTotalSeconds();

	return true;
}

bool CPlyFile5nt::bFloatCompareEqual( float a, float b, float epsilon )
{
	this->m_plyCompareComparisonsPerformedCount++;
	float diff = a - b;
	if ( diff < 0.0f) {	diff = -diff; }
	if ( diff < epsilon )
	{
		return true;
	}
	return false;
}

void CPlyFile5nt::m_addToVecErrors( std::vector<std::string> &vecErrors, std::stringstream &ssError )
{
	if ( this->m_plyCompareCurrentErrorCount < this->m_plyCompareErrorCountLimit )
	{
		vecErrors.push_back( ssError.str() );
		this->m_plyCompareCurrentErrorCount++;
	}
	return;
}

void CPlyFile5nt::m_addToVecErrors( std::vector<std::string> &vecErrors, std::string error )
{
	if ( this->m_plyCompareCurrentErrorCount < this->m_plyCompareErrorCountLimit )
	{
		vecErrors.push_back( error );
		this->m_plyCompareCurrentErrorCount++;
	}
	return;
}


// Tests to see if the files are 'the same' 
// Same number of vertices and indices. NOTE: Will exit early if this is the case (i.e. will NOT compare vertex and index values)
// Same float values in vertices, to within a certain epsilon (default is 0.0001f)
// Returns true if they are "the same" (if the are the same object, it's the same pointer, will return true, but with an error-warning)
// vecErrors list details (every single time there is a difference), up to the errorCountLimit
bool CPlyFile5nt::bComparePlyFileObjects( CPlyFile5nt* pPlyToCompare, std::vector<std::string> &vecErrors, float floatEpsilonForEquivalency /*= 0.0001f*/, int errorCountLimit /*= INT_MAX*/ )
{
	if ( this == pPlyToCompare )
	{
		vecErrors.push_back( "Warning: You are comparing the same object (pointer is the same).");
		return true;
	}

	bool bTheyAreTheSame = true;

	// This is just for my peace of mind...
	this->m_plyCompareComparisonsPerformedCount = 0;

	if ( this->GetNumberOfVerticies() != pPlyToCompare->GetNumberOfVerticies() )
	{
		std::stringstream ssError;
		ssError << "ERROR: Different number of vertices: " << this->GetNumberOfVerticies() << " vs " << pPlyToCompare->GetNumberOfVerticies();
		vecErrors.push_back( ssError.str() );
		bTheyAreTheSame = false;
	}

	if ( this->GetNumberOfElements() != pPlyToCompare->GetNumberOfElements() )
	{
		std::stringstream ssError;
		ssError << "ERROR: Different number of elements: " << this->GetNumberOfElements() << " vs " << pPlyToCompare->GetNumberOfElements();
		vecErrors.push_back( ssError.str() );
		bTheyAreTheSame = false;
	}

	// They have the same number of vertices and elements, so do a detailed comparison
	this->m_plyCompareCurrentErrorCount = 0;
	this->m_plyCompareErrorCountLimit = errorCountLimit;

	for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
	{
		PlyVertex refVert = this->getVertex_at( index );
		PlyVertex testVert = pPlyToCompare->getVertex_at( index );
		if ( !this->bFloatCompareEqual( refVert.xyz.x, testVert.xyz.x, floatEpsilonForEquivalency ) )
		{
			std::stringstream ssError;
			ssError << index << " x values don't match: " << refVert.xyz.x << " != " << testVert.xyz.x << std::endl;
			this->m_addToVecErrors( vecErrors, ssError );
			bTheyAreTheSame = false;
		}
		if ( !bFloatCompareEqual( refVert.xyz.y, testVert.xyz.y, floatEpsilonForEquivalency ) )
		{
			std::stringstream ssError;
			ssError << index << " y values don't match: " << refVert.xyz.y << " != " << testVert.xyz.y << std::endl;
			this->m_addToVecErrors( vecErrors, ssError );
			bTheyAreTheSame = false;
		}
		if ( !bFloatCompareEqual( refVert.xyz.z, testVert.xyz.z, floatEpsilonForEquivalency ) )
		{
			std::stringstream ssError;
			ssError << index << " z values don't match: " << refVert.xyz.z << " != " << testVert.xyz.z << std::endl;
			this->m_addToVecErrors( vecErrors, ssError );
			bTheyAreTheSame = false;
		}
	}//for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )

	// Test indices
	for ( int index = 0; index != this->GetNumberOfElements(); index++ )
	{
		if ( this->getElement_at(index).vertex_index_1 != pPlyToCompare->getElement_at(index).vertex_index_1 )
		{
			std::stringstream ssError;
			ssError << index << " element index 1 doesn't match: " << this->getElement_at(index).vertex_index_1 << " != " << pPlyToCompare->getElement_at(index).vertex_index_1 << std::endl;
			this->m_addToVecErrors( vecErrors, ssError );
			bTheyAreTheSame = false;
		}
		if ( this->getElement_at(index).vertex_index_2 != pPlyToCompare->getElement_at(index).vertex_index_2 )
		{
			std::stringstream ssError;
			ssError << index << " element index 2 doesn't match: " << this->getElement_at(index).vertex_index_2 << " != " << pPlyToCompare->getElement_at(index).vertex_index_2 << std::endl;
			this->m_addToVecErrors( vecErrors, ssError );
			bTheyAreTheSame = false;
		}
		if ( this->getElement_at(index).vertex_index_3 != pPlyToCompare->getElement_at(index).vertex_index_3 )
		{
			std::stringstream ssError;
			ssError << index << " element index 3 doesn't match: " << this->getElement_at(index).vertex_index_3 << " != " << pPlyToCompare->getElement_at(index).vertex_index_3 << std::endl;
			this->m_addToVecErrors( vecErrors, ssError );
			bTheyAreTheSame = false;
		}
	}//for ( int index = 0; index != this->GetNumberOfElements(); index++ )

	// These are all the optional items that might be in the files...

	//__   __       _             _  _                    _    ___ 
	//\ \ / /__ _ _| |_ _____ __ | \| |___ _ _ _ __  __ _| |__|__ \
	// \ V / -_) '_|  _/ -_) \ / | .` / _ \ '_| '  \/ _` | (_-< /_/
	//  \_/\___|_|  \__\___/_\_\ |_|\_\___/_| |_|_|_\__,_|_/__/(_)                                                               
	if ( this->bHasNormalsInFile() )
	{	// This has normals. Does the comparison file have normals as well?
		if ( pPlyToCompare->bHasNormalsInFile() )
		{	// Yup
			for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
			{
				PlyVertex refVert = this->getVertex_at( index );
				PlyVertex testVert = pPlyToCompare->getVertex_at( index );
				if ( !this->bFloatCompareEqual( refVert.nx, testVert.nx, floatEpsilonForEquivalency ) )
				{
					std::stringstream ssError;
					ssError << index << " NORMAL x values don't match: " << refVert.nx << " != " << testVert.nx << std::endl;
					this->m_addToVecErrors( vecErrors, ssError );
					bTheyAreTheSame = false;
				}
				if ( !bFloatCompareEqual( refVert.ny, testVert.ny, floatEpsilonForEquivalency ) )
				{
					std::stringstream ssError;
					ssError << index << " NORMAL y values don't match: " << refVert.ny << " != " << testVert.ny << std::endl;
					this->m_addToVecErrors( vecErrors, ssError );
					bTheyAreTheSame = false;
				}
				if ( !bFloatCompareEqual( refVert.nz, testVert.nz, floatEpsilonForEquivalency ) )
				{
					std::stringstream ssError;
					ssError << index << " NORMAL z values don't match: " << refVert.nz << " != " << testVert.nz << std::endl;
					this->m_addToVecErrors( vecErrors, ssError );
					bTheyAreTheSame = false;
				}
			}//for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )			
		}//if ( pPlyToCompare->bHadNormalsInFile() )
		else
		{
			this->m_addToVecErrors( vecErrors, "ERROR: This object has normals, but comparison object does NOT." );
			bTheyAreTheSame = false;
		}//if ( pPlyToCompare->bHadNormalsInFile() )
	}
	else
	{	// This DOESN'T have normals, but does the comparison file have them?
		if ( pPlyToCompare->bHasNormalsInFile() )
		{	// Yup, so that's not good
			this->m_addToVecErrors( vecErrors, "ERROR: This object DOESN'T have normals, but comparison object DOES." );
			bTheyAreTheSame = false;
		}
	}//if ( this->bHadNormalsInFile() )

	
	//__   __       _             _   ___   __  ___ 
	//\ \ / /__ _ _| |_ _____ __ | | | \ \ / /_|__ \
	// \ V / -_) '_|  _/ -_) \ / | |_| |\ V (_-< /_/
	//  \_/\___|_|  \__\___/_\_\  \___/  \_//__/(_)                                              	
	if ( this->bHasTextureCoordinatesInFile() )
	{	// This has UVs.. how about the other object
		if ( pPlyToCompare->bHasTextureCoordinatesInFile() )
		{	//Yup, so compare them
			for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
			{
				PlyVertex refVert = this->getVertex_at( index );
				PlyVertex testVert = pPlyToCompare->getVertex_at( index );
				if ( !this->bFloatCompareEqual( refVert.tex0u, testVert.tex0u, floatEpsilonForEquivalency ) )
				{
					std::stringstream ssError;
					ssError << index << " Texture Coord U or S values don't match: " << refVert.tex0u << " != " << testVert.tex0u << std::endl;
					this->m_addToVecErrors( vecErrors, ssError );
					bTheyAreTheSame = false;
				}
				if ( !bFloatCompareEqual( refVert.tex0v, testVert.tex0v, floatEpsilonForEquivalency ) )
				{
					std::stringstream ssError;
					ssError << index << " Texture Coords V or T values don't match: " << refVert.tex0v << " != " << testVert.tex0v << std::endl;
					this->m_addToVecErrors( vecErrors, ssError );
					bTheyAreTheSame = false;
				}
			}//for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )			
		}
		else
		{	// This has, but comparison object doesn't
			this->m_addToVecErrors( vecErrors, "ERROR: This object has texture coords, but comparison object does NOT." );
			bTheyAreTheSame = false;
		}//if ( pPlyToCompare->bHadTextureCoordinatesInFile() )
	}
	else
	{	// This DOESN'T have UVs, how about the other object
		if ( pPlyToCompare->bHasTextureCoordinatesInFile() )
		{	// Yup, so that's not good
			this->m_addToVecErrors( vecErrors, "ERROR: This object DOESN'T have texture coords, but comparison object DOES." );
			bTheyAreTheSame = false;
		}//if ( pPlyToCompare->bHadTextureCoordinatesInFile() )
	}//if ( this->bHadTextureCoordinatesInFile() )


	//__   __       _             ___  ___ ___    ___     _                 ___ 
	//\ \ / /__ _ _| |_ _____ __ | _ \/ __| _ )  / __|___| |___ _  _ _ _ __|__ \
	// \ V / -_) '_|  _/ -_) \ / |   / (_ | _ \ | (__/ _ \ / _ \ || | '_(_-< /_/
	//  \_/\___|_|  \__\___/_\_\ |_|_\\___|___/  \___\___/_\___/\_,_|_| /__/(_) 
   if ( this->bHasRGBAColoursInFile() )
	{	// This has RGB Colours.. how about the other object
		if ( pPlyToCompare->bHasRGBAColoursInFile() )
		{	//Yup, so compare them
			for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
			{
				PlyVertex refVert = this->getVertex_at( index );
				PlyVertex testVert = pPlyToCompare->getVertex_at( index );
				if ( !this->bFloatCompareEqual( refVert.red, testVert.red, floatEpsilonForEquivalency ) )
				{
					std::stringstream ssError;
					ssError << index << " RGBA 'red' values don't match: " << refVert.red << " != " << testVert.red << std::endl;
					this->m_addToVecErrors( vecErrors, ssError );
					bTheyAreTheSame = false;
				}
				if ( !this->bFloatCompareEqual( refVert.green, testVert.green, floatEpsilonForEquivalency ) )
				{
					std::stringstream ssError;
					ssError << index << " RGBA 'green' values don't match: " << refVert.green << " != " << testVert.green << std::endl;
					this->m_addToVecErrors( vecErrors, ssError );
					bTheyAreTheSame = false;
				}
				if ( !this->bFloatCompareEqual( refVert.blue, testVert.blue, floatEpsilonForEquivalency ) )
				{
					std::stringstream ssError;
					ssError << index << " RGBA 'blue' values don't match: " << refVert.blue << " != " << testVert.blue << std::endl;
					this->m_addToVecErrors( vecErrors, ssError );
					bTheyAreTheSame = false;
				}
				if ( !this->bFloatCompareEqual( refVert.alpha, testVert.alpha, floatEpsilonForEquivalency ) )
				{
					std::stringstream ssError;
					ssError << index << " RGBA 'alpha' values don't match: " << refVert.alpha << " != " << testVert.alpha << std::endl;
					this->m_addToVecErrors( vecErrors, ssError );
					bTheyAreTheSame = false;
				}
			}//for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )			
		}
		else
		{	// This has, but comparison object doesn't
			this->m_addToVecErrors( vecErrors, "ERROR: This object has RBGA colours, but comparison object does NOT." );
			bTheyAreTheSame = false;
		}//if ( pPlyToCompare->bHasRGBColoursInFile() )
	}
	else
	{	// This DOESN'T have RGB Colours, how about the other object
		if ( pPlyToCompare->bHasRGBAColoursInFile() )
		{	// Yup, so that's not good
			this->m_addToVecErrors( vecErrors, "ERROR: This object DOESN'T have RGBA colours, but comparison object DOES." );
			bTheyAreTheSame = false;
		}//if ( pPlyToCompare->bHasRGBColoursInFile() )
	}//if ( this->bHasRGBColoursInFile() )


	//__   __       _             _                          _      ___ 
	//\ \ / /__ _ _| |_ _____ __ | |_ __ _ _ _  __ _ ___ _ _| |_ __|__ \
	// \ V / -_) '_|  _/ -_) \ / |  _/ _` | ' \/ _` / -_) ' \  _(_-< /_/
	//  \_/\___|_|  \__\___/_\_\  \__\__,_|_||_\__, \___|_||_\__/__/(_) 
	//                                         |___/  
	if ( this->bHasTagentsInFile() )
	{	// This has RGB Colours.. how about the other object
		if ( pPlyToCompare-bHasTagentsInFile() )
		{	//Yup, so compare them
			for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
			{
				PlyVertex refVert = this->getVertex_at( index );
				PlyVertex testVert = pPlyToCompare->getVertex_at( index );
				if ( !this->bFloatCompareEqual( refVert.tangent.x, testVert.tangent.x, floatEpsilonForEquivalency ) )
				{
					std::stringstream ssError;
					ssError << index << " TANGENT x values don't match: " << refVert.tangent.x << " != " << testVert.tangent.x << std::endl;
					this->m_addToVecErrors( vecErrors, ssError );
					bTheyAreTheSame = false;
				}
				if ( !this->bFloatCompareEqual( refVert.tangent.y, testVert.tangent.y, floatEpsilonForEquivalency ) )
				{
					std::stringstream ssError;
					ssError << index << " TANGENT y values don't match: " << refVert.tangent.y << " != " << testVert.tangent.y << std::endl;
					this->m_addToVecErrors( vecErrors, ssError );
					bTheyAreTheSame = false;
				}
				if ( !this->bFloatCompareEqual( refVert.tangent.z, testVert.tangent.z, floatEpsilonForEquivalency ) )
				{
					std::stringstream ssError;
					ssError << index << " TANGENT z values don't match: " << refVert.tangent.z << " != " << testVert.tangent.z << std::endl;
					this->m_addToVecErrors( vecErrors, ssError );
					bTheyAreTheSame = false;
				}
			}//for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )			
		}
		else
		{	// This has, but comparison object doesn't
			this->m_addToVecErrors( vecErrors, "ERROR: This object has tangents, but comparison object does NOT." );
			bTheyAreTheSame = false;
		}//if ( pPlyToCompare->bHasTagentsInFile() )
	}
	else
	{	// This DOESN'T have tangent values, how about the other object
		if ( pPlyToCompare->bHasTagentsInFile() )
		{	// Yup, so that's not good
			this->m_addToVecErrors( vecErrors, "ERROR: This object DOESN'T have tangents, but comparison object DOES." );
			bTheyAreTheSame = false;
		}//if ( pPlyToCompare->bHasTagentsInFile() )
	}//if ( this->bHasTagentsInFile() )

	//__   __       _             _    _                         _    ___ 
	//\ \ / /__ _ _| |_ _____ __ | |__(_)_ _  ___ _ _ _ __  __ _| |__|__ \
	// \ V / -_) '_|  _/ -_) \ / | '_ \ | ' \/ _ \ '_| '  \/ _` | (_-< /_/
	//  \_/\___|_|  \__\___/_\_\ |_.__/_|_||_\___/_| |_|_|_\__,_|_/__/(_) 
	if ( this->bHasBinormalsInFile() )
	{	// This has binormals.. how about the other object
		if ( pPlyToCompare->bHasBinormalsInFile() )
		{	//Yup, so compare them
			for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )
			{
				PlyVertex refVert = this->getVertex_at( index );
				PlyVertex testVert = pPlyToCompare->getVertex_at( index );
				if ( !this->bFloatCompareEqual( refVert.binormal.x, testVert.binormal.x, floatEpsilonForEquivalency ) )
				{
					std::stringstream ssError;
					ssError << index << " BINORMAL x values don't match: " << refVert.binormal.x << " != " << testVert.binormal.x << std::endl;
					this->m_addToVecErrors( vecErrors, ssError );
					bTheyAreTheSame = false;
				}
				if ( !this->bFloatCompareEqual( refVert.binormal.y, testVert.binormal.y, floatEpsilonForEquivalency ) )
				{
					std::stringstream ssError;
					ssError << index << " TANGENT y values don't match: " << refVert.binormal.y << " != " << testVert.binormal.y << std::endl;
					this->m_addToVecErrors( vecErrors, ssError );
					bTheyAreTheSame = false;
				}
				if ( !this->bFloatCompareEqual( refVert.binormal.z, testVert.binormal.z, floatEpsilonForEquivalency ) )
				{
					std::stringstream ssError;
					ssError << index << " TANGENT z values don't match: " << refVert.binormal.z << " != " << testVert.binormal.z << std::endl;
					this->m_addToVecErrors( vecErrors, ssError );
					bTheyAreTheSame = false;
				}
			}//for ( int index = 0; index != this->GetNumberOfVerticies(); index++ )			
		}
		else
		{	// This has, but comparison object doesn't
			this->m_addToVecErrors( vecErrors, "ERROR: This object has binormals, but comparison object does NOT." );
			bTheyAreTheSame = false;
		}//if ( pPlyToCompare->bHasBinormalsInFile() )
	}
	else
	{	// This DOESN'T have binormals values, how about the other object
		if ( pPlyToCompare->bHasTagentsInFile() )
		{	// Yup, so that's not good
			this->m_addToVecErrors( vecErrors, "ERROR: This object DOESN'T have binormals, but comparison object DOES." );
			bTheyAreTheSame = false;
		}//if ( pPlyToCompare->bHasBinormalsInFile() )
	}//if ( this->bHasBinormalsInFile() )                                                                     



	// All done.
	return bTheyAreTheSame;
}

float CPlyFile5nt::getLastLoadOrSaveTime(void)
{
	return this->m_lastLoadOrSaveTime;
}

std::wstring CPlyFile5nt::GetFilenameWithoutExtension(std::wstring fileNameWithExtension, bool bOnlyLookForKnowExtensions /*= true*/ )
{
	std::reverse( fileNameWithExtension.begin(), fileNameWithExtension.end() );
	//Look for the first slash (of either kind)

	std::wstring::size_type charIndexFirstPeriod = fileNameWithExtension.find_first_of( L"." );

	// Did we find the "."? Is the period the first thing in the string?
	if ( (charIndexFirstPeriod == std::wstring::npos) || (charIndexFirstPeriod == 0) )
	{	// There's no ".", or it's the first thing in the string, so it will be hard find an extension...
		// Return it unchanged
		std::reverse( fileNameWithExtension.begin(), fileNameWithExtension.end() );
		return fileNameWithExtension;
	}

	std::wstring extension = fileNameWithExtension.substr( 0, charIndexFirstPeriod );
	std::wstring fileNameNoExtension = fileNameWithExtension.substr( charIndexFirstPeriod + 1, std::wstring::npos );

	std::reverse( fileNameWithExtension.begin(), fileNameWithExtension.end() );

	std::reverse( extension.begin(), extension.end() );
	std::transform( extension.begin(), extension.end(), extension.begin(), ::towupper );
	std::reverse( fileNameNoExtension.begin(), fileNameNoExtension.end() );

	if ( bOnlyLookForKnowExtensions )
	{
		if ( ( extension == L"PLY" ) || ( extension == L"GDP") )
		{	// Has known extension
			return fileNameNoExtension;
		}
		// else it's an unknown extension
		return fileNameWithExtension;
	}

	// Look for any extension
	return fileNameNoExtension;
}

bool CPlyFile5nt::IsFilePresent(std::wstring fileName)
{
	bool bOpennedFile = false;
	std::ifstream testFile( fileName.c_str() );
	bOpennedFile = testFile.is_open();
	testFile.close();
	return bOpennedFile;
}

