#pragma once
#include "GlobalStuff.h"
#include <vector>
// connection between 2 nodes
class cPathConnection;
// part of a 2d grid used for navigation and path following
class cPathNode
{
public:
	float Radius;  // used to determined if an entity is on this node
	glm::vec3 Position;
	//cColour4 Color;
	
	bool bWalkable;
	float TravelCostFactor;
	std::vector<cPathConnection*> Connections;


	cPathNode();
	~cPathNode();
};

class cPathConnection
{
public:
	float Distance;
	float DistanceSquared;
	cPathNode* NodeA;
	cPathNode* NodeB;

	cPathConnection(cPathNode* nodeA, cPathNode* nodeB);
	~cPathConnection();

	cPathNode* GetOther(cPathNode* node);
};

class cPathNodeGrid
{
public:
	struct sAStarHelper
	{
		int Row;
		int Col;
		cPathNode* Node;
		sAStarHelper* Parent;
		float gScore;
		float fScore;
	};
	cPathNodeGrid(/*float radius,*/ int nodesA, int nodesB, const glm::vec3& init, const glm::vec3& edgeA, const glm::vec3& edgeB);
	~cPathNodeGrid();
	void Clear();

	std::vector<cPathConnection*> Connections;
	std::vector<std::vector<cPathNode*>> Nodes;

	int Rows();
	int Cols();

	cPathNode* GetNodeAt(int row, int col);

	float ManhattanDist(cPathNode* nodeA, cPathNode* nodeB);
	bool AStar(int rowSrc, int colSrc, int rowDst, int colDst, std::vector<cPathNode*>& path);

};