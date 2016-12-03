#ifndef _GELMESHMANAGER_HG_
#define _GELMESHMANAGER_HG_

#include "GelStructures.h"
#include <string>
#include <vector>
#include <map>

#include "iManager.h"

class gelMeshManager : iManager 
{
public:
	//Singleton  
	static gelMeshManager& sharedInstance()
	{
		static gelMeshManager instance;
		return instance;   
	}

	void ScaleEverythingToBoundingBoxOnLoad(bool scale, float boxWidth);

	//Load a Ply file into the GL buffer, third is the short name
	bool LoadPlyIntoGLBuffer(unsigned int programID, std::string alias, std::string plyFile, bool isCollisionModel = false, int collisionScale = 1.0f);//added new flag and scale.
	bool LookUpMeshInfo(std::string name, unsigned int &VAO_ID, int &numberOfIndices, float &unitScale);
	float LookUpMeshRadius(std::string name);

	//trying different kinds of stuff to access that huge triangle array/map
	bool GetCollisionModelTriangleInfoForMesh(std::string meshName, float& halfExtend, glm::vec3& minPoint);
	bool GetCollisionModelTriangleArrayForMesh(std::string meshName, int AABBid, std::vector<GelTriangle> &vecTriangle);
	bool GetCollisionModelTriangleMapForMesh(std::string meshName, std::map<int, std::vector<GelTriangle> > &mapTriangle);

	bool GetTriangleArrayForMesh(std::string meshName, std::vector<GelTriangle> &vecTriangle);

	virtual void Update(float deltaTime);
	void Setup();
	//~GelMeshManager();
private:
	gelMeshManager() {};
	gelMeshManager(gelMeshManager const&); //dont implement - to build
	void operator=(gelMeshManager const&); //dont implement - to build

	bool ReadPlyFileHeaders(std::string fileName, int& vertexCount, int& faceCount, int& propCount);
	bool LoadPlyFile(GelVertex* vertexArray, int* indexArray, std::string alias, std::string fileName, int vertexCount, int faceCount, int propCount, float& radius, bool isCollisionModel, int collisionScale);

	bool scaleToBoundingBoxOnLoad;	// = false by default
	float boundingBoxScalingSize;	// = 1.0f by default

	enum enumTEXCOORDBIAS
	{
		POSITIVE_X, POSITIVE_Y, POSITIVE_Z
	};

	struct VAOInfo
	{
		VAOInfo() : VAO_ID(0), numberOfIndices(0), radius(1.0f), unitScaleValue(1.0f) {}
		std::string meshName;
		std::string name;
		unsigned int VAO_ID; //refactor pls b0ss
		unsigned int numberOfIndices;
		float radius; //for collision
		float unitScaleValue; //multiply by this scale and object 1.0f
	};

	struct CollisionTriangleInfo
	{
		CollisionTriangleInfo() : alias(""), aabbHalfExtend(5.0f), collisionModelScale(1.0f) {}
		float collisionModelScale;
		float aabbHalfExtend; //multiply by this scale and object 1.0f
		glm::vec3 minPoint;
		std::string alias;
		std::map<int, std::vector<GelTriangle> > aabbTriangleMap;
	};

	// Map... aka "dictionay" "look up table"
	std::map< std::string, VAOInfo > meshMap;

	std::map< std::string, CollisionTriangleInfo > collisionMap;

	//this will be moved to elsewhere, added for physics
	std::map< std::string, std::vector<GelTriangle> > triangleMap;
	std::map< std::string, std::vector<GelTriangle> > triangleMap_CollisionModels;

	std::map < std::string, std::map<int, std::vector<GelTriangle> > > map_aliasTo_AABBToTriangleArray;
	std::map<int, std::vector<GelTriangle> > temp_collision_triangles;
};


#endif // !_GELMESHMANAGER_HG_
