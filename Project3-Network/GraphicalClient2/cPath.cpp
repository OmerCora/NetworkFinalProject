#include "cPath.h"
#include <iostream>

cPathNode::cPathNode() : TravelCostFactor(1.f)
{

}

cPathNode::~cPathNode()
{
	Connections.clear();
}

cPathConnection::cPathConnection(cPathNode* nodeA, cPathNode* nodeB)
	: NodeA(nodeA), NodeB(nodeB)
{
	DistanceSquared = glm::length(NodeA->Position - NodeB->Position);
	Distance = sqrt(DistanceSquared);
}

cPathConnection::~cPathConnection()
{
	
}

cPathNode* cPathConnection::GetOther(cPathNode* node)
{
	return (node == NodeA) ? NodeB : NodeA;
}
										    //num of rows  num of columns  //initial corner point
cPathNodeGrid::cPathNodeGrid(/*float radius,*/ int nodesA, int nodesB, const glm::vec3& init, const glm::vec3& edgeA, const glm::vec3& edgeB)
{
	Clear();
	int rows = nodesA;
	int cols = nodesB;
	glm::vec3 sepA = edgeA / (float)(nodesA - 1);
	glm::vec3 sepB = edgeB / (float)(nodesB - 1);
	for (int idxRow = 0; idxRow < rows; idxRow++)
	{
		glm::vec3 posA = init + sepA * float(idxRow);
		std::vector<cPathNode*> row;
		for (int idxCol = 0; idxCol < cols; idxCol++)
		{
			cPathNode* node = new cPathNode();
			//node->Color = nColour::GRAY;
			//node->Color.a = 0.4f;
			//node->Radius = radius;
			node->Position = posA + sepB * float(idxCol);
			row.push_back(node);
		}
		Nodes.push_back(row);
	}
	for (int idxRow = 0; idxRow < rows - 1; idxRow++)
	{
		for (int idxCol = 0; idxCol < cols - 1; idxCol++)
		{
			// grab dem nodes
			cPathNode* node = Nodes[idxRow][idxCol];				// node
			cPathNode* neighbourEast = Nodes[idxRow][idxCol + 1];	// east (right) neighbour
			cPathNode* neighbourSouth = Nodes[idxRow + 1][idxCol];	// south (down) neighbour
			// make the connections
			cPathConnection* connectEW = new cPathConnection(node, neighbourEast);
			cPathConnection* connectNS = new cPathConnection(node, neighbourSouth);
			// add the connections
			Connections.push_back(connectEW);
			Connections.push_back(connectNS);
			// let the nodes know about their connections
			node->Connections.push_back(connectEW);
			neighbourEast->Connections.push_back(connectEW);
			node->Connections.push_back(connectNS);
			neighbourSouth->Connections.push_back(connectNS);
		}
	}
}



cPathNodeGrid::~cPathNodeGrid()
{
	Clear();
}

void cPathNodeGrid::Clear()
{
	std::vector<std::vector<cPathNode*>>::iterator itRow = Nodes.begin();
	while (itRow != Nodes.end())
	{
		std::vector<cPathNode*>::iterator itNode = itRow->begin();
		while (itNode != itRow->end())
		{
			delete (*itNode);
			itNode++;
		}
		itRow->clear();
		itRow++;
	}
	Nodes.clear();
	std::vector<cPathConnection*>::iterator itConnection = Connections.begin();
	while (itConnection != Connections.end())
	{
		delete (*itConnection);
		itConnection++;
	}
	Connections.clear();
}

int cPathNodeGrid::Rows()
{
	return Nodes.size();
}

int cPathNodeGrid::Cols()
{
	return Nodes[0].size();
}

cPathNode* cPathNodeGrid::GetNodeAt(int row, int col)
{
	return Nodes[row][col];
}



float cPathNodeGrid::ManhattanDist(cPathNode* nodeA, cPathNode* nodeB)
{
	glm::vec3 sep = nodeA->Position - nodeB->Position;
	return abs(sep.x) + abs(sep.y) + abs(sep.z);
}
// give this source row and column, and a destination row and column
// it fills out a vector of path nodes from that source to that destination
// bool retun value is for success/failure
bool cPathNodeGrid::AStar(int rowSrc, int colSrc, int rowDst, int colDst, std::vector<cPathNode*>& path)
{
	path.resize(0);

	std::vector<std::vector<sAStarHelper*>> helpers;
	
	unsigned int numRows = Rows();
	unsigned int numCols = Cols();
	// just initializing the helpers
	for (unsigned int idxRow = 0; idxRow < numRows; idxRow++)
	{
		helpers.push_back(std::vector<sAStarHelper*>());
		for (unsigned int idxCol = 0; idxCol < numCols; idxCol++)
		{
			sAStarHelper* helper = new sAStarHelper();
			helper->Row = idxRow;
			helper->Col = idxCol;
			helper->Node = Nodes[idxRow][idxCol];
			helper->Parent = 0;
			helper->fScore = FLT_MAX;
			helper->gScore = FLT_MAX;
			helpers[idxRow].push_back(helper);
		}
	}
	// get the actual nodes for the start and end
	cPathNode* startNode = Nodes[rowSrc][colSrc];
	cPathNode* endNode = Nodes[rowDst][colDst];

	std::vector<sAStarHelper*> closedSet;
	std::vector<sAStarHelper*> openSet;

	// set up the open set to begin
	openSet.push_back(helpers[rowSrc][colSrc]);
	helpers[rowSrc][colSrc]->gScore = 0.f;
	helpers[rowSrc][colSrc]->fScore = helpers[rowSrc][colSrc]->gScore + ManhattanDist(startNode, endNode);

	sAStarHelper* current;

	while (!openSet.empty())
	{
		// get the node in openSet with the lowest fScoreValue
		current = 0;
		float bestF = FLT_MAX;
		unsigned int openSetSize = openSet.size();
		unsigned int idxOpenBestF = 0;
		for (unsigned int idx = 0; idx < openSetSize; idx++)
		{
			if (openSet[idx]->fScore < bestF)
			{
				bestF = openSet[idx]->fScore;
				current = openSet[idx];
				idxOpenBestF = idx;
			}
		}
		
		if (current == 0) // shouldn't happen
		{
			bool sexybreakpoint = true;
		}


		if (current->Node == endNode) // completion condition, we're there
		{
			std::cout << "FINAL PATH: " << std::endl;
			std::cout << current->Row << " " << current->Col << std::endl;
			// reconstruct the path form cameFrom
			path.push_back(current->Node);
			sAStarHelper* parent = current->Parent;
			while (parent != 0)
			{
				std::cout << parent->Row << " " << parent->Col << std::endl;
				path.push_back(parent->Node);
				parent = parent->Parent;
			}
			openSet.clear();
			break;
		}
		else
		{
			// add it to the closed list
			closedSet.push_back(current);
			// remove it from the open list
			openSet.erase(openSet.begin() + idxOpenBestF);
			
			std::vector<cPathConnection*>::iterator itPath = current->Node->Connections.begin();
			while (itPath != current->Node->Connections.end())   // for each neighbour of current
			{
				cPathNode* neighbourNode = (*itPath)->GetOther(current->Node);
				sAStarHelper* neighbourHelper = 0;
				for (unsigned int idxRow = 0; idxRow < numRows && !neighbourHelper; idxRow++)
				{
					for (unsigned int idxCol = 0; idxCol < numCols && !neighbourHelper; idxCol++)
					{
						if (neighbourNode == helpers[idxRow][idxCol]->Node)
						{
							neighbourHelper = helpers[idxRow][idxCol];
						}
					}
				}

				// are you in the closed set?
				if (std::find(closedSet.begin(), closedSet.end(), neighbourHelper) != closedSet.end())
				{
					itPath++;
					continue;  // ignore the neighbour which is already evaluated
				}

				float tentativeGScore = current->gScore + (*itPath)->Distance * neighbourNode->TravelCostFactor;

				// if you're not already in the open set
				if (std::find(openSet.begin(), openSet.end(), neighbourHelper) == openSet.end())
				{
					openSet.push_back(neighbourHelper); // discover a new node!
				}
				else if (tentativeGScore >= neighbourHelper->gScore)
				{
					itPath++;
					continue;  // this is not a better path
				}
				
				// this path is the best until now, record it!
				neighbourHelper->Parent = current;
				neighbourHelper->gScore = tentativeGScore;
				neighbourHelper->fScore = neighbourHelper->gScore + ManhattanDist(neighbourNode, endNode);

				itPath++;
			}
		}
	}

	for (unsigned int idxRow = 0; idxRow < numRows; idxRow++)
	{
		for (unsigned int idxCol = 0; idxCol < numCols; idxCol++)
		{
			delete helpers[idxRow][idxCol];
		}
	}

	return !path.empty();
}