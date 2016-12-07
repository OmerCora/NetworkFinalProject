#ifndef _cMeshTypeManager_HG_
#define _cMeshTypeManager_HG_

#include <string>
#include <vector>
#include <map>
//#include <GLFW\glfw3.h>		// For GLuint

//struct cPlyVertexXYZ
//{
//	cPlyVertexXYZ(): x(0.0f), y(0.0f), z(0.0f), nX(0.0f), nY(0.0f), nZ(0.0f) {}
//	float x;
//	float y;
//	float z;
//	float nX;
//	float nY;
//	float nZ;
//};

struct cPlyVertexXYZ_N_STU
{
	cPlyVertexXYZ_N_STU(): x(0.0f), y(0.0f), z(0.0f), nX(0.0f), nY(0.0f), nZ(0.0f), s(0.0f), t(0.0f), u(0.0f) {}
	float x;
	float y;
	float z;
	float nX;
	float nY;
	float nZ;
	float s;
	float t;
	float u;
};

struct cPlyTriFace
{
	cPlyTriFace(): v1(-1), v2(-1), v3(-1) {}	// can never be -1 in an actual model
	int v1;
	int v2;
	int v3;
};

class cMeshTypeManager 
{
public:
	cMeshTypeManager();
	~cMeshTypeManager();

	void ScaleEverythingToBoundingBoxOnLoad( bool bScale, float BoxWidth );


	// Loads file from disk, AND
	// Copies inforamtion into VAO (Vertex Array Object)
	bool LoadPlyFileIntoGLBuffer( unsigned int programID, std::string plyFileNameNoBasePath,  
								  bool bMakeFabulousRainbows = false );
	bool LoadPlyFileIntoGLBuffer( unsigned int programID, std::string plyFileNameNoBasePath, 
								  float vertRed, float vertGreen, float vertBlue );
	bool LoadPlyFileIntoGLBuffer( unsigned int programID, std::string plyFileNameNoBasePath, 
								  float vertRed, float vertGreen, float vertBlue, 
								  bool bMakeFabulousRainbows = false );

	// basepath is "" by default, and persists between calls to LoadPlyFileIntoGLBuffer
	void SetBaseFilePath( std::string basepath );

	bool LookUpMeshInfo( std::string meshName, 
						 unsigned int &VAO_ID,	    // GLuint by ref
						 int &numberOfIndices,
						 float &unitScale );		// by ref

private: 

	bool m_loadPlyModel( std::string filename, 
				   std::vector< cPlyVertexXYZ_N_STU > &vecPlyVerts, 
				   std::vector< cPlyTriFace > &vecPlyIndices );

	std::string m_basepath;

	bool m_bScaleEverythingToBoundingBoxOnLoad;	// = false by default
	float m_BoundingBoxScalingSize;	// = 1.0f by default
	
	void m_Normalize( std::vector< cPlyVertexXYZ_N_STU > &vecPlyVerts );

	void m_CalculateExtents( const std::vector< cPlyVertexXYZ_N_STU > &vecPlyVerts, 
                             float &minX, float &minY, float &minZ, 
                             float &maxX, float &maxY, float &maxZ, 
                             float &extentX, float &extentY, float &extentZ, 
                             float &maxExtent );


	struct VAOInfo
	{	
		VAOInfo() : VAO_ID(0), numberOfIndices(0), unitScaleValue(1.0f) {}
		std::string meshName;
		unsigned int VAO_ID;
		unsigned int numberOfIndices;
		float unitScaleValue;		// Multiply by this scale and object 1.0f
	};
	// Map... aka "dictionay" "look up table"
	std::map< std::string, VAOInfo > m_MapMeshNameToVAOInfo;
};

#endif
  