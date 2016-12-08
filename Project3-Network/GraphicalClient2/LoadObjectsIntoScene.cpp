#include "GlobalStuff.h"
#include "cBMPmapReader.h"
#include "Utilities.h"	// For getRandFloat

#include <vector>
//#include <glm/glm.hpp>

bool LoadObjectsIntoScene(cPlayer*& Player1, cPlayer*& Player2, cPlayer** building)
{
	//instantiate cBMPmapReader object to read the BMP file
	cBMPmapReader aBMPreader;
	aBMPreader.getValuesFromBMPFile("BitMapPositions.bmp");

	float tempLocX = 0.0f;
	float tempLocZ = 0.0f;

	Player1 = new cPlayer();
	tempLocX = aBMPreader.vec_PlayerLocXZ[0].x;
	tempLocZ = aBMPreader.vec_PlayerLocXZ[0].z;
	Player1->mPhysicsInfo.Position.x = tempLocX;
	Player1->mPhysicsInfo.Position.y = 0.0f;
	Player1->mPhysicsInfo.Position.z = tempLocZ;
	Player1->mRenderingInfo.Scale.x = g_pValueReader->Obj1ScaleXYZ;
	Player1->mRenderingInfo.Scale.y = g_pValueReader->Obj1ScaleXYZ;
	Player1->mRenderingInfo.Scale.z = g_pValueReader->Obj1ScaleXYZ;
	Player1->mPlayerNumber = 1;
	Player1->mRenderingInfo.textureName = g_pValueReader->Obj1TexturefileName;
	Player1->mRenderingInfo.meshName = g_pValueReader->Obj1PlyfileName;
	gVec_Entities.push_back(Player1);

	Player2 = new cPlayer();
	tempLocX = aBMPreader.vec_GoalLocXZ[0].x;
	tempLocZ = aBMPreader.vec_GoalLocXZ[0].z;
	Player2->mPhysicsInfo.Position.x = tempLocX;
	Player2->mPhysicsInfo.Position.y = 0.0f;
	Player2->mPhysicsInfo.Position.z = tempLocZ;
	Player2->mRenderingInfo.Scale.x = g_pValueReader->Obj6ScaleXYZ;
	Player2->mRenderingInfo.Scale.y = g_pValueReader->Obj6ScaleXYZ;
	Player2->mRenderingInfo.Scale.z = g_pValueReader->Obj6ScaleXYZ;
	Player2->mPlayerNumber = 2;
	Player2->mRenderingInfo.textureName = g_pValueReader->Obj6TexturefileName;
	Player2->mRenderingInfo.meshName = g_pValueReader->Obj6PlyfileName;
	gVec_Entities.push_back(Player2);

	for (int i = 0; i < 40; ++i)
	{
		building[i] = new cPlayer();
		tempLocX = aBMPreader.vec_GoalLocXZ[0].x;
		tempLocZ = aBMPreader.vec_GoalLocXZ[0].z;
		building[i]->mPhysicsInfo.Position.x = tempLocX;
		building[i]->mPhysicsInfo.Position.y = 0.0f;
		building[i]->mPhysicsInfo.Position.z = tempLocZ;
		building[i]->mRenderingInfo.Scale.x = g_pValueReader->Obj6ScaleXYZ;
		building[i]->mRenderingInfo.Scale.y = g_pValueReader->Obj6ScaleXYZ;
		building[i]->mRenderingInfo.Scale.z = g_pValueReader->Obj6ScaleXYZ;
		building[i]->mPlayerNumber = 3;
		building[i]->mRenderingInfo.textureName = g_pValueReader->Obj4TexturefileName;
		building[i]->mRenderingInfo.meshName = g_pValueReader->Obj4PlyfileName;
		gVec_Entities.push_back(building[i]);
	}



	int numOfBlocks = (int)aBMPreader.vec_BlockLocXZ.size();

	//for the maze
	for (int i = 0; i<numOfBlocks; i++)
	{
		cEntity* pBlock = new cEntity();
		tempLocX = aBMPreader.vec_BlockLocXZ[i].x;
		tempLocZ = aBMPreader.vec_BlockLocXZ[i].z;
		pBlock->mPhysicsInfo.Position.x = tempLocX;
		pBlock->mPhysicsInfo.Position.y = 0.0f;
		pBlock->mPhysicsInfo.Position.z = tempLocZ;
		pBlock->mRenderingInfo.Scale.x = g_pValueReader->Obj4ScaleXYZ;
		pBlock->mRenderingInfo.Scale.y = g_pValueReader->Obj4ScaleXYZ;
		pBlock->mRenderingInfo.Scale.z = g_pValueReader->Obj4ScaleXYZ;
		pBlock->mPhysicsInfo.IsStatic = true;
		pBlock->mRenderingInfo.textureName = g_pValueReader->Obj4TexturefileName;
		pBlock->mRenderingInfo.meshName = g_pValueReader->Obj4PlyfileName;
		//gVec_Entities.push_back(pBlock);
		//will be used as obstacles check
		//gVec_Blocks.push_back(pBlock);
	}

	//// for enemy type 1 position to avoid overlapping any block
	//float enemyType1LocX = 0.0f;
	//float enemyType1LocZ = 0.0f;
	//float enemyType1Scale = g_pValueReader->Obj2ScaleXYZ;
	//for (int i = 0; i < numOfBlocks; i++)
	//{
	//	float tempBlockLocX = aBMPreader.vec_BlockLocXZ[i].x;
	//	float tempBlockLocZ = aBMPreader.vec_BlockLocXZ[i].z;
	//	tempLocX = ::getRandFloat(-22.0f, 0.0f);
	//	tempLocZ = ::getRandFloat(-22.0f, 0.0f);		
	//	if (!(
	//		( //the position point of the block is left bottom corner (min point)
	//			tempBlockLocX >= (tempLocX - tempLocX*enemyType1Scale / 2)
	//			&& tempBlockLocX <= (tempLocX + tempLocX*enemyType1Scale / 2)
	//			||  // the width of a block is 1.0 , this checks the other corner
	//			(tempBlockLocX + 1.0f) >= (tempLocX - tempLocX*enemyType1Scale / 2)
	//			&& (tempBlockLocX + 1.0f) <= (tempLocX + tempLocX*enemyType1Scale / 2)
	//			) &&
	//			(
	//				tempBlockLocZ >= (tempLocZ - tempLocZ*enemyType1Scale / 2)
	//				&& tempBlockLocZ <= (tempLocZ + tempLocZ*enemyType1Scale / 2)
	//				||
	//				(tempBlockLocZ + 1.0f) >= (tempLocZ - tempLocZ*enemyType1Scale / 2)
	//				&& (tempBlockLocZ + 1.0f) <= (tempLocZ + tempLocZ*enemyType1Scale / 2)
	//			)
	//		))
	//	{
	//		enemyType1LocX = tempLocX;
	//		enemyType1LocZ = tempLocZ;
	//		break;
	//	}//if
	//}//for

	//gEnemy = new cEnemy();
	//gEnemy->mType = cEnemy::TYPE1;
	//gEnemy->mPhysicsInfo.Position.x = enemyType1LocX;
	//gEnemy->mPhysicsInfo.Position.y = 0.0f;
	//gEnemy->mPhysicsInfo.Position.z = enemyType1LocZ;
	//gEnemy->mRenderingInfo.Scale.x = enemyType1Scale;
	//gEnemy->mRenderingInfo.Scale.y = enemyType1Scale;
	//gEnemy->mRenderingInfo.Scale.z = enemyType1Scale;
	//gEnemy->collisonDetectPoint = gEnemyYtype1MaxZ * enemyType1Scale;
	//gEnemy->mRenderingInfo.textureName = g_pValueReader->Obj2TexturefileName;
	//gEnemy->mRenderingInfo.meshName = g_pValueReader->Obj2PlyfileName;
	//gVec_Entities.push_back(gEnemy);

	//// for enemy type 2 position
	//float enemyType2LocX = 0.0f;
	//float enemyType2LocZ = 0.0f;
	//float enemyType2Scale = g_pValueReader->Obj3ScaleXYZ;
	//for (int i = 0; i < numOfBlocks; i++)
	//{
	//	float tempBlockLocX = aBMPreader.vec_BlockLocXZ[i].x;
	//	float tempBlockLocZ = aBMPreader.vec_BlockLocXZ[i].z;
	//	tempLocX = ::getRandFloat(0.0f, 22.0f);
	//	tempLocZ = ::getRandFloat(0.0f, 22.0f);
	//	//to avoid enemy's random position overlapping any block
	//	if (!(
	//		( //the position point of the block is left bottom corner (min point)
	//			tempBlockLocX >= (tempLocX - tempLocX*enemyType2Scale / 2)
	//			&& tempBlockLocX <= (tempLocX + tempLocX*enemyType2Scale / 2)
	//			||  // the width of a block is 1.0 , this checks the other corner
	//			(tempBlockLocX + 1.0f) >= (tempLocX - tempLocX*enemyType2Scale / 2)
	//			&& (tempBlockLocX + 1.0f) <= (tempLocX + tempLocX*enemyType2Scale / 2)
	//			) &&
	//			(
	//				tempBlockLocZ >= (tempLocZ - tempLocZ*enemyType2Scale / 2)
	//				&& tempBlockLocZ <= (tempLocZ + tempLocZ*enemyType2Scale / 2)
	//				||
	//				(tempBlockLocZ + 1.0f) >= (tempLocZ - tempLocZ*enemyType2Scale / 2)
	//				&& (tempBlockLocZ + 1.0f) <= (tempLocZ + tempLocZ*enemyType2Scale / 2)
	//				)
	//		))
	//	{
	//		enemyType2LocX = tempLocX;
	//		enemyType2LocZ = tempLocZ;
	//		break;
	//	}//if
	//}//for

	//gEnemy = new cEnemy();
	//gEnemy->mType = cEnemy::TYPE2;
	//gEnemy->mPhysicsInfo.Position.x = enemyType2LocX;
	//gEnemy->mPhysicsInfo.Position.y = 0.0f;
	//gEnemy->mPhysicsInfo.Position.z = enemyType2LocZ;
	//gEnemy->mRenderingInfo.Scale.x = enemyType2Scale;
	//gEnemy->mRenderingInfo.Scale.y = enemyType2Scale;
	//gEnemy->mRenderingInfo.Scale.z = enemyType2Scale;
	//gEnemy->collisonDetectPoint = gEnemyYtype2MaxZ * enemyType2Scale;
	//gEnemy->mRenderingInfo.textureName = g_pValueReader->Obj3TexturefileName;
	//gEnemy->mRenderingInfo.meshName = g_pValueReader->Obj3PlyfileName;
	//gVec_Entities.push_back(gEnemy);

	//for the field
	cEntity* pPlayGround = new cEntity();
	pPlayGround->mRenderingInfo.textureName = g_pValueReader->Obj5TexturefileName;
	pPlayGround->mPhysicsInfo.Position.x = g_pValueReader->Obj5PositionX;
	pPlayGround->mPhysicsInfo.Position.y = g_pValueReader->Obj5PositionY;
	pPlayGround->mPhysicsInfo.Position.z = g_pValueReader->Obj5PositionZ;
	pPlayGround->mRenderingInfo.Scale.x = g_pValueReader->Obj5ScaleXYZ;
	pPlayGround->mRenderingInfo.Scale.y = g_pValueReader->Obj5ScaleXYZ;
	pPlayGround->mRenderingInfo.Scale.z = g_pValueReader->Obj5ScaleXYZ;
	pPlayGround->mPhysicsInfo.IsStatic = true;
	pPlayGround->mRenderingInfo.meshName = g_pValueReader->Obj5PlyfileName;
	gVec_Entities.push_back(pPlayGround);


	
	/********** for cube box(skybox) implementation ************/
	::g_pSkyBoxObject = new cEntity();
	::g_pSkyBoxObject->mRenderingInfo.meshName = "Isoshphere_xyz_InvertedNormals.ply";
	::g_pSkyBoxObject->mRenderingInfo.Scale.x = 1000.0f;
	::g_pSkyBoxObject->mRenderingInfo.Scale.y = 1000.0f;
	::g_pSkyBoxObject->mRenderingInfo.Scale.z = 1000.0f;
	::g_pSkyBoxObject->mRenderingInfo.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	::g_pSkyBoxObject->mPhysicsInfo.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	::g_pSkyBoxObject->mPhysicsInfo.IsStatic = true;


	return true;
}