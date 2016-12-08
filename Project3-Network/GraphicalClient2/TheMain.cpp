/*
* Program Name : INFO6017_AI_Project4
* Purpose      : INFO-6017	Artificial Intelligence	- Project4
* Description  : 
* Date         : Nov. 8, 2016
* Coder        : Jaehuhn Park
*/

#include "GlobalStuff.h"	

#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include "Update.h"

//#include "vert_XYZ_RGB.h"

#include "Utilities.h"

#include "Network/cTCPClient.h"

//void PhysicsStep(float deltaTime);

bool LoadObjectsIntoScene(cPlayer*& Player1, cPlayer*& Player2, cPlayer** building);


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


//#define __TEST_GRAPHICS_MONOPOLY_

static const int numDistricts = 40;
static glm::vec3 districtLocations[] =
{
	// bot line
	{ -23,0,-23 },
	{ -16,0,-23 },
	{ -12,0,-23 },
	{ -8,0,-23 },
	{ -4,0,-23 },
	{ 0,0,-23 },
	{ 4,0,-23 },
	{ 8,0,-23 },
	{ 12,0,-23 },
	{ 16,0,-23 },

	// left
	{ 23, 0, -23 },
	{ 23, 0, -16 },
	{ 23, 0, -12 },
	{ 23, 0,  -8 },
	{ 23, 0,  -4 },
	{ 23, 0,   0 },
	{ 23, 0,   4 },
	{ 23, 0,   8 },
	{ 23, 0,  12 },
	{ 23, 0,  16 },

	// top
	{ 23,0,23 },
	{ 16,0,23 },
	{ 12,0,23 },
	{ 8,0,23 },
	{ 4,0,23 },
	{ -0,0,23 },
	{ -4,0,23 },
	{ -8,0,23 },
	{ -12,0,23 },
	{ -16,0,23 },

	// right
	{ -23, 0, 23 },
	{ -23, 0, 16 },
	{ -23, 0, 12 },
	{ -23, 0,  8 },
	{ -23, 0,  4 },
	{ -23, 0,   0 },
	{ -23, 0,   -4 },
	{ -23, 0,  -8 },
	{ -23, 0,  -12 },
	{ -23, 0,  -16 },
};
cTCPClient* gMonopolyClient = 0;
struct sPlayerInfo
{
	cPlayer* playerObject;
	sProtocolPlayerInfo playerInfo;
	glm::vec3 color;
};
struct sBuildingInfo
{
	cPlayer* buildig;
	sProtocolPlayerInfo* owner;
};
void PlayerUpdate(sPlayerInfo& playerA, sPlayerInfo& playerB, sBuildingInfo* buildingInfos, cTCPClient* client);
int main(void)
{
	cTCPClient* client = new cTCPClient();
	gMonopolyClient = client;

	// initialize server
	if (!client->StartClient())
		exit(EXIT_FAILURE);

	// message loop
	if (!client->RunClient())
		exit(EXIT_FAILURE);


#ifndef __TEST_GRAPHICS_MONOPOLY_
	// wait for game start
	while (!client->IsGameStarted())
	{
		Sleep(100);
	}
#endif


    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	gWindow = glfwCreateWindow(1200, 800, "INFO-6017	Artificial Intelligence	- Project4", NULL, NULL);
    if (!gWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(gWindow, key_callback);
	glfwSetCursorPosCallback(gWindow, mouseposcallback);
	//glfwSetScrollCallback(window, mouseScrollcallback);

    glfwMakeContextCurrent(gWindow);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);


	std::cout << glGetString(GL_VENDOR) << " " 
		<< glGetString(GL_RENDERER) << ", " 
		<< glGetString(GL_VERSION) << std::endl;
	std::cout << "Shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	::g_pTheShaderManager = new cShaderManager();

	cShaderManager::cShader verShader;	
	cShaderManager::cShader fragShader;

	verShader.fileName = "uberVertShader.glsl";
	fragShader.fileName = "uberFragShader.glsl";
	::g_pTheShaderManager->setBasePath( "assets//shaders//" );
	if ( ! ::g_pTheShaderManager->createProgramFromFile( "simple", verShader, fragShader ) )
	{	// Oh no!
		std::cout << ::g_pTheShaderManager->getLastError() << std::endl;
		std::cout.flush();
		// TODO: Shut it all down... 
		return -1;
	}

	::g_pTheMeshTypeManager = new cMeshTypeManager();


	::shadProgID = ::g_pTheShaderManager->getIDFromFriendlyName("simple");
	::g_pTheMeshTypeManager->SetBaseFilePath( "assets//models//" );

	::g_pTheMeshTypeManager->ScaleEverythingToBoundingBoxOnLoad( false, 1.0f );

	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "Homer_x10_0Y_N.ply");//palyer
	//::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Bunny_x100_CenterX_0Y_N.ply"); //slower enemy
	//::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Cow_0Y_CenterX_N.ply");		//faster enemy		
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Cube_Unit_CornerAtOrigin_N.ply"); //wall
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "1x1_2Tri_Quad_1_Sided_XZ_Plane.ply"); //ground
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Isoshphere_xyz_InvertedNormals.ply");	//background
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "AUDI_TT_COUPE_decimate01_N.ply");	//background

	LightShaderInit();

	::g_pTextureManager = new cBasicTextureManager();

	::g_pTextureManager->SetBasePath("assets/textures");
	::g_pTextureManager->Create2DTextureFromBMPFile("purple_texture_1.bmp", true);
	//::g_pTextureManager->Create2DTextureFromBMPFile("furTexture.bmp", true);
	//::g_pTextureManager->Create2DTextureFromBMPFile( "cowTexture.bmp", true );
	::g_pTextureManager->Create2DTextureFromBMPFile("wallTexture03.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("monopolyBoard_x1500.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("gold_textured_background_hd_picture.bmp", true);

	/********** for cube box implementation ************/
	GLenum ignoreerorr = glGetError();

	::g_pTextureManager->SetBasePath("assets/textures/skybox");
	GLenum glErrorEnum = 0;
	std::string errorString, errorDetails;
	if (!::g_pTextureManager->CreateNewCubeTextureFromBMPFiles("SkyMap",
		"sky1_texture_right1_posX.bmp",
		"sky1_texture_left2_negX.bmp",
		"sky1_texture_top3_posY.bmp",
		"sky1_texture_bottom4_negY.bmp",
		"sky1_texture_front5_posZ.bmp",
		"sky1_texture_back6_negZ.bmp",
		true, glErrorEnum,
		errorString,
		errorDetails))
	{
		std::cout << "Trouble loading skybox" << std::endl;
		std::cout << "Error: " << errorString << std::endl;
		std::cout << "Details: " << errorDetails << std::endl;
	}


	locID_matModel = glGetUniformLocation(shadProgID, "matModel");
	if (dGLErrorCheck("matModel"))
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	locID_matView = glGetUniformLocation(shadProgID, "matView");
	if (dGLErrorCheck("matView"))
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	locID_matProj = glGetUniformLocation(shadProgID, "matProj");

	
	::g_pValueReader = new cValueReader();
	::g_pValueReader->getValuesFromTxtFile("SceneDescription.txt");

	
	gCamera = new cCamera();
	gAreaInfo = new cAreaInfo();

	glm::vec3 initPoint(-25.0f, 0.0f, -25.f);
	glm::vec3 edgeAPoint(25.0f, 0.0f, -25.f);
	glm::vec3 edgeBPoint(-25.0f, 0.0f, 25.f);

	//gPathNodeGrid = new cPathNodeGrid(50, 50, initPoint, edgeAPoint, edgeBPoint);
	

	////////////////////////////////////////////////////////////////////////////
	// player infomations
	sPlayerInfo playerA;
	sPlayerInfo playerB;
	sBuildingInfo buildingInfos[numDistricts];

	playerA.playerInfo = client->MonopolyPacketProcedure()->MyInfo();
	playerB.playerInfo = client->MonopolyPacketProcedure()->OpponentInfo();

	cPlayer* p1, *p2, *buildig[numDistricts];
	if ( ! LoadObjectsIntoScene(p1, p2, buildig) )
	{
		std::cout << "WARNING: Could not load all models into the scene." << std::endl;
	}

#ifdef __TEST_GRAPHICS_MONOPOLY_
	playerA.playerObject = p1;
	playerB.playerObject = p2;
#else
	if (playerA.playerInfo.id == 0)
	{
		playerA.playerObject = p1;
		playerB.playerObject = p2;
	}
	else
	{
		playerA.playerObject = p2;
		playerB.playerObject = p1;
	}

	playerA.playerObject->mRenderingInfo.Color = glm::vec3(1.0f, 0.5f, 0.5f);
	playerB.playerObject->mRenderingInfo.Color = glm::vec3(0.5f, 0.5f, 1.0f);
#endif

	playerA.playerObject->mPhysicsInfo.Position = { -23,0,-23 };
	playerB.playerObject->mPhysicsInfo.Position = { -23,0,-23 };

	// player infomations
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// building infomations
	for (int i = 0; i < numDistricts; ++i)
	{
		buildingInfos[i].buildig = buildig[i];
		buildingInfos[i].buildig->isVisible = false;
		buildingInfos[i].buildig->mPhysicsInfo.Position = districtLocations[i];
		buildingInfos[i].buildig->mRenderingInfo.Scale = glm::vec3(1.5f, 1.5f, 1.5f);
		if (buildingInfos[i].buildig->mPhysicsInfo.Position.z < -20)
		{
			// bot line
			buildingInfos[i].buildig->mPhysicsInfo.Position.z = -20;
		}
		else if (buildingInfos[i].buildig->mPhysicsInfo.Position.z > 20)
		{
			// top line
			buildingInfos[i].buildig->mPhysicsInfo.Position.z = 20;
		}
		else if (buildingInfos[i].buildig->mPhysicsInfo.Position.x < -20)
		{
			// right line
			buildingInfos[i].buildig->mPhysicsInfo.Position.x = -20;
		}
		else if (buildingInfos[i].buildig->mPhysicsInfo.Position.x > 20)
		{
			// left line
			buildingInfos[i].buildig->mPhysicsInfo.Position.x = 20;
		}
		buildingInfos[i].buildig->mPhysicsInfo.Position.z -= 2;
		buildingInfos[i].owner = 0;
	}

	// building infomations
	////////////////////////////////////////////////////////////////////////////



	UniformLoc_ID_objectColour = glGetUniformLocation( shadProgID, "objectColour" );
	UniformLoc_ID_isWireframe = glGetUniformLocation( shadProgID, "isWireframe" );
	UniformLoc_ID_bUseObjColorNoTexture = glGetUniformLocation( shadProgID, "bUseObjColorNoTexture" );

	UniformLoc_alpha = glGetUniformLocation( shadProgID, "alpha" );

	UniformLoc_texSamp2D_00 = glGetUniformLocation( shadProgID, "texSamp2D_00" );
	UniformLoc_texSamp2D_01 = glGetUniformLocation( shadProgID, "texSamp2D_01" );
	UniformLoc_texSamp2D_02 = glGetUniformLocation( shadProgID, "texSamp2D_02" );
	UniformLoc_texSamp2D_03 = glGetUniformLocation( shadProgID, "texSamp2D_03" );

	UniformLoc_bUseTextureAsDiffuse = glGetUniformLocation( shadProgID, "bUseTextureAsDiffuse" );
	UniformLoc_bUseLighting = glGetUniformLocation( shadProgID, "bUseLighting" );
	UniformLoc_bIsImposterBlack  = glGetUniformLocation( shadProgID, "bIsImposterBlack" );

	UniformLoc_bIsCubeMapObject = glGetUniformLocation(shadProgID, "bIsCubeMapObject");
	UniformLoc_myCubemapSkyBoxSampler = glGetUniformLocation(shadProgID, "myCubemapSkyBoxSampler");
	

	glEnable(GL_DEPTH_TEST);

	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(gWindow))
	{
		int width, height;
		glfwGetFramebufferSize(gWindow, &width, &height);
		gWindowRatio = width / (float)height;

		//glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		static const float MAXDELTATIME = 0.1f;	// 100 ms
		float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
		lastTime = glfwGetTime();
		if (deltaTime > MAXDELTATIME)
		{
			deltaTime = MAXDELTATIME;
		}

		LightSetting();

		// logic update
		PlayerUpdate(playerA, playerB, buildingInfos, client);
		//Update();

		glm::mat4 matProjection;
		glm::mat4 matView;
		gCamera->GetProjectionMatrix(matProjection);
		gCamera->GetViewMatrix(matView);

		glUniformMatrix4fv(locID_matProj, 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(matProjection));

		// This is set once at the start of the "scene" draw.
		glUniformMatrix4fv(locID_matView, 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(matView));


		glUniform1i(UniformLoc_bIsCubeMapObject, FALSE);
		DrawSkyBox();

		// Draw Scene
		for (int index = 0; index != ::gVec_Entities.size(); index++)
		{
			cEntity* pCurrEntity = ::gVec_Entities[index];
			if (pCurrEntity->isVisible)
			{
				DrawObject(pCurrEntity);
			}
		}




		// Show or "present" what we drew...
		glfwSwapBuffers(gWindow);

		glfwPollEvents();
	}


	//free the heap
	delete ::g_pTheMeshTypeManager;
	delete ::g_pTheShaderManager;
	delete ::g_pTextureManager;
	delete ::g_pValueReader;
	delete ::gCamera;
	delete ::gAreaInfo;
	gCamera = 0;
	//freeing the heap allocated by all the entities 
	for (auto x : gVec_Entities)
	{
		delete x;
		x = 0;
	}

    glfwDestroyWindow(gWindow);
    glfwTerminate();

	// clean up server
	if (!client->ShutDown())
		exit(EXIT_FAILURE);

	delete client;
	gMonopolyClient = 0;
	client = 0;

    exit(EXIT_SUCCESS);

}//end of Main()

void DrawSkyBox(void)
{
	glUniform1i(UniformLoc_bIsCubeMapObject, TRUE);

	GLuint texNumberSky = ::g_pTextureManager->getTextureIDFromName("SkyMap");

	GLuint texture01Unit = 0;
	glActiveTexture(texture01Unit + GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, texNumberSky);

	glUniform1i(UniformLoc_myCubemapSkyBoxSampler, texture01Unit);

	//glDisable(GL_DEPTH);			// turn off depth test (i.e. just write)
	glDepthMask(GL_FALSE);		// no writing to z or depth buffer
	DrawObject(::g_pSkyBoxObject, false);
	glDepthMask(GL_TRUE);
	//glEnable(GL_DEPTH);

	glUniform1i(UniformLoc_bIsCubeMapObject, FALSE);

	return;
}



//update the scene and supplying delta time to camera and entities
void PlayerUpdate(sPlayerInfo& playerA, sPlayerInfo& playerB, sBuildingInfo* buildingInfos, cTCPClient* client)
{

	double tmpLastTime = glfwGetTime();
	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	if (deltaTime == 0.f)
	{
		lastTime = currentTime;
		return;
	}
	if (deltaTime < 0.01)
	{
		return;
	}
	if (deltaTime > 0.05)
	{
		deltaTime = 0.05;
	}

	static const float pieceAnimationDurationPerDistrict = 0.2f;
	static float pieceAnimationTime = 0.0f;
	//static int pieceCurrentLocationA = 0;
	//static int pieceCurrentLocationB = 0;
	static glm::vec3 currentLocationA = districtLocations[0];
	static glm::vec3 nextLocationA = districtLocations[0];
	static glm::vec3 currentLocationB = districtLocations[0];
	static glm::vec3 nextLocationB = districtLocations[0];

	static const float jumpHeight = 3;
	static float ratioAnimationTime;

	if (client->GetState() == iTCPClient::eGameMonopolyState::e_GM_AnimationThrowDice)
	{
		// TODO: doing something

		client->SetState(iTCPClient::eGameMonopolyState::e_GM_MovePiece);
	}
	else if (client->GetState() == iTCPClient::eGameMonopolyState::e_GM_AnimationMovePiece)
	{
		pieceAnimationTime += deltaTime;

		if (pieceAnimationTime >= pieceAnimationDurationPerDistrict)
		{
			pieceAnimationTime = 0;

			if (client->MonopolyPacketProcedure()->IsMyTurn())
			{
				currentLocationA = districtLocations[playerA.playerInfo.location];
				if (playerA.playerInfo.location == client->MonopolyPacketProcedure()->MyInfo().location)
				{
					client->SetState(iTCPClient::eGameMonopolyState::e_GM_Wait);
				}
				playerA.playerInfo.location++;
				if (playerA.playerInfo.location >= numDistricts)
					playerA.playerInfo.location = 0;


				nextLocationA = districtLocations[playerA.playerInfo.location];
			}
			else
			{
				currentLocationB = districtLocations[playerB.playerInfo.location];
				if (playerB.playerInfo.location == client->MonopolyPacketProcedure()->OpponentInfo().location)
				{
					client->SetState(iTCPClient::eGameMonopolyState::e_GM_Wait);
				}
				playerB.playerInfo.location++;
				if (playerB.playerInfo.location >= numDistricts)
					playerB.playerInfo.location = 0;


				nextLocationB = districtLocations[playerB.playerInfo.location];
			}
		}

		ratioAnimationTime = (pieceAnimationTime / pieceAnimationDurationPerDistrict);
		if (client->MonopolyPacketProcedure()->IsMyTurn())
		{
			// move
			playerA.playerObject->mPhysicsInfo.Position = currentLocationA + (nextLocationA - currentLocationA) * ratioAnimationTime;
			// jump
			playerA.playerObject->mPhysicsInfo.Position.y = glm::sin(glm::pi<float>() * ratioAnimationTime) * jumpHeight;
		}
		else
		{
			// move
			playerB.playerObject->mPhysicsInfo.Position = currentLocationB + (nextLocationB - currentLocationB) * ratioAnimationTime;
			// jump
			playerB.playerObject->mPhysicsInfo.Position.y = glm::sin(glm::pi<float>() * ratioAnimationTime) * jumpHeight;
		}
	}
	else if (client->GetState() == iTCPClient::eGameMonopolyState::e_GM_AnimationBuy)
	{
		if (client->MonopolyPacketProcedure()->IsMyTurn())
		{
			if (buildingInfos[client->MonopolyPacketProcedure()->MyInfo().location].owner == 0 &&
				client->MonopolyPacketProcedure()->CurrentDistrictInfo().owner_id >= 0)
			{
				pieceAnimationTime = 0;
				buildingInfos[client->MonopolyPacketProcedure()->MyInfo().location].owner = &client->MonopolyPacketProcedure()->MyInfo();
				buildingInfos[client->MonopolyPacketProcedure()->MyInfo().location].buildig->isVisible = true;
				buildingInfos[client->MonopolyPacketProcedure()->MyInfo().location].buildig->mRenderingInfo.Color = playerA.playerObject->mRenderingInfo.Color;
				buildingInfos[client->MonopolyPacketProcedure()->MyInfo().location].buildig->mPhysicsInfo.Position.y = jumpHeight;
			}
			else
			{
				client->SetState(iTCPClient::eGameMonopolyState::e_GM_Wait);
			}
		}
		else
		{
			if (buildingInfos[client->MonopolyPacketProcedure()->OpponentInfo().location].owner == 0 &&
				client->MonopolyPacketProcedure()->CurrentDistrictInfo().owner_id >= 0)
			{
				pieceAnimationTime = 0;
				buildingInfos[client->MonopolyPacketProcedure()->OpponentInfo().location].owner = &client->MonopolyPacketProcedure()->OpponentInfo();
				buildingInfos[client->MonopolyPacketProcedure()->OpponentInfo().location].buildig->isVisible = true;
				buildingInfos[client->MonopolyPacketProcedure()->OpponentInfo().location].buildig->mRenderingInfo.Color = playerB.playerObject->mRenderingInfo.Color;
				buildingInfos[client->MonopolyPacketProcedure()->OpponentInfo().location].buildig->mPhysicsInfo.Position.y = jumpHeight;
			}
			else
			{
				client->SetState(iTCPClient::eGameMonopolyState::e_GM_Wait);
			}
		}

		pieceAnimationTime += deltaTime;
		ratioAnimationTime = (pieceAnimationTime / pieceAnimationDurationPerDistrict);

		if (pieceAnimationTime >= pieceAnimationDurationPerDistrict)
		{
			pieceAnimationTime = 0;
			client->SetState(iTCPClient::eGameMonopolyState::e_GM_Wait);
			if (client->MonopolyPacketProcedure()->IsMyTurn())
			{
				buildingInfos[client->MonopolyPacketProcedure()->MyInfo().location].buildig->mPhysicsInfo.Position.y = 0;
			}
			else
			{
				buildingInfos[client->MonopolyPacketProcedure()->OpponentInfo().location].buildig->mPhysicsInfo.Position.y = 0;
			}
		}

		if (client->MonopolyPacketProcedure()->IsMyTurn())
		{
			buildingInfos[client->MonopolyPacketProcedure()->MyInfo().location].buildig->mPhysicsInfo.Position.y = glm::cos(glm::pi<float>() * ratioAnimationTime) * jumpHeight;
		}
		else
		{
			buildingInfos[client->MonopolyPacketProcedure()->OpponentInfo().location].buildig->mPhysicsInfo.Position.y = glm::cos(glm::pi<float>() * ratioAnimationTime) * jumpHeight;
		}
	}


	gCamera->Update(deltaTime);

	lastTime = currentTime;

}

