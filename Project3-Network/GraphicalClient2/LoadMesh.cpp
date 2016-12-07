#include "LoadMesh.h"

// assimp is isolated here
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>


bool LoadMesh(	const char * path,	std::vector<unsigned int> & indices,	std::vector<sMeshVertex>& vertices,	sMeshEntry& entryOut	)
{
	entryOut.BaseIndex = 0;
	entryOut.BaseVertex = 0;
	entryOut.NumIndices = 0;

	Assimp::Importer importer;
	unsigned int flags = 0;
	//flags |= aiProcess_SortByPType;
	//flags |= aiProcess_LimitBoneWeights;
	//flags |= aiProcess_CalcTangentSpace;
	//flags |= aiProcess_JoinIdenticalVertices;
	//flags |= aiProcess_GenNormals;
	//flags |= aiProcess_GenUVCoords;
	//flags |= aiProcess_TransformUVCoords;
	flags |= aiProcess_GenSmoothNormals;
	//flags |= aiProcess_Triangulate;
	//flags |= aiProcess_FindDegenerates;
	//flags |= aiProcess_FindInvalidData;
	const aiScene* scene = importer.ReadFile(path, flags);
	if (!scene) {
		fprintf(stderr, importer.GetErrorString());
		getchar();
		return false;
	}
	const aiMesh* mesh = scene->mMeshes[0]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)

	unsigned int vertexOffset = vertices.size();
	unsigned int indexOffset = indices.size();
	unsigned int numVertices = mesh->mNumVertices;
	unsigned int numIndices = 3 * mesh->mNumFaces; // assume triangles

	entryOut.BaseIndex = indexOffset;
	entryOut.BaseVertex = vertexOffset;
	entryOut.NumIndices = numIndices;
	
	// Fill vertices positions
	vertices.resize(vertexOffset + numVertices);
	for (unsigned int i = 0; i<mesh->mNumVertices; i++)
	{
		aiVector3D pos = mesh->mVertices[i];
		//aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
		aiVector3D n = mesh->mNormals[i];
		
		sMeshVertex& vert = vertices[vertexOffset + i];
		Set(vert.Position, pos.x, pos.y, pos.z, 1.f);
		Set(vert.TexCoord, 0.f, 0.f, 0.f, 0.f);
		//Set(vert.TexCoord, UVW.x, UVW.y, UVW.x, UVW.y);
		Set(vert.Normal, n.x, n.y, n.z, 1.f);
		//std::cout << n.x << "," << n.y << "," << n.z << std::endl;
	}


	// Fill face indices
	indices.resize(indexOffset + numIndices);
	for (unsigned int i = 0; i<mesh->mNumFaces; i++)
	{
		unsigned int baseInd = 3 * i;
		// Assume the model has only triangles.
		indices[indexOffset + baseInd] = mesh->mFaces[i].mIndices[0];
		indices[indexOffset + baseInd + 1] = mesh->mFaces[i].mIndices[1];
		indices[indexOffset + baseInd + 2] = mesh->mFaces[i].mIndices[2];
	}
	/*
	if (!mesh->mNormals)
	{
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			sMeshVertex& vert = vertices[vertexOffset + i];
			Set(vert.Normal, 0.f, 0.f, 0.f, 0.f);
		}
		for (unsigned int i = 0; i<mesh->mNumFaces; i++)
		{
			unsigned int baseInd = 3 * i;
			
			sMeshVertex& vA = vertices[indices[indexOffset + baseInd]];
			sMeshVertex& vB = vertices[indices[indexOffset + baseInd + 1]];
			sMeshVertex& vC = vertices[indices[indexOffset + baseInd + 2]];

			glm::vec4 ab = vB.Position - vA.Position;
			glm::vec4 ac = vC.Position - vA.Position;
			glm::vec3 n = glm::cross(glm::vec3(ac.x, ac.y, ac.z), glm::vec3(ab.x, ab.y, ab.z));
			vA.Normal.x += n.x; vA.Normal.y += n.y; vA.Normal.z += n.z; vA.Normal.w += 1.f;
			vB.Normal.x += n.x; vB.Normal.y += n.y; vB.Normal.z += n.z; vB.Normal.w += 1.f;
			vC.Normal.x += n.x; vC.Normal.y += n.y; vC.Normal.z += n.z; vC.Normal.w += 1.f;
		}
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			sMeshVertex& vert = vertices[vertexOffset + i];
			if (vert.Normal.w != 0.f)
			{
				vert.Normal.x /= vert.Normal.w;
				vert.Normal.y /= vert.Normal.w;
				vert.Normal.z /= vert.Normal.w;
				vert.Normal.w = 1.f;
				glm::vec3 n(vert.Normal.x, vert.Normal.y, vert.Normal.z);
				n = glm::normalize(n);
				vert.Normal.x = n.x;
				vert.Normal.y = n.y;
				vert.Normal.z = n.z;
			}
		}
	}
	*/
	// The "scene" pointer will be deleted automatically by "importer"
}