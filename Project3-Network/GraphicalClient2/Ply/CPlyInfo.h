#ifndef _CPlyInfo_HG_
#define _CPlyInfo_HG_

#include <string>
//#include <GL\glew.h>			// Because it complains when we don't include it
//#include <GL\freeglut.h>		// Because of the GLuint

class CPlyInfo
{
public:
	CPlyInfo() : numberOfVertices(0), numberOfElements(0),			// "initializer" list
		         vertexBufferStartIndex(0), indexBufferStartIndex(0), extent(0.0f), 
				 vertexBufferObjectID(0), vertexAttribBufferID(0), 
				 indexBufferID(0) {};
	~CPlyInfo() {};				// empty d'stor
	int numberOfVertices;
	int numberOfElements;		// aka "Triangles"
	int vertexBufferStartIndex;
	int indexBufferStartIndex;
	float extent;				// aka "maxExtent"
	std::wstring plyFileName;

	std::string plyFileASCII;		
	
	unsigned int vertexBufferObjectID;	//(GLuint)	// vertexPositionBufferID;
	unsigned int vertexAttribBufferID;	//(GLuint)	// vertexColourBufferID;
	unsigned int indexBufferID;			//(GLuint)
};

#endif
