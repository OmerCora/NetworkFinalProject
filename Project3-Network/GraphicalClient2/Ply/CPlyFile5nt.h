#ifndef _CPlyFile5nt_DX11_HG_
#define _CPlyFile5nt_DX11_HG_

// Written by Michael Feeney, Fanshawe College, 2009-2015
// mfeeney@fanshawec.ca
// It may be distributed under the terms of the MIT License:
// https://opensource.org/licenses/MIT
// Use this code at your own risk. It is indented only as a learning aid.

// Fairly simple PLY file loader that also calculates the normal for the model.
//  Note that it assumes a certain winding order...
//	Can load the Stanford Bunny and most (?) other files from the 
//	The Stanford 3D Scanning Repository
//	http://graphics.stanford.edu/data/3Dscanrep/
// (Well, I haven't tested them all, but it loads the bunny, dragon, and armadillo...)
//
// See Greg Turk's paper and details of the bunny here:
// http://www.cc.gatech.edu/~turk/bunny/bunny.html

#include <string>
#include <vector>
#include "CVector3f.h"
#include "CPlyInfo.h"
#include <sstream>

// This structure holds the vertex information 
// as listed in the bunny file
struct PlyVertex
{
	PlyVertex(): nx(0.0f), ny(0.0f), nz(0.0f), 
				 tex0u(0.0f), tex0v(0.0f), tex1u(0.0f), tex1v(0.0f),
				 red(0.0f), green(0.0f), blue(0.0f), alpha(0.0f) {}
	CVector3f xyz; 
	//float x, y, z;
	float nx, ny, nz;
	float tex0u, tex0v;
	float tex1u, tex1v;
	// *********************
	float red, green, blue, alpha;
	CVector3f tangent;
	CVector3f binormal;
};

// This structure holds the elements, which 
// refer to the vertex indicies
struct PlyElement
{
	PlyElement(): vertex_index_1(0), vertex_index_2(0), vertex_index_3(0) {}
	int vertex_index_1;
	int vertex_index_2;
	int vertex_index_3;
};

struct CFileInfo
{
public:
	std::wstring fileName;
	enum enumFileType
	{
		MODEL_FILE_TYPE_PLY,
		MODEL_FILE_TYPE_GDP,
		MODEL_FILE_TYPE_UNKNOWN
	};	
	enumFileType fileType;
	CFileInfo() : fileType(MODEL_FILE_TYPE_UNKNOWN){};
	CFileInfo( std::wstring fileName, enumFileType fileType ) : fileName(fileName), fileType(fileType) {};
};


class CPlyFile5nt
{
public:
	CPlyFile5nt();
	~CPlyFile5nt();
	CPlyFile5nt( const CPlyFile5nt &rhs );	// Copy constructor
	CPlyFile5nt& operator=( const CPlyFile5nt &rhs );

	CFileInfo m_fileInformation;

	// error returns the reason for failure
	bool OpenPLYFile(std::wstring fileName, std::wstring &error);
	// Added November 1, 2014
	bool OpenPLYFile2(std::wstring fileName, std::wstring &error);
	bool SavePlyFileASCII(std::wstring fileName, bool bOverwrite, bool bIncludeNormals, bool bIncludeVertTex0, bool bIncludeVertTex1 );
	bool SaveGDPFile(std::wstring fileName, bool bOverwrite, std::wstring &error);
	bool OpenGDPFile(std::wstring fileName, std::wstring &error);	// GDP model format (basically a binary PLY version)
	// reads only the header information (number of vertices, etc.)
	bool ReadPLYFileHeader(std::wstring fileName, std::wstring &error);
	bool ReadGDPFileHeader(std::wstring fileName, std::wstring &error);
	bool IsFilePresent(std::wstring fileName);

	// Loading settings
	void SetMinFloatRoundToZero(float minRoundToZero );
	float GetMinFloatRoundToZeroValue(void);
	void SetRoundTinyFloatsToZeroOnLoadFlag(bool bNewRoundToZeroFlag );
	bool GetRoundTinyFloatsToZeroOnLoadFlag(void);
	void SetScaleRGBA_OneByteValuesToFloatZeroToOne(bool bEnabled);
	bool GetScaleRGBA_OneByteValuesToFloatZeroToOne(void);

	std::wstring GetFilenameWithoutExtension(std::wstring fileNameWithExtension, bool bOnlyLookForKnowExtensions = true);
	
	int GetNumberOfVerticies(void);
	int GetNumberOfElements(void);
	PlyVertex getVertex_at(std::vector<PlyVertex>::size_type index);
	PlyElement getElement_at(std::vector<PlyElement>::size_type index);
	float getMaxX(void); float getMinX(void); float getDeltaX(void);
	float getMaxY(void); float getMinY(void); float getDeltaY(void);
	float getMaxZ(void); float getMinZ(void); float getDeltaZ(void);
	float getMaxExtent(void);
	float getMaxExtent(bool bRecalculateExtents);	
	// *********************
	void normalizeTheModelBaby_Rock_n_Roll(void);
	//void normalizeTheModelBabyDirectXVersion_Rock_n_Roll(void);
	void normalizeTheModelBaby(void);
	void normalizeTheModelBabyDirectXVersion(void);

	// Manipulation operations
	void Scale( float scale );
	// Scales based on a different origin. Doesn't change the original origin, though.
	void Scale( float scale, CVector3f origin );
	// Scales to a bounding box of certain size. 
	// Assumes that size is entire extent of bounding box
	void ScaleToFit( float boundingBoxSize );

	void Translate( CVector3f trans );
	void AlignMinXToPlane( float xMinAxisPlane );
	void AlignMinYToPlane( float yMinAxisPlane );
	void AlignMinZToPlane( float zMinAxisPlane );
	void AlignMaxXToPlane( float xMaxAxisPlane );
	void AlignMaxYToPlane( float yMaxAxisPlane );
	void AlignMaxZToPlane( float zMaxAxisPlane );
	void ShiftToCentreOfVertices(void);


	enum enumTEXCOORDBIAS
	{
		POSITIVE_X, POSITIVE_Y, POSITIVE_Z
	};

	void GenTextureCoordsSpherical( enumTEXCOORDBIAS uBias, enumTEXCOORDBIAS vBias, bool basedOnNormals, float scale, bool fast );
	
	unsigned int GetID(void);
	// Added: March 14, 2012
	bool bHasNormalsInFile(void);
	bool bHasTextureCoordinatesInFile(void);
	bool bHasRGBAColoursInFile(void);
	bool bHasTagentsInFile(void);
	bool bHasBinormalsInFile(void);

	// Added: November 2, 2014
	bool bIsThisMachineIsBigEndian(void);
	// Added: June 8, 2013
	void normlizeExistingNomrals(void);	
	// End of Added

	void scaleVertices( float scaleFactor );
	void calcualteExtents(void);

	// Tests to see if the files are 'the same' 
	// Same number of vertices and indices. NOTE: Will exit early if this is the case (i.e. will NOT compare vertex and index values)
	// Same float values in vertices, to within a certain epsilon (default is 0.0001f)
	// Returns true if they are "the same" (if the are the same object, it's the same pointer, will return true, but with an error-warning)
	// vecErrors list details (every single time there is a difference), up to the errorCountLimit
	bool bComparePlyFileObjects( CPlyFile5nt* pPlyToCompare, std::vector<std::string> &vecErrors, float floatEpsilonForEquivalency = 0.0001f, int errorCountLimit = INT_MAX );
	bool bFloatCompareEqual( float a, float b, float epsilon );

	float getLastLoadOrSaveTime(void);

private:
	std::vector<PlyVertex> m_verticies;
	std::vector<PlyElement> m_elements;
	float m_minX, m_maxX, m_deltaX;
	float m_minY, m_maxY, m_deltaY;
	float m_minZ, m_maxZ, m_deltaZ;
	// *********************
	float m_centreX, m_centreY, m_centreZ;
	float m_maxExtent;

	float m_lastLoadOrSaveTime;

	bool m_b_ScaleRGBA_OneByteValuesToFloatZeroToOne;
	float m_minRoundToZero;
	bool m_bNewRoundToZeroFlag; 

	// This is used internally to decode the files. 
	// It's a separate class so that we can determine the loading interfaces easily
	class CPlyHeaderDescription
	{
	public:
		CPlyHeaderDescription();

		enum enumPlyHeaderLayout
		{
			HEADER_LAYOUT_IS_ASCII_XYZ = (INT_MAX - 5000),		// So it doesn't start at 0 or 1 (or whatever)
			HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ,
			HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_vertUV,
			HEADER_LAYOUT_IS_ASCII_XYZ_vertUV,
			HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_ucharRGBA_vertUV,

			HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_ucharRGB,	// Added June 2016
			HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_ucharRGBA,	// Added June 2016

			HEADER_LAYOUT_IS_BINARY_XYZ,
			HEADER_LAYOUT_IS_BINARY_XYZ_nXYZ,
			HEADER_LAYOUT_IS_BINARY_XYZ_nXYZ_vertUV,

			HEADER_LAYOUT_UNKNOWN
		};
		enumPlyHeaderLayout plyHeaderLayout;

		enum enumFormatASCIIorBinary
		{
			FORMAT_IS_ASCII = (CPlyHeaderDescription::HEADER_LAYOUT_UNKNOWN + 1),	// Start off after the last enum
			FORMAT_IS_BINARY_BIG_ENDIAN,			// Motorola, PowerPC often big endian
			FORMAT_IS_BINARY_LITTLE_ENDIAN,			// everyone else (Intel) is little
			FORMAT_IS_GDP_BIG_ENDIAN,
			FORMAT_IS_GDP_LITTLE_ENDIAN,
			FORMAT_UNKNOWN
		};

		enumFormatASCIIorBinary plyFormatASCIIorBinary; 

		// Based on the information contained inside, determine the header type
		void DeterminePlyFileType(void);
		bool bIsHeader_XYZ(void);
		bool bIsHeader_XYZ_nXYZ(void);
		bool bIsHeader_XYZ_nXYZ_vertUV(void);
		bool bIsHeader_XYZ_vertUV(void);
		bool bIsHeader_XYZ_nXYZ_ucharRGBA_vertUV(void);
		bool bIsHeader_XYZ_nXYZ_ucharRGB(void);		// Added June 2016
		bool bIsHeader_XYZ_nXYZ_ucharRGBA(void);	// Added June 2016


		inline bool bIsThisMachineIsBigEndian(void);	// Motorola, PowerPC often big endian - everyone else (Intel) is little

		int totalProperties;
		int x_propertyIndex;		int y_propertyIndex;	int z_propertyIndex;
		int normx_propertyIndex;	int normy_propertyIndex;	int normz_propertyIndex;
		int red_propertyIndex;		int green_propertyIndex;	int blue_propertyIndex;		int alpha_propertyIndex;

		int tex0u_propertyIndex;		// "u"
		int tex0v_propertyIndex;		// "v"
		int tex1u_propertyIndex;		// "tex1u"
		int tex1v_propertyIndex;		// "tex1v"

		int tangentX_propertyIndex;		int tangentY_propertyIndex;		int tangentZ_propertyIndex;
		int binormalX_propertyIndex;	int binormalY_propertyIndex;	int binormalZ_propertyIndex;


		// Added: March 14, 2012
		bool bHasTextureCoordinatesInFile;
		bool bHasNormalsInFile;
		bool bHasTangentsInFile;		// "tx, ty, tz"	when using assimp 
		bool bHasBiNormalsInFile;		// "bx, by, bz"	when using assimp
		bool bHasColourRGBAInFile;

		// Mainly used for the GDP file format
		static const char GDP_FALSE = 0;
		static const char GDP_TRUE = 1;


		int numberOfElements;
		int numberOfVertices;
	};

	CPlyHeaderDescription m_PlyHeaderInfo;

	// Used to assist in reading and writing GDP files:
	static const unsigned int GDPHEADERSIZE = 64;	// 64 chars in header
	void m_gdp_StoreBoolToCharArray( char* pCharArrLoc, bool bValue );
	bool m_gdp_bReadBoolFromCharArray( char* pCharArrLoc );
	void m_gdp_StoreInt32IntoCharArray( char* pCharArrayLocation, int theInt );
	int  m_gdp_ReadInt32FromCharArray( char* pCharArrLoc );
	void m_gdp_StoreFloat32IntoCharArray( char* pCharArrLoc, float fValue );
	float m_gdp_ReadFloat32FromCharArray( char* pCharArrLoc );
	std::string m_gdp_ReadStringFromCharArray( char* pCharArrLoc, int numberOfChars );
	void m_gdp_StoreStringIntoCharArray( char* pCharArrLoc, std::string theString );

	// This is used for the plyfile compare (will add to vector if under the errorCountLimit
	// (Added to make the comparison code a little easier to read, yo)
	void m_addToVecErrors( std::vector<std::string> &vecErrors, std::stringstream &ssError );
	void m_addToVecErrors( std::vector<std::string> &vecErrors, std::string error );
	int m_plyCompareErrorCountLimit;
	int m_plyCompareCurrentErrorCount;
	int m_plyCompareComparisonsPerformedCount;

	//int m_numberOfVertices;		// now in CPlyHeaderDescription
	//int m_numberOfElements;		// now in CPlyHeaderDescription

	// **********************************************************
	// **** Because the ply files can be of various formats, ****
	// ****	 this code is to allow loading 'most' ply files  ****
	// ****  not just the ones that are the 'Stanford bunny' ****
	// ****  format.                                         ****
	// **********************************************************
	//int m_totalProperties;		// now in CPlyHeaderDescription
	//int m_x_propertyIndex;		// now in CPlyHeaderDescription
	//int m_y_propertyIndex;		// now in CPlyHeaderDescription
	//int m_z_propertyIndex;		// now in CPlyHeaderDescription
	//int m_normx_propertyIndex;	// now in CPlyHeaderDescription
	//int m_normy_propertyIndex;	// now in CPlyHeaderDescription
	//int m_normz_propertyIndex;	// now in CPlyHeaderDescription
	//int m_red_propertyIndex;		// now in CPlyHeaderDescription
	//int m_green_propertyIndex;	// now in CPlyHeaderDescription
	//int m_blue_propertyIndex;		// now in CPlyHeaderDescription

	// Added: March 14, 2012
	//bool m_bHadTextureCoordinatesInFile;		// now in CPlyHeaderDescription
	//bool m_bHadNormalsInFile;					// now in CPlyHeaderDescription
	//int m_tex0u_propertyIndex;		// "u"			// now in CPlyHeaderDescription
	//int m_tex0v_propertyIndex;		// "v"			// now in CPlyHeaderDescription
	//int m_tex1u_propertyIndex;		// "tex1u"		// now in CPlyHeaderDescription
	//int m_tex1v_propertyIndex;		// "tex1v"		// now in CPlyHeaderDescription
	int m_UNKNOWN_IGNORE_propertyIndex;		// This is used when we don't know what they heck this index is.
	// End of Added

	// 
	void m_setIndexBasedOnPropertyName(int curIndex, std::wstring propName);
	void m_setIndexBasedOnPropertyNameASCII(int curIndex, std::string propName);	// For OpenPLY2()
	bool m_placeItemInAppropriateVertexLocation( std::wstring tempString, int propertyIndex, PlyVertex &tempVertex );

	unsigned int m_ID;
	static unsigned int statNextID;

	// *************************************************************************************************************
	// These are used for the various adapter. 
	class CDataReader
	{
	public:
		CDataReader();
		// Eventually, these may be inline, but not for now
		inline std::string ASCIIReadNextString( char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		inline float ASCIIReadNextFloat( char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		inline int ASCIIReadNextInt( char* pData, unsigned int &curIndex, const unsigned int &arraySize );

		void SetMinFloatRoundToZero(float minRoundToZero );
		float GetMinFloatRoundToZeroValue(void);
		void SetRoundTinyFloatsToZeroOnLoadFlag(bool bNewRoundToZeroFlag );
		bool GetRoundTinyFloatsToZeroOnLoadFlag(void);
		//inline float BINReadNextFloat( char* pData, unsigned int &curIndex );
		//inline int BINReadNextInt( char* pData, unsigned int &curIndex );

		static const float DEFAULTROUNDSMALLFLOATTOZEROVALUE; // = 0.001f;
	private:
		float m_roundToZeroValue;
		bool m_bRoundSmallFloatToZeroFlag;
	};


	class IVertexReader
	{
	public:
		virtual ~IVertexReader() {};
		virtual bool IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType ) = 0;
		virtual bool ProcessNextVertex( PlyVertex &vertex, char* pData, unsigned int &curIndex, const unsigned int &arraySize ) = 0;
		// Not technically an interface, but yolo: I can't see why on Earth you'd want to overload these
		// (and adding them to Every. Single. Derrived. Class. seems pretty pointless)
		virtual void SetScaleRGBA_OneByteValuesToFloatZeroToOne(bool bEnabled){ this->m_b_ScaleRGBA_OneByteValuesToFloatZeroToOne = bEnabled; }
		virtual bool GetScaleRGBA_OneByteValuesToFloatZeroToOne(void) { return this->m_b_ScaleRGBA_OneByteValuesToFloatZeroToOne; }
		virtual void ScaleRGBA_OneByteValuesToZeroToOne( PlyVertex &vertex )
		{
			vertex.red =	( vertex.red / 255.0f );
			vertex.green =	( vertex.green / 255.0f );
			vertex.blue =	( vertex.blue / 255.0f );
			vertex.alpha =	( vertex.alpha / 255.0f );
			return;
		}
	protected:
		bool m_b_ScaleRGBA_OneByteValuesToFloatZeroToOne;// = bEnabled;
	};


	// These are used for OpenPLYFile2()
	class CVertexReader_ASCII_fXYZ : public IVertexReader
	{
	public:
		virtual bool IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType );
		virtual bool ProcessNextVertex( PlyVertex &vertex, char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		CPlyFile5nt::CDataReader reader;
	};

	class CVertexReader_ASCII_fXYZ_nXYZ : public IVertexReader
	{
	public:
		virtual bool IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType );
		virtual bool ProcessNextVertex( PlyVertex &vertex, char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		CPlyFile5nt::CDataReader reader;
	};	
	
	// These are used for OpenPLYFile2()
	class CVertexReader_ASCII_fXYZ_nXYZ_vertUV : public IVertexReader
	{
	public:
		virtual bool IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType );
		virtual bool ProcessNextVertex( PlyVertex &vertex, char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		CPlyFile5nt::CDataReader reader;
	};
	// These are used for OpenPLYFile2()
	class CVertexReader_ASCII_fXYZ_vertUV : public IVertexReader
	{
	public:
		virtual bool IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType );
		virtual bool ProcessNextVertex( PlyVertex &vertex, char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		CPlyFile5nt::CDataReader reader;
	};
	// These are used for OpenPLYFile2()	
	class CVertexReader_ASCII_XYZ_nXYZ_ucharRGBA_vertUV : public IVertexReader	// HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_ucharRGBA_vertUV
	{
	public:
		virtual bool IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType );
		virtual bool ProcessNextVertex( PlyVertex &vertex, char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		CPlyFile5nt::CDataReader reader;
	};
	// Added June 2016
	class CVertexReader_ASCII_XYZ_nXYZ_ucharRGB : public IVertexReader	// HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_ucharRGB
	{
	public:
		virtual bool IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType );
		virtual bool ProcessNextVertex( PlyVertex &vertex, char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		CPlyFile5nt::CDataReader reader;
	};
	// Added June 2016
	class CVertexReader_ASCII_XYZ_nXYZ_ucharRGBA : public IVertexReader	// HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_ucharRGBA
	{
	public:
		virtual bool IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType );
		virtual bool ProcessNextVertex( PlyVertex &vertex, char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		CPlyFile5nt::CDataReader reader;
	};

	class IElementReader
	{
	public:
		virtual ~IElementReader() {};
		virtual bool ProcessNextElement( PlyElement &element, char* pData, unsigned int &curIndex, const unsigned int &arraySize ) = 0;
	};


	// This is the run-of-the-mill "property list uchar int vertex_indices" element reader
	// Specifically for triangles (example: "3 2070 2072 2073")
	class CElementReader_3intVert : public IElementReader
	{
	public:
		virtual bool ProcessNextElement( PlyElement &element, char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		CPlyFile5nt::CDataReader reader;
	};
};


#endif
