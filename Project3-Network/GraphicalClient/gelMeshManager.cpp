#include "gelMeshManager.h"

#include <fstream>
#include <vector>

#include "externals.h"

#include "cAABB.h"

const float PI = 3.1415926f;

void gelMeshManager::ScaleEverythingToBoundingBoxOnLoad(bool scale, float boxWidth)
{
	this->scaleToBoundingBoxOnLoad = scale;
	this->boundingBoxScalingSize = boxWidth;
}

void divideTriangles(std::vector<GelTriangle>&dividedTriangles)
{
	for (int i = 0; i != dividedTriangles.size(); i++)
	{
		float edge1 = glm::distance(dividedTriangles[i].v1, dividedTriangles[i].v2);
		float edge2 = glm::distance(dividedTriangles[i].v2, dividedTriangles[i].v3);
		float edge3 = glm::distance(dividedTriangles[i].v1, dividedTriangles[i].v3);

		if (edge1 < 15.0f && edge2 < 15.0f && edge3 < 15.0f )
		{
			continue;
		}

		glm::vec3 center = (dividedTriangles[i].v1 + dividedTriangles[i].v2 + dividedTriangles[i].v3) / 3.0f;
		std::vector<GelTriangle> triangleVector;

		GelTriangle newTri1;
		GelTriangle newTri2;
		GelTriangle newTri3;

		newTri1.v1 = dividedTriangles[i].v1;
		newTri1.v2 = center;
		newTri1.v3 = dividedTriangles[i].v2;

		newTri2.v1 = dividedTriangles[i].v1;
		newTri2.v2 = dividedTriangles[i].v3;
		newTri2.v3 = center;

		newTri3.v1 = dividedTriangles[i].v3;
		newTri3.v2 = dividedTriangles[i].v2;
		newTri3.v3 = center;

		dividedTriangles.push_back(newTri1);
		dividedTriangles.push_back(newTri2);
		dividedTriangles.push_back(newTri3);
	}
	

	//if (glm::distance(v1, v2) < 10.0f &&
	//	glm::distance(v2, v3) < 10.0f &&
	//	glm::distance(v1, v3) < 10.0f
	//	)
	//{
	//	
	//}
	//else
	//{
	//	glm::vec3 center = (v1 + v2 + v3) / 3.0f;
	//	/*std::vector<GelTriangle> triangleVector;

	//	//send those new triangles to this function recursively
	//	divideTriangles(v1, center, v2, dividedTriangles);
	//	divideTriangles(v1, v3, center, dividedTriangles);
	//	divideTriangles(v3, v2, center, dividedTriangles);
	//}
}

bool trianglesNeedsDividing(std::vector<GelTriangle>&triangleVector)
{
	for (int i = 0; i != triangleVector.size(); i++)
	{
		if (glm::distance(triangleVector[i].v1, triangleVector[i].v2) >= 10.0f &&
			glm::distance(triangleVector[i].v2, triangleVector[i].v3) >= 10.0f &&
			glm::distance(triangleVector[i].v1, triangleVector[i].v3) >= 10.0f
			)
		{
			return true;
		}
	}
	return false;
}

// Loads file from disk, AND
// Copies inforamtion into VAO (Vertex Array Object)
bool gelMeshManager::LoadPlyIntoGLBuffer(unsigned int programID, std::string alias, std::string plyFile, bool isCollisionModel, int collisionScale)
{
	// Here is where the model is loaded into the vertex buffer
	std::vector< GelVertex > vecPlyVerts;
	std::vector< GelTriFace > vecPlyIndices;
	std::vector< GelTriangle > vecPlyTriangles;

	int vertexCount = 0;
	int faceCount = 0;
	int indexCount = 0;
	int vertexPropertyCount = 0;

	//CoraVertex* vertexArray, int* indexArray, std::string fileName
	if (!this->ReadPlyFileHeaders(plyFile, vertexCount, faceCount, vertexPropertyCount))
	{
		return false;
	}

	indexCount = faceCount * 3;
	GelVertex* vertexArray = new GelVertex[vertexCount];
	int* indexArray = new int[indexCount];
	float radius = 0.0f;

	if (!this->LoadPlyFile(vertexArray, indexArray, alias, plyFile, vertexCount, faceCount, vertexPropertyCount, radius, isCollisionModel, collisionScale))
	{
		//std::cout << "Can't load model. Sorry it didn't work out." << std::endl;
		//return -1;
		return false;
	}

	// Create a vertex array object...	//  "bunny.ply" ---> VAO_ID
	gelMeshManager::VAOInfo tempMeshVAOInfo;
	tempMeshVAOInfo.meshName = plyFile;
	tempMeshVAOInfo.name = alias;
	tempMeshVAOInfo.radius = radius;
	tempMeshVAOInfo.unitScaleValue = 1.0f / (radius*2);

	//	GLuint VAO_ID = 0;
	//	glGenVertexArrays(1, &VAO_ID );
	glGenVertexArrays(1, &(tempMeshVAOInfo.VAO_ID));
	// Refer to specific vertex array buffer
	//	glBindVertexArray( VAO_ID );
	glBindVertexArray(tempMeshVAOInfo.VAO_ID);

	GLuint vertex_buffer = 0;
	GLuint index_buffer = 0;

	int sizeInBytes_1 = sizeof(GelVertex) * vertexCount;
	// Create a vertex buffer
	glGenBuffers(1, &vertex_buffer);
	// Brings the particular vertex buffer into context
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeInBytes_1,
		vertexArray, // vertices,
		GL_STATIC_DRAW);

	tempMeshVAOInfo.numberOfIndices = indexCount;
	int sizeInBytes_2 = indexCount * sizeof(int);
	// Generate an "index" buffer
	glGenBuffers(1, &index_buffer);
	// Brings the particular vertex buffer into context
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeInBytes_2,
		indexArray,			// index array // Pointer to that array
		GL_STATIC_DRAW);

	// Bind the vertex attributes to the shader
	int sizeOfVertInBytes = sizeof(GelVertex);

	// Telling the shader where the vertex info is...
	// It's "layout"
	// Note that we need the shader program ID to store the connection 
	//	between the vertex buffers and the shader program attributes
	GLuint vpos_location = glGetAttribLocation(programID,  "vPosition");// 12
	GLuint vcol_location = glGetAttribLocation(programID,  "vColour");	// 24
	GLuint vnorm_location = glGetAttribLocation(programID, "vNormal");	// 32
	GLuint vstu_location = glGetAttribLocation(programID, "vTexSTU");	// 

	//x y z
	int offsetInBytesToPosition = offsetof(GelVertex, x);
	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 3,
		GL_FLOAT, GL_FALSE,
		sizeOfVertInBytes,	// sizeof(float) * 6,		// 
		(void*) offsetInBytesToPosition);//(void*)0);

	//r g b 
	int offsetInBytesToColour = offsetof(GelVertex, r);
	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
		sizeOfVertInBytes,	// sizeof(float) * 5, 
		(void*)offsetInBytesToColour); // (void*)(sizeof(float) * 3));
	
	//nx ny nz
	int offsetInBytesToNormal = offsetof(GelVertex, nX);
	glEnableVertexAttribArray(vnorm_location);
	glVertexAttribPointer(vnorm_location, 3, GL_FLOAT, GL_FALSE,
		sizeOfVertInBytes,	
		(void*)offsetInBytesToNormal);

	//s t u
	int offsetInBytesToSTU = offsetof(GelVertex, s);
	glEnableVertexAttribArray(vstu_location);
	glVertexAttribPointer(vstu_location, 3, GL_FLOAT, GL_FALSE,
		sizeOfVertInBytes,
		(void*)offsetInBytesToSTU);


	// Save that VAO info for later
	this->meshMap[alias] = tempMeshVAOInfo;

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Data is copied into the vertex (and index) buffers, so delete array(s)
	delete[] vertexArray;
	delete[] indexArray;

	return true;
}

bool gelMeshManager::ReadPlyFileHeaders(std::string fileName, int& vertexCount, int& faceCount, int& propCount)
{
	// Note: "C-style" string conversion
	std::ifstream plyFile(fileName.c_str());
	if (!plyFile.is_open())
	{
		//std::cout << "Can't open file" << std::endl;
		return false;		// non zero = it's bad
	}

	std::string uselessPlyContent;

	//move head to vertex
	do {
		plyFile >> uselessPlyContent;
	} while (uselessPlyContent != "vertex");

	plyFile >> vertexCount;

	do
	{
		if (uselessPlyContent == "property")
		{
			propCount++;
		}
		plyFile >> uselessPlyContent;
	} while (uselessPlyContent != "face");

	plyFile >> faceCount;

	//right now we skip edges and don't check properties extensively(like intensity)

	plyFile.close();
	return true;
}

bool gelMeshManager::LoadPlyFile(GelVertex* vertexArray, int* indexArray, std::string alias, std::string fileName, int vertexCount, int faceCount, int propCount, float& radius, bool isCollisionModel, int collisionScale)
{
	std::ifstream plyFile(fileName.c_str());
	//right now we skip edges and don't check properties extensively(like intensity)
	std::string tmp;
	do //skip until data
	{
		plyFile >> tmp;
	} while (tmp != "end_header");

	//we got the file, now read all until encountering "vertex" and "face",
	//next up get their counts, and jump that many lines to move the head and get property count of vertices 

	//first part

	//also calculate maxXYZ minXYZ and find the largest bounding distance for radius
	float minX = 0.0f;
	float minY = 0.0f; 
	float minZ = 0.0f;

	float maxX = 0.0f;
	float maxY = 0.0f;
	float maxZ = 0.0f;

	bool basedOnNormals = false;
	bool fast = false;
	enumTEXCOORDBIAS uBias = POSITIVE_X;
	enumTEXCOORDBIAS vBias = POSITIVE_Y;
	float scale = 1.0f;
	// determine the 'remaining' coordinate...

	if (propCount == 3) //assumed x,y,z
	{
		bool xUsed = false;
		bool yUsed = false;
		bool zUsed = false;
		if (uBias == POSITIVE_X || vBias == POSITIVE_X)	xUsed = true;
		if (uBias == POSITIVE_Y || vBias == POSITIVE_Y)	yUsed = true;
		if (uBias == POSITIVE_Z || vBias == POSITIVE_Z)	yUsed = true;


		for (int i = 0; i < vertexCount; i++)
		{
			GelVertex* coraVertex = new GelVertex();

			float x, y, z;

			plyFile >> x;
			plyFile >> y;
			plyFile >> z;

			coraVertex->x = x;
			coraVertex->y = y;
			coraVertex->z = z;

			//vecVertices.push_back(coraVertex);

			if (maxX < x) maxX = x;
			if (maxY < y) maxY = y;
			if (maxZ < z) maxZ = z;

			if (minX > x) minX = x;
			if (minY > y) minY = y;
			if (minZ > z) minZ = z;

			//generate texture coord
			glm::vec3 xyz;
			if (uBias == POSITIVE_X)		xyz.x = x;
			else if (uBias == POSITIVE_Y)	xyz.x = y;
			else if (uBias == POSITIVE_Z)	xyz.x = z;

			if (vBias == POSITIVE_X)		xyz.y = x;
			else if (vBias == POSITIVE_Y)	xyz.y = y;
			else if (vBias == POSITIVE_Z)	xyz.y = z;

			// Fill in the remaining coordinate...
			if (!xUsed)	xyz.z = x;
			if (!yUsed)	xyz.z = y;
			if (!zUsed)	xyz.z = z;

			xyz = glm::normalize(xyz);
			coraVertex->s = ((asin(xyz.x) / PI) + 0.5f) * scale;
			coraVertex->t = ((asin(xyz.y) / PI) + 0.5f) * scale;

			vertexArray[i] = *coraVertex;
		}
	}
	else if (propCount == 5) //open the ones with intensity and stuff   //assumed x,y,z, s,t
	{
		for (int i = 0; i < vertexCount; i++)
		{
			GelVertex* coraVertex = new GelVertex();

			float x, y, z;
			float t1, t2;
			plyFile >> x;
			plyFile >> y;
			plyFile >> z;
			plyFile >> t1;
			plyFile >> t2;

			coraVertex->x = x;
			coraVertex->y = y;
			coraVertex->z = z;

			if (maxX < x) maxX = x;
			if (maxY < y) maxY = y;
			if (maxZ < z) maxZ = z;

			if (minX > x) minX = x;
			if (minY > y) minY = y;
			if (minZ > z) minZ = z;

			coraVertex->s = t1;
			coraVertex->t = t1;

			//vecVertices.push_back(coraVertex);
			vertexArray[i] = *coraVertex;
		}
	}
	else if (propCount == 6) //assumed x,y,z,normalXYZ for the sake of simplicity
	{
		bool xUsed = false;
		bool yUsed = false;
		bool zUsed = false;
		if (uBias == POSITIVE_X || vBias == POSITIVE_X)	xUsed = true;
		if (uBias == POSITIVE_Y || vBias == POSITIVE_Y)	yUsed = true;
		if (uBias == POSITIVE_Z || vBias == POSITIVE_Z)	yUsed = true;

		for (int i = 0; i < vertexCount; i++)
		{
			GelVertex* coraVertex = new GelVertex();

			float x, y, z;
			float nX, nY, nZ;

			plyFile >> x;
			plyFile >> y;
			plyFile >> z;
			plyFile >> nX;
			plyFile >> nY;
			plyFile >> nZ;

			coraVertex->x = x;
			coraVertex->y = y;
			coraVertex->z = z;

			glm::vec3 normNormal = glm::normalize(glm::vec3(nX, nY, nZ));

			coraVertex->nX = normNormal.x;
			coraVertex->nY = normNormal.y;
			coraVertex->nZ = normNormal.z;
			coraVertex->s = 0.0f;
			coraVertex->t = 0.0f;

			if (maxX < x) maxX = x;
			if (maxY < y) maxY = y;
			if (maxZ < z) maxZ = z;

			if (minX > x) minX = x;
			if (minY > y) minY = y;
			if (minZ > z) minZ = z;

			//vecVertices.push_back(coraVertex);

			//generate texture coord
			glm::vec3 xyz;
			if (uBias == POSITIVE_X)		xyz.x = x;
			else if (uBias == POSITIVE_Y)	xyz.x = y;
			else if (uBias == POSITIVE_Z)	xyz.x = z;

			if (vBias == POSITIVE_X)		xyz.y = x;
			else if (vBias == POSITIVE_Y)	xyz.y = y;
			else if (vBias == POSITIVE_Z)	xyz.y = z;

			// Fill in the remaining coordinate...
			if (!xUsed)	xyz.z = x;
			if (!yUsed)	xyz.z = y;
			if (!zUsed)	xyz.z = z;

			xyz = glm::normalize(xyz);
			coraVertex->s = ((asin(xyz.x) / PI) + 0.5f) * scale;
			coraVertex->t = ((asin(xyz.y) / PI) + 0.5f) * scale;

			vertexArray[i] = *coraVertex;
		}
	}
	else if (propCount == 8) //assumed x,y,z,normalXYZ, s,t for the sake of simplicity
	{
		for (int i = 0; i < vertexCount; i++)
		{
			GelVertex* coraVertex = new GelVertex();

			float x, y, z;
			float nX, nY, nZ;
			float s, t;

			plyFile >> x;
			plyFile >> y;
			plyFile >> z;
			plyFile >> nX;
			plyFile >> nY;
			plyFile >> nZ;
			plyFile >> s;
			plyFile >> t;

			coraVertex->x = x;
			coraVertex->y = y;
			coraVertex->z = z;
			glm::vec3 normNormal = glm::normalize(glm::vec3(nX, nY, nZ));

			coraVertex->nX = normNormal.x;
			coraVertex->nY = normNormal.y;
			coraVertex->nZ = normNormal.z;
			coraVertex->s = s;
			coraVertex->t = t;

			if (maxX < x) maxX = x;
			if (maxY < y) maxY = y;
			if (maxZ < z) maxZ = z;

			if (minX > x) minX = x;
			if (minY > y) minY = y;
			if (minZ > z) minZ = z;

			//vecVertices.push_back(coraVertex);
			vertexArray[i] = *coraVertex;
		}
	}

	float distX = abs(maxX - minX);
	float distY = abs(maxY - minY);
	float distZ = abs(maxZ - minZ);
	float max2Radius = 0.0f;
	if (distX > distY)
	{
		max2Radius = distX;
	}
	else
	{
		max2Radius = distY;
	}
	if (max2Radius < distZ)
	{
		max2Radius = distZ;
	}
	std::cout << "max 2radius = " << max2Radius << std::endl;
	radius = max2Radius / 2.0f;
	if (scaleToBoundingBoxOnLoad)
	{
		float scaleValue = boundingBoxScalingSize / max2Radius;
		radius *= scaleValue;
	}

	//second part

	//for normals
	std::vector< glm::vec3 > vertexNormals;
	//for physics
	std::vector< GelTriangle > triangleArray;
	triangleArray.reserve(faceCount);

	for (int i = 0; i < faceCount; i++)
	{
		int faceSize;
		plyFile >> faceSize;

		if (faceSize == 3) //triangle     //ignore rest of the faces for now
		{
			int i1;
			int i2;
			int i3;

			plyFile >> i1;
			plyFile >> i2;
			plyFile >> i3;

			GelTriFace* triFace = new GelTriFace();
			triFace->i1 = i1;
			triFace->i2 = i2;
			triFace->i3 = i3;

			//vecTriFaces.push_back(triFace);
			indexArray[(i * 3) + 0] = triFace->i1;
			indexArray[(i * 3) + 1] = triFace->i2;
			indexArray[(i * 3) + 2] = triFace->i3;

			////write vertices according to the face index order
			GelVertex vertex_1 = vertexArray[triFace->i1];
			GelVertex vertex_2 = vertexArray[triFace->i2];
			GelVertex vertex_3 = vertexArray[triFace->i3];

			//set the triangle
			GelTriangle triangle;

			//push the triangle into the triangle array
			triangleArray.push_back(triangle);
			triangleArray[i].v1.x = vertex_1.x;
			triangleArray[i].v1.y = vertex_1.y;
			triangleArray[i].v1.z = vertex_1.z;

			triangleArray[i].v2.x = vertex_2.x;
			triangleArray[i].v2.y = vertex_2.y;
			triangleArray[i].v2.z = vertex_2.z;

			triangleArray[i].v3.x = vertex_3.x;
			triangleArray[i].v3.y = vertex_3.y;
			triangleArray[i].v3.z = vertex_3.z;

			glm::vec3 normal_1;
			glm::vec3 normal_2;
			glm::vec3 normal_3;

			//set the normals and push into their respective arrays
			if (propCount == 6)
			{
				normal_1 = glm::normalize(glm::vec3(vertexArray[triFace->i1].nX, vertexArray[triFace->i1].nY, vertexArray[triFace->i1].nZ));
				normal_2 = glm::normalize(glm::vec3(vertexArray[triFace->i2].nX, vertexArray[triFace->i2].nY, vertexArray[triFace->i2].nZ));
				normal_3 = glm::normalize(glm::vec3(vertexArray[triFace->i3].nX, vertexArray[triFace->i3].nY, vertexArray[triFace->i3].nZ));
			}
			else
			{
				//normal on first vertex
				glm::vec3 n1_1(
					vertex_3.x - vertex_1.x,
					vertex_3.y - vertex_1.y,
					vertex_3.z - vertex_1.z);
				glm::vec3 n1_2(
					vertex_2.x - vertex_1.x,
					vertex_2.y - vertex_1.y,
					vertex_2.z - vertex_1.z);

				//normal on second vertex
				glm::vec3 n2_1(
					vertex_1.x - vertex_2.x,
					vertex_1.y - vertex_2.y,
					vertex_1.z - vertex_2.z);
				glm::vec3 n2_2(
					vertex_3.x - vertex_2.x,
					vertex_3.y - vertex_2.y,
					vertex_3.z - vertex_2.z);

				//normal on second vertex
				glm::vec3 n3_1(
					vertex_2.x - vertex_3.x,
					vertex_2.y - vertex_3.y,
					vertex_2.z - vertex_3.z);
				glm::vec3 n3_2(
					vertex_1.x - vertex_3.x,
					vertex_1.y - vertex_3.y,
					vertex_1.z - vertex_3.z);

				//get cross products for normals
				normal_1 = glm::normalize(glm::cross(n1_1, n1_2));
				normal_2 = glm::normalize(glm::cross(n2_1, n2_2));
				normal_3 = glm::normalize(glm::cross(n3_1, n3_2));

			}

			/*if (alias == "ground2")
			{
				normal_1.x = 0.0f;
				normal_1.y = -1.0f;
				normal_1.z = 0.0f;
				normal_2.x = 0.0f;
				normal_2.y = -1.0f;
				normal_2.z = 0.0f;
				normal_3.x = 0.0f;
				normal_3.y = -1.0f;
				normal_3.z = 0.0f;
			}*/

			vertexArray[triFace->i1].nX = -normal_1.x;
			vertexArray[triFace->i1].nY = -normal_1.y;
			vertexArray[triFace->i1].nZ = -normal_1.z;

			vertexArray[triFace->i2].nX = -normal_2.x;
			vertexArray[triFace->i2].nY = -normal_2.y;
			vertexArray[triFace->i2].nZ = -normal_2.z;

			vertexArray[triFace->i3].nX = -normal_3.x;
			vertexArray[triFace->i3].nY = -normal_3.y;
			vertexArray[triFace->i3].nZ = -normal_3.z;
			
			//question this later...
			/*vertexArray[triFace->i1].nX = (vertexArray[triFace->i1].nX + normal_1.x) * 0.5f;
			vertexArray[triFace->i1].nY = (vertexArray[triFace->i1].nY + normal_1.y) * 0.5f;
			vertexArray[triFace->i1].nZ = (vertexArray[triFace->i1].nZ + normal_1.z) * 0.5f;

			vertexArray[triFace->i2].nX = (vertexArray[triFace->i2].nX + normal_2.x) * 0.5f;
			vertexArray[triFace->i2].nY = (vertexArray[triFace->i2].nY + normal_2.y) * 0.5f;
			vertexArray[triFace->i2].nZ = (vertexArray[triFace->i2].nZ + normal_2.z) * 0.5f;

			vertexArray[triFace->i3].nX = (vertexArray[triFace->i3].nX + normal_3.x) * 0.5f;
			vertexArray[triFace->i3].nY = (vertexArray[triFace->i3].nY + normal_3.y) * 0.5f;
			vertexArray[triFace->i3].nZ = (vertexArray[triFace->i3].nZ + normal_3.z) * 0.5f;*/


			////fill the vertexArray with 3 vertices, unindexed, lots of dupes

			//// if i = 0 then,
			//// fill [0] [1] [2]

			//// if i = 1 then,
			//// fill [3] [4] [5]

			//// useful indexes will be [0],[3],[6],[9],[12]...

			//vertexArray[(i * 3) + 0].x = vertex_1->x;
			//vertexArray[(i * 3) + 0].y = vertex_1->y;
			//vertexArray[(i * 3) + 0].z = vertex_1->z;

			//vertexArray[(i * 3) + 1].x = vertex_2->x;
			//vertexArray[(i * 3) + 1].y = vertex_2->y;
			//vertexArray[(i * 3) + 1].z = vertex_2->z;

			//vertexArray[(i * 3) + 2].x = vertex_3->x;
			//vertexArray[(i * 3) + 2].y = vertex_3->y;
			//vertexArray[(i * 3) + 2].z = vertex_3->z;
		}
		else if (faceSize == 4) //quad //ignored
		{
			//TODO: also fill an unindexed vertex array w.r.t. Quad faces
			int i1;
			int i2;
			int i3;
			int i4;

			plyFile >> i1;
			plyFile >> i2;
			plyFile >> i3;
			plyFile >> i4;

			GelQuadFace* quadFace = new GelQuadFace();
			quadFace->i1 = i1;
			quadFace->i2 = i2;
			quadFace->i3 = i3;
			quadFace->i4 = i4;

			//vecQuadFaces.push_back(quadFace);
		}
		else //cool shape //also ignored
		{
			//TODO: also fill an N-sized vertex array w.r.t. N-sized faces???
			//exhaust file
			/*for (int k = 0; k < faceSize; k++)
			{
				plyFile >> faceSize;
			}*/

			//TODO:may try to exhaust with getLine, dunno which is faster
		}
	}

	/*minX = 0.0f;
	minY = 0.0f;
	minZ = 0.0f;

	maxX = 0.0f;
	maxY = 0.0f;
	maxZ = 0.0f;
*/

	float xmaxnew = 0.0f;
	float ymaxnew = 0.0f;
	float zmaxnew = 0.0f;
	float xminnew = 0.0f;
	float yminnew = 0.0f;
	float zminnew = 0.0f;

	if (scaleToBoundingBoxOnLoad)
	{
		// This will scale everything to a set bounding box scale
		float scaleValue = this->boundingBoxScalingSize / (max2Radius);

		for (int i = 0; i < vertexCount; i++)
		{
			//point test for collision 
			float test_x = vertexArray[i].x * (collisionScale / boundingBoxScalingSize) * scaleValue;
			float test_y = vertexArray[i].y * (collisionScale / boundingBoxScalingSize) * scaleValue;
			float test_z = vertexArray[i].z * (collisionScale / boundingBoxScalingSize) * scaleValue;

			vertexArray[i].x  *= scaleValue ;
			vertexArray[i].y  *= scaleValue ;
			vertexArray[i].z  *= scaleValue ;
			//vertexArray[i].nX *= -scaleValue;
			//vertexArray[i].nY *= -scaleValue;
			//vertexArray[i].nZ *= -scaleValue;

			//new max min
			if (xmaxnew < test_x) xmaxnew = test_x;
			if (ymaxnew < test_y) ymaxnew = test_y;
			if (zmaxnew < test_z) zmaxnew = test_z;

			if (xminnew > test_x) xminnew = test_x;
			if (yminnew > test_y) yminnew = test_y;
			if (zminnew > test_z) zminnew = test_z;
		}

		glm::vec3 minPoint;
		minPoint.x = xminnew;
		minPoint.y = yminnew;
		minPoint.z = zminnew;

		distX = abs(xmaxnew - xminnew);
		distY = abs(ymaxnew - yminnew);
		distZ = abs(zmaxnew - zminnew);
		float new2r = 0.0f;
		if (distX > distY)
		{
			new2r = distX;
		}
		else
		{
			new2r = distY;
		}
		if (new2r < distZ)
		{
			new2r = distZ;
		}
		new2r /= 2;

		temp_collision_triangles.clear();

		cAABB bbox(minPoint, boundingBoxScalingSize);
		std::map<int, std::vector<GelTriangle> > collisionMap;
		for (int i = 0; i < faceCount; i++)
		{
			if (isCollisionModel)
			{
				glm::vec3 v1;
				v1.x = triangleArray[i].v1.x * (collisionScale / boundingBoxScalingSize) * scaleValue;
				v1.y = triangleArray[i].v1.y * (collisionScale / boundingBoxScalingSize) * scaleValue;
				v1.z = triangleArray[i].v1.z * (collisionScale / boundingBoxScalingSize) * scaleValue;
				//std::cout << bbox.CalculateID(v1) << std::endl;

				glm::vec3 v2;
				v2.x = triangleArray[i].v2.x * (collisionScale / boundingBoxScalingSize) * scaleValue;
				v2.y = triangleArray[i].v2.y * (collisionScale / boundingBoxScalingSize) * scaleValue;
				v2.z = triangleArray[i].v2.z * (collisionScale / boundingBoxScalingSize) * scaleValue;

				glm::vec3 v3;
				v3.x = triangleArray[i].v3.x * (collisionScale / boundingBoxScalingSize) * scaleValue;
				v3.y = triangleArray[i].v3.y * (collisionScale / boundingBoxScalingSize) * scaleValue;
				v3.z = triangleArray[i].v3.z * (collisionScale / boundingBoxScalingSize) * scaleValue;

				GelTriangle scaledTri;
				scaledTri.v1 = v1;
				scaledTri.v2 = v2; 
				scaledTri.v3 = v3;

				int aabbID  = bbox.CalculateID_Local(v1, new2r);
				int aabbID2 = bbox.CalculateID_Local(v2, new2r);
				int aabbID3 = bbox.CalculateID_Local(v3, new2r);

				this->temp_collision_triangles[aabbID].push_back(scaledTri);
				if (aabbID != aabbID2)
				{
					this->temp_collision_triangles[aabbID2].push_back(scaledTri);
				}
				if (aabbID != aabbID3)
				{
					this->temp_collision_triangles[aabbID3].push_back(scaledTri);
				}
				//this allowes some ids to have duplicate triangles but with much faster loading time.

				//this destroys memory

				//if (glm::distance(v1, v2) >= boundingBoxScalingSize &&
				//	glm::distance(v2, v3) >= boundingBoxScalingSize &&
				//	glm::distance(v1, v3) >= boundingBoxScalingSize
				//	)
				//{
				//	//divide triangle into smaller ones
				//	std::vector<GelTriangle> dividedTriangles;
				//	GelTriangle firstTriangle;
				//	firstTriangle.v1 = v1;
				//	firstTriangle.v2 = v2;
				//	firstTriangle.v3 = v3;
				//	dividedTriangles.push_back(firstTriangle);

				//	while (trianglesNeedsDividing(dividedTriangles))
				//	{
				//		divideTriangles(dividedTriangles);
				//		int x = 0; //test bp
				//	}
				//	int x = 0;
				//	
				//}

				if (isCollisionModel)
				{
					
				}
				

				////this block gives me severe discomfort
				//if (aabbID != aabbID2)
				//{
				//	if (aabbID != aabbID3)
				//	{
				//		//all different
				//		std::vector<GelTriangle> vec1 = this->battlestar_collision_triangles[aabbID];
				//		std::vector<GelTriangle> vec2 = this->battlestar_collision_triangles[aabbID2];
				//		std::vector<GelTriangle> vec3 = this->battlestar_collision_triangles[aabbID3];
				//		vec1.push_back(triangleArray[i]);
				//		vec2.push_back(triangleArray[i]);
				//		vec3.push_back(triangleArray[i]);
				//		this->battlestar_collision_triangles[aabbID] = vec1;
				//		this->battlestar_collision_triangles[aabbID2] = vec2;
				//		this->battlestar_collision_triangles[aabbID3] = vec3;
				//	}
				//	else if (aabbID2 == aabbID3)
				//	{
				//		//1 and 2-3 different, 2 and 3 same
				//		std::vector<GelTriangle> vec1 = this->battlestar_collision_triangles[aabbID];
				//		std::vector<GelTriangle> vec2 = this->battlestar_collision_triangles[aabbID2];
				//		vec1.push_back(triangleArray[i]);
				//		vec2.push_back(triangleArray[i]);
				//		this->battlestar_collision_triangles[aabbID] = vec1;
				//		this->battlestar_collision_triangles[aabbID2] = vec2;
				//	}
				//	else
				//	{
				//		//1-3 and 2 different, 1 and 3 same
				//		std::vector<GelTriangle> vec1 = this->battlestar_collision_triangles[aabbID];
				//		std::vector<GelTriangle> vec2 = this->battlestar_collision_triangles[aabbID2];
				//		vec1.push_back(triangleArray[i]);
				//		vec2.push_back(triangleArray[i]);
				//		this->battlestar_collision_triangles[aabbID] = vec1;
				//		this->battlestar_collision_triangles[aabbID2] = vec2;
				//	}
				//}
				//else
				//{
				//	if (aabbID == aabbID3)
				//	{
				//		//all same ID
				//		std::vector<GelTriangle> vec1 = this->battlestar_collision_triangles[aabbID];
				//		vec1.push_back(triangleArray[i]);
				//		this->battlestar_collision_triangles[aabbID] = vec1;
				//	}
				//	else
				//	{
				//		//1 and 2 same, 1-2 and 3 different;
				//		std::vector<GelTriangle> vec1 = this->battlestar_collision_triangles[aabbID];
				//		std::vector<GelTriangle> vec3 = this->battlestar_collision_triangles[aabbID3];
				//		vec1.push_back(triangleArray[i]);
				//		vec3.push_back(triangleArray[i]);
				//		this->battlestar_collision_triangles[aabbID] = vec1;
				//		this->battlestar_collision_triangles[aabbID3] = vec3;
				//	}
				//}

				/*if (collisionMap[bbox.CalculateID(v1)].size() > 0)
				{
					.push_back(triangleArray[i].v1);
				}*/
			}

			triangleArray[i].v1.x *= scaleValue;
			triangleArray[i].v1.y *= scaleValue;
			triangleArray[i].v1.z *= scaleValue;

			triangleArray[i].v2.x *= scaleValue;
			triangleArray[i].v2.y *= scaleValue;
			triangleArray[i].v2.z *= scaleValue;

			triangleArray[i].v3.x *= scaleValue;
			triangleArray[i].v3.y *= scaleValue;
			triangleArray[i].v3.z *= scaleValue;
		}

		//test breakpoint for specific model
		if (isCollisionModel)
		{
			CollisionTriangleInfo colTriInfo;
			colTriInfo.collisionModelScale = collisionScale;
			colTriInfo.alias = alias;
			colTriInfo.aabbTriangleMap = temp_collision_triangles;
			colTriInfo.aabbHalfExtend = new2r;
			colTriInfo.minPoint = minPoint;
			std::cout << "Battlestar_Collision: max min, x y z" << xmaxnew << " " << ymaxnew << " " << zmaxnew << " " << xminnew << " " << yminnew << " " << zminnew << std::endl;
			//temp_collision_triangles;
			this->collisionMap[alias] = colTriInfo;
		}
		else if (alias == "Battlestar")
		{
			std::cout << "Battlestar: max min, x y z" << xmaxnew << " " << ymaxnew << " " << zmaxnew << " " << xminnew << " " << yminnew<< " " << zminnew<< std::endl;
		}
	}


	//write triangle data into the dictionary
	if (isCollisionModel)
	{
		this->triangleMap_CollisionModels[alias] = triangleArray;
	}
	else
	{
		this->triangleMap[alias] = triangleArray;
	}

	plyFile.close();
	return true;
}


bool gelMeshManager::LookUpMeshInfo(std::string name, unsigned int &VAO_ID, int &numberOfIndices, float &unitScale) //unsigned int is GLuint here.
{
	// Perform a lookup into the map
	std::map< std::string, gelMeshManager::VAOInfo >::iterator itVAO
		= this->meshMap.find(name);

	// If the iterator ISN'T set to end(), then it found it
	if (itVAO == this->meshMap.end())
	{	// On no! Didn't find it
		return false;
	}

	VAO_ID = itVAO->second.VAO_ID;
	numberOfIndices = itVAO->second.numberOfIndices;

	if (scaleToBoundingBoxOnLoad)
	{
		unitScale = itVAO->second.unitScaleValue;
	}
	else
	{
		unitScale = 1.0f;
	}
	

	return true;
}

float gelMeshManager::LookUpMeshRadius(std::string name)
{
	// Perform a lookup into the map
	std::map< std::string, gelMeshManager::VAOInfo >::iterator itVAO
		= this->meshMap.find(name);

	// If the iterator ISN'T set to end(), then it found it
	if (itVAO == this->meshMap.end())
	{	// On no! Didn't find it
		return 0.0f;
	}

	return itVAO->second.radius;
}

bool gelMeshManager::GetCollisionModelTriangleInfoForMesh(std::string meshName, float & halfExtend, glm::vec3 & minPoint)
{
	//search
	std::map< std::string, CollisionTriangleInfo >::iterator mapIterator =
		this->collisionMap.find(meshName);

	if (mapIterator == this->collisionMap.end())
	{
		//not found
		return false;
	}

	//found
	CollisionTriangleInfo triInfo = mapIterator->second;

	halfExtend = triInfo.aabbHalfExtend;
	minPoint = triInfo.minPoint;

	return true;
}

bool gelMeshManager::GetCollisionModelTriangleArrayForMesh(std::string meshName, int AABBid, std::vector<GelTriangle>& vecTriangle)
{
	//search
	std::map< std::string, CollisionTriangleInfo >::iterator mapIterator =
		this->collisionMap.find(meshName);

	if (mapIterator == this->collisionMap.end())
	{
		//not found
		return false;
	}

	//found

	//try this 
	CollisionTriangleInfo triInfo = mapIterator->second;
	vecTriangle = triInfo.aabbTriangleMap[AABBid];
	return true;


	std::map< int, std::vector<GelTriangle> >::iterator mapIterator2 =
		triInfo.aabbTriangleMap.find(AABBid);

	if (mapIterator2 == triInfo.aabbTriangleMap.end())
	{
		//not found
		return false;
	}

	vecTriangle = mapIterator2->second;

	return true;
}

bool gelMeshManager::GetCollisionModelTriangleMapForMesh(std::string meshName, std::map<int, std::vector<GelTriangle> > &mapTriangle)
{
	std::map< std::string, CollisionTriangleInfo >::iterator mapIterator =
		this->collisionMap.find(meshName);

	if (mapIterator == this->collisionMap.end())
	{
		//not found
		return false;
	}

	//found
	CollisionTriangleInfo triInfo = mapIterator->second;

	mapTriangle = triInfo.aabbTriangleMap;

	return true;
}

//use this with alias as meshName instead of filePath, refer to gameFile for the alias, i.e. "sphere"
bool gelMeshManager::GetTriangleArrayForMesh(std::string meshName, std::vector<GelTriangle>& vecTriangle)
{
	//search
	std::map< std::string, std::vector< GelTriangle > >::iterator mapIterator =
		this->triangleMap.find(meshName);

	if (mapIterator == this->triangleMap.end())
	{
		//not found
		return false;
	}

	//found
	vecTriangle = mapIterator->second;

	return true;
}

void gelMeshManager::Update(float deltaTime)
{
}

void gelMeshManager::Setup()
{
	this->scaleToBoundingBoxOnLoad = false;
	this->boundingBoxScalingSize = 1.0f;
}
