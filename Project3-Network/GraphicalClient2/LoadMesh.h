#pragma once

#include "externals.h"
#include <vector>



//glDrawElementsBaseVertex

struct sMeshEntry
{
	int NumIndices;
	int BaseIndex;
	int BaseVertex;
};

struct sMeshVertex
{
	glm::vec4 Position;
	glm::vec4 TexCoord;
	glm::vec4 Normal;
};

bool LoadMesh(
	const char * path,
	std::vector<unsigned int> & indices,
	std::vector<sMeshVertex>& vertices,
	sMeshEntry& entryOut
	);