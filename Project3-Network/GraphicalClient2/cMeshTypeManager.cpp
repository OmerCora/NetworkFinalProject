#include "cMeshTypeManager.h"

//#include <vector>
#include <fstream>
#include "Utilities.h"		// for getRandFloat
#include "GlobalStuff.h"
//#include <glad/glad.h>			// OpenGL defines...
//#include <glm/glm.hpp>

//#include "vert_XYZ_RGB.h"
//#include "vert_XYZ_RGB_Nxyz.h"
#include "vert_XYZ_RGB_Nxyz_STU.h"		

#include "Ply/CPlyFile5nt.h"
#include "Ply/CStringHelper.h"


cMeshTypeManager::cMeshTypeManager()
{
	this->m_BoundingBoxScalingSize = 1.0f;
	this->m_bScaleEverythingToBoundingBoxOnLoad = false;

	return;
}

cMeshTypeManager::~cMeshTypeManager()
{
	// Add code here soon
	return;
}

void cMeshTypeManager::SetBaseFilePath( std::string basepath )
{
	this->m_basepath = basepath;
	return;
}

bool cMeshTypeManager::LoadPlyFileIntoGLBuffer( unsigned int programID, std::string plyFile,  
                                                bool bMakeFabulousRainbows /*= false*/ )
{
	return this->LoadPlyFileIntoGLBuffer( programID, plyFile, 
										  1.0f, 1.0f, 1.0f, bMakeFabulousRainbows );
}

bool cMeshTypeManager::LoadPlyFileIntoGLBuffer( unsigned int programID, std::string plyFile, 
                                                float vertRed, float vertGreen, float vertBlue )
{
	return this->LoadPlyFileIntoGLBuffer( programID, plyFile, 
										  vertRed, vertGreen, vertBlue, false );
}

void cMeshTypeManager::ScaleEverythingToBoundingBoxOnLoad( bool bScale, float BoxWidth )
{
	this->m_bScaleEverythingToBoundingBoxOnLoad = bScale;
	this->m_BoundingBoxScalingSize = BoxWidth;
	return;
}

void cMeshTypeManager::m_Normalize( std::vector< cPlyVertexXYZ_N_STU > &vecPlyVerts )
{
	for ( int index = 0; index != vecPlyVerts.size(); index++ )
	{
		glm::vec3 norm( vecPlyVerts[index].nX, vecPlyVerts[index].nY, vecPlyVerts[index].nZ );
		glm::normalize( norm );
		vecPlyVerts[index].nX = norm.x;
		vecPlyVerts[index].nY = norm.y;
		vecPlyVerts[index].nZ = norm.z;
	}
	return;
}


void cMeshTypeManager::m_CalculateExtents( const std::vector< cPlyVertexXYZ_N_STU > &vecPlyVerts, 
										   float &minX, float &minY, float &minZ, 
										   float &maxX, float &maxY, float &maxZ, 
										   float &extentX, float &extentY, float &extentZ, 
										   float &maxExtent )
{
	// Set the initial values to the 1st vertex.
	minX = maxX = vecPlyVerts[0].x;
	minY = maxY = vecPlyVerts[0].y;
	minZ = maxZ = vecPlyVerts[0].z;

	for ( int index = 0; index != vecPlyVerts.size(); index++ )
	{
		if ( vecPlyVerts[index].x < minX ) { minX = vecPlyVerts[index].x; }
		if ( vecPlyVerts[index].y < minY ) { minY = vecPlyVerts[index].y; }
		if ( vecPlyVerts[index].z < minZ ) { minZ = vecPlyVerts[index].z; }

		if ( vecPlyVerts[index].x > maxX ) { maxX = vecPlyVerts[index].x; }
		if ( vecPlyVerts[index].y > maxY ) { maxY = vecPlyVerts[index].y; }
		if ( vecPlyVerts[index].z > maxZ ) { maxZ = vecPlyVerts[index].z; }
	}

	// What the max extent
	extentX = maxX - minX; 
	extentY = maxY - minY; 
	extentZ = maxZ - minZ;

	maxExtent = extentX;
	if ( extentY > maxExtent ) { maxExtent = extentY; }
	if ( extentZ > maxExtent ) { maxExtent = extentZ; }

	return;
}

// Loads file from disk, AND
// Copies inforamtion into VAO (Vertex Array Object)
bool cMeshTypeManager::LoadPlyFileIntoGLBuffer( unsigned int programID, std::string plyFileNameNoBasePath, 
                                                float vertRed, float vertGreen, float vertBlue, 
                                                bool bMakeFabulousRainbows /*=false*/ )
{
	// ********************************************************************
	// *** START OF: Copying object data into VAO (vertex array object)

	// Here is where the model is loaded into the vertex buffer
	std::vector< cPlyVertexXYZ_N_STU > vecPlyVerts;
	std::vector< cPlyTriFace > vecPlyIndices;

//	if ( ! this->m_loadPlyModel( plyFile, vecPlyVerts, vecPlyIndices ) )
//	{
//		//std::cout << "Can't load model. Sorry it didn't work out." << std::endl;
//		//return -1;
//		return false;
//	}

	// Add the basepath the the filename
	std::string fullFileName = this->m_basepath + plyFileNameNoBasePath;


	CPlyFile5nt plyModel;
	std::wstring wError;
	if ( ! plyModel.OpenPLYFile2( CStringHelper::ASCIIToUnicodeQnD(fullFileName), wError ) )
	{
		return false;
	}

	// Normals? 
	if ( ! plyModel.bHasNormalsInFile() )
	{
		plyModel.normalizeTheModelBaby();
	}

	// normalize the normals (as meshlab tends to make them really long)
	plyModel.normlizeExistingNomrals();

	// If there aren't texture coordinates, generate them
	if ( ! plyModel.bHasTextureCoordinatesInFile() )
	{
		plyModel.GenTextureCoordsSpherical( CPlyFile5nt::POSITIVE_X, 
										    CPlyFile5nt::POSITIVE_Y, 
										    false /*based on normals*/,
										    1.0f  /*scale*/,
										    false );
	}

	{// START OF: SCOPE FOR COPY
		// Copy the vertices from the CPlyFile5nt loader to something we "know"
		// (This step could be removed if we convert everything else to the format inside
		//  CPlyFile5nt, of course)
		int numberOfVertices = plyModel.GetNumberOfVerticies();
		for ( int index = 0; index != numberOfVertices; index++ )
		{
			cPlyVertexXYZ_N_STU tempVert; 
			tempVert.x = plyModel.getVertex_at(index).xyz.x;
			tempVert.y = plyModel.getVertex_at(index).xyz.y;
			tempVert.z = plyModel.getVertex_at(index).xyz.z;

			tempVert.nX = plyModel.getVertex_at(index).nx;
			tempVert.nY = plyModel.getVertex_at(index).ny;
			tempVert.nZ = plyModel.getVertex_at(index).nz;

			tempVert.s = plyModel.getVertex_at(index).tex0u;
			tempVert.t = plyModel.getVertex_at(index).tex0v;
			tempVert.u = 0.0f;

			vecPlyVerts.push_back(tempVert);
		}
	}// ENDOF: SCOPE FOR COPY

	float minX, minY, minZ, maxX, maxY, maxZ, extentX, extentY, extentZ, maxExtent;

	this->m_CalculateExtents( vecPlyVerts, minX, minY, minZ, maxX, maxY, maxZ, extentX, extentY, extentZ, maxExtent );

	// ************************************************************
	// If we are to scale the model on load, then scale it! 
	if ( this->m_bScaleEverythingToBoundingBoxOnLoad )
	{
		// This will scale everything to a set bounding box scale
		float scaleValue = this->m_BoundingBoxScalingSize / maxExtent;

		// Scale based on the extent
		for ( int index = 0; index != vecPlyVerts.size(); index++ )
		{
			vecPlyVerts[index].x *= scaleValue;
			vecPlyVerts[index].y *= scaleValue;
			vecPlyVerts[index].z *= scaleValue;
		}
		// Recalculate the extents (as they've likely changed)
		this->m_CalculateExtents( vecPlyVerts, minX, minY, minZ, maxX, maxY, maxZ, extentX, extentY, extentZ, maxExtent );

	}//if ( this->m_bScaleEverythingToBoundingBoxOnLoad )
	// ************************************************************
	if(plyFileNameNoBasePath == "Homer_x10_0Y_N.ply")
		gPlayerMaxZ = maxZ;
	if (plyFileNameNoBasePath == "Cow_0Y_CenterX_N.ply")
		gEnemyYtype1MaxZ;
	if (plyFileNameNoBasePath == "Bunny_x100_CenterX_0Y_N.ply")
		gEnemyYtype2MaxZ;

	this->m_Normalize( vecPlyVerts );

	// vert_XYZ_RGB for my bunny
	int numberofVerts = vecPlyVerts.size();
//	vert_XYZ_RGB* p_vertArray = new vert_XYZ_RGB[numberofVerts]; 
//	vert_XYZ_RGB_Nxyz* p_vertArray = new vert_XYZ_RGB_Nxyz[numberofVerts]; 
	vert_XYZ_RGB_Nxyz_STU* p_vertArray = new vert_XYZ_RGB_Nxyz_STU[numberofVerts]; 

	for ( int index = 0; index != numberofVerts; index++ )
	{
		p_vertArray[index].x = vecPlyVerts[index].x;
		p_vertArray[index].y = vecPlyVerts[index].y;
		p_vertArray[index].z = vecPlyVerts[index].z;

		p_vertArray[index].r = vertRed;
		p_vertArray[index].g = vertGreen;
		p_vertArray[index].b = vertBlue;

		if ( bMakeFabulousRainbows )
		{
			p_vertArray[index].r = getRandFloat(0.0f, 1.0f);
			p_vertArray[index].g = getRandFloat(0.0f, 1.0f);
			p_vertArray[index].b = getRandFloat(0.0f, 1.0f);
		}

		p_vertArray[index].nX = vecPlyVerts[index].nX;
		p_vertArray[index].nY = vecPlyVerts[index].nY;
		p_vertArray[index].nZ = vecPlyVerts[index].nZ;

		// Added: texture coordinates
		p_vertArray[index].s = vecPlyVerts[index].s;
		p_vertArray[index].t = vecPlyVerts[index].t;
		// There's a potential 3rd texture coordinate, but we aren't loading it now. 
		p_vertArray[index].u = 0.0f;	//vecPlyVerts[index].u;
	}

	cMeshTypeManager::VAOInfo tempMeshVAOInfo;	
	tempMeshVAOInfo.meshName = plyFileNameNoBasePath;
	// Save the unit scale value
	tempMeshVAOInfo.unitScaleValue = 1.0f / maxExtent;  //0.01f

	glGenVertexArrays(1, &(tempMeshVAOInfo.VAO_ID) );	
// Refer to specific vertex array buffer
//	glBindVertexArray( VAO_ID );
	glBindVertexArray( tempMeshVAOInfo.VAO_ID );

	GLuint vertex_buffer = 0;
	GLuint index_buffer = 0;

	// Create a vertex buffer
    glGenBuffers(1, &vertex_buffer);  
	// Brings the particular vertex buffer into context
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

//	int sizeInBytes = sizeof( vert_XYZ_RGB ) * numberofVerts;
//	int sizeInBytes = sizeof( vert_XYZ_RGB_Nxyz ) * numberofVerts;
	int sizeInBytes = sizeof( vert_XYZ_RGB_Nxyz_STU ) * numberofVerts;

    glBufferData(GL_ARRAY_BUFFER, 
				 sizeInBytes, 
				 p_vertArray,			// vertices,				// Pointer to that array
				 GL_STATIC_DRAW);

	// Generate an "index" buffer
    glGenBuffers(1, &index_buffer);  
	// Brings the particular vertex buffer into context
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

	{ // START OF: SCOPE FOR COPY
		// Copy the vertices from the CPlyFile5nt loader to something we "know"
		// (This step could be removed if we convert everything else to the format inside
		//  CPlyFile5nt, of course)
		int numberOfElements = plyModel.GetNumberOfElements();
		for ( int index = 0; index != numberOfElements; index++ )
		{
			cPlyTriFace tempTri;
			tempTri.v1 = plyModel.getElement_at(index).vertex_index_1;
			tempTri.v2 = plyModel.getElement_at(index).vertex_index_2;
			tempTri.v3 = plyModel.getElement_at(index).vertex_index_3;

			vecPlyIndices.push_back( tempTri );
		}
	} // END OF: SCOPE FOR COPY

	// Copy the index information into the index buffer
	// std::vector< cPlyTriFace > vecPlyIndices;
	// 986, 985, 968, and so on...
	int numberOfTriangles = vecPlyIndices.size();
//	int numberOfIndices = numberOfTriangles * 3;
	tempMeshVAOInfo.numberOfIndices = numberOfTriangles * 3;
	int* pIndexArray = new int[tempMeshVAOInfo.numberOfIndices];

	// indexBufferIndex			triAngleVecIndex
	// 0                        0  (v1)
	// 1                        0  (v2)
	// 2                        0  (v3)
	// 3						1  (v1)



	int indexBufferIndex = 0;
	for ( int triIndex = 0; 
		  triIndex != vecPlyIndices.size(); 
		  triIndex++, indexBufferIndex += 3 )
	{
		pIndexArray[indexBufferIndex + 0]  = vecPlyIndices[triIndex].v1;		// Index 0
		pIndexArray[indexBufferIndex + 1]  = vecPlyIndices[triIndex].v2;		// Index 0
		pIndexArray[indexBufferIndex + 2]  = vecPlyIndices[triIndex].v3;		// Index 0
	}

	int indexBufferArraySizeInBytes = tempMeshVAOInfo.numberOfIndices * sizeof(int);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
				 indexBufferArraySizeInBytes, 
				 pIndexArray,			// index array // Pointer to that array
				 GL_STATIC_DRAW);



	// Bind the vertex attributes to the shader
//	int sizeOfVertInBytes = sizeof( vert_XYZ_RGB );
//	int sizeOfVertInBytes = sizeof( vert_XYZ_RGB_Nxyz );
	int sizeOfVertInBytes = sizeof( vert_XYZ_RGB_Nxyz_STU );

	// Telling the shader where the vertex info is...
	// It's "layout"
	// Note that we need the shader program ID to store the connection 
	//	between the vertex buffers and the shader program attributes
    GLuint vpos_location = glGetAttribLocation(programID, "vPosition");	// 12 byes
    GLuint vcol_location = glGetAttribLocation(programID, "vColour");	// 12 bytes, starts at byte 12
    GLuint vnorm_location = glGetAttribLocation(programID, "vNormal");	// 12 bytes, starts at byte 24
    GLuint vstu_location = glGetAttribLocation(programID, "vTexSTU");	// 12 bytes, starts at byte 36

    glEnableVertexAttribArray(vpos_location);
//	int offsetInBytesToPosition = offsetof( vert_XYZ_RGB, r );
//	int offsetInBytesToPosition = offsetof( vert_XYZ_RGB_Nxyz, x );
	int offsetInBytesToPosition = offsetof( vert_XYZ_RGB_Nxyz_STU, x );
    glVertexAttribPointer( vpos_location, 3, 
						  GL_FLOAT, GL_FALSE,
                          sizeOfVertInBytes,	// sizeof(float) * 6,		// 
						  //(void*) 0);
						  (void*) offsetInBytesToPosition);

    glEnableVertexAttribArray(vcol_location);
//	int offsetInBytesToColour = offsetof( vert_XYZ_RGB, r );
//	int offsetInBytesToColour = offsetof( vert_XYZ_RGB_Nxyz, r );
	int offsetInBytesToColour = offsetof( vert_XYZ_RGB_Nxyz_STU, r );
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeOfVertInBytes,	// sizeof(float) * 5, 
						 // (void*) (sizeof(float) * 3));
						  (void*) offsetInBytesToColour);

    glEnableVertexAttribArray(vnorm_location);
//	int offsetInBytesToNormal = offsetof( vert_XYZ_RGB_Nxyz, nX );
 	int offsetInBytesToNormal = offsetof( vert_XYZ_RGB_Nxyz_STU, nX );
    glVertexAttribPointer(vnorm_location, 3, GL_FLOAT, GL_TRUE,
                          sizeOfVertInBytes,	// sizeof(float) * 5, 
						  //(void*) (sizeof(float) * 3));
						  (void*) offsetInBytesToNormal);

    glEnableVertexAttribArray(vstu_location);
//	int offsetInBytesToNormal = offsetof( vert_XYZ_RGB_Nxyz, nX );
 	int offsetInBytesToSTU = offsetof( vert_XYZ_RGB_Nxyz_STU, s );
    glVertexAttribPointer(vstu_location, 3, GL_FLOAT, GL_TRUE,
                          sizeOfVertInBytes,	// sizeof(float) * 5, 
						  //(void*) (sizeof(float) * 3));
						  (void*) offsetInBytesToSTU);


	// Data is copied into the vertex (and index) buffers, so delete array(s)
	delete [] p_vertArray;			// Note slightly strange syntax for array deletion
	delete [] pIndexArray;

	// Save that VAO info for later

	this->m_MapMeshNameToVAOInfo[plyFileNameNoBasePath] = tempMeshVAOInfo;
	// "bunny" --> VAO_


	glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ********************************************************************
	// *** END OF: Copying object data into VAO (vertex array object)
	// ********************************************************************

	return true;
}

bool cMeshTypeManager::LookUpMeshInfo( std::string meshName, 
						unsigned int &VAO_ID,	    // GLuint by ref
						int &numberOfIndices,
					    float &unitScale ) 
{
	// Perform a lookup into the map
	std::map< std::string, cMeshTypeManager::VAOInfo >::iterator itVAO
		= this->m_MapMeshNameToVAOInfo.find( meshName );

	// If the iterator ISN'T set to end(), then it found it
	if ( itVAO == this->m_MapMeshNameToVAOInfo.end() )
	{	// On no! Didn't find it
		return false;
	}

	VAO_ID = itVAO->second.VAO_ID;
	numberOfIndices = itVAO->second.numberOfIndices;
	unitScale = itVAO->second.unitScaleValue;

	return true;
}

bool cMeshTypeManager::m_loadPlyModel( std::string filename_nobasepath, 
				   std::vector< cPlyVertexXYZ_N_STU > &vecPlyVerts, 
				   std::vector< cPlyTriFace > &vecPlyIndices )
{
	// Add the basepath the the filename
	std::string fullFileName = this->m_basepath + filename_nobasepath;

	std::ifstream theFile(fullFileName.c_str());
	if ( ! theFile.is_open() )
	{
		return false;
	}

	// Assume formant is only xyz with no other vertex attribs
	
	// Read until we get to the text "vertex" 
	std::string temp;
	bool bKeepReading = true;
	while ( bKeepReading )
	{	
		theFile >> temp;
		// "vertex"?
		if ( temp == "vertex" )
		{
			bKeepReading = false;
		}
	}
	// Next value is the number of vertices
	int numVertices = 0;
	theFile >> numVertices;

	// Read until we get to the text "face"
	bKeepReading = true;
	while ( bKeepReading )
	{	
		theFile >> temp;
		// "face"?
		if ( temp == "face" )
		{
			bKeepReading = false;
		}
	}
	// Next value is the number of faces (aka "triangles")
	int numberOfTriangles = 0;
	theFile >> numberOfTriangles;

	// Read until we get to "end_header"
	bKeepReading = true;
	while ( bKeepReading )
	{	
		theFile >> temp;
		// "end_header"?
		if ( temp == "end_header" )
		{
			bKeepReading = false;
		}
	}

	// Now read all the vertices
	for ( int index = 0; index != numVertices; index++ )
	{
		cPlyVertexXYZ_N_STU tempVert;
		theFile >> tempVert.x >> tempVert.y >> tempVert.z;
		theFile >> tempVert.nX >> tempVert.nY >> tempVert.nZ;
		// add to the temporary vector
		vecPlyVerts.push_back( tempVert );
	}
	
	// Now the indices...
	for ( int index = 0; index != numberOfTriangles; index++ )
	{
		cPlyTriFace tempTri;
		int numIndicesDiscard = 0;		// 3
		theFile >> numIndicesDiscard >> tempTri.v1 >> tempTri.v2 >> tempTri.v3;
		vecPlyIndices.push_back( tempTri );
	}

	// All done.

	return true;
}