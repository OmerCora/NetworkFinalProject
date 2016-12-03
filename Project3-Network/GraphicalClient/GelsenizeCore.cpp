//GELSENIZE ENGINE

//GELSENIZE GELSENIZE GELSENIZE

//Aimed to be developed in parallel with the progress of GDP classes.
//Main intention is to create a "game engine" on the go, with 0 game development experience.
//Reusing bunch of lines of code from the classes =)

//Omer Cora, 2016

#include "GelsenizeCore.h"

#include "managers.h"
#include "cContact.h"

//TODO: config
#define GELSENIZE_BUILD 42
#define GELSENIZE_VER NULL

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>		// "String Stream", used for setting the windows title

#include <vector>
#include <fstream>
#include <string>
#include "iGameObject.h"

#include "gelObjectBuilder.h"
#include "gelWorldManager.h" //world object mediator

//JSON
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>

#include "OCHelper.h"
#include "commands.h"

#include "cFlock.h"

//abstraction soon
void PhysicsStep(float deltaTime);
void CollisionStep(float deltaTime);
float getRandFloat(float LO, float HI);
std::vector< cContact > g_vec_Contacts;

iGameObject *g_ambientLight0;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	//Wireframe mode toggle for each game object
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		std::vector<iGameObject*> gameObjects = gelObjectManager::sharedInstance().GetGameObjects();

		for (int index = 0; index != gameObjects.size(); index++)
		{
			RenderInfo render = gameObjects[index]->GetRender();
			render.isWireframe = !render.isWireframe;
			gameObjects[index]->SetRender(render);
		}
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		gelSoundManager::sharedInstance().DecreaseMasterVolume();
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		gelSoundManager::sharedInstance().IncreaseMasterVolume();
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		gelSoundManager::sharedInstance().DecreasePan();
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		gelSoundManager::sharedInstance().IncreasePan();
	}
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		gelSoundManager::sharedInstance().DecreasePitch();
	}
	if (key == GLFW_KEY_6 && action == GLFW_PRESS)
	{
		gelSoundManager::sharedInstance().IncreasePitch();
	}

	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
	{
		iGameObject* player = gelObjectManager::sharedInstance().GetMainPlayer();

		PhysicsInfo physics = player->GetPhysics();
		physics.velocity *= 0.0f;
		physics.acceleration *= 0.0f;
		physics.rotationalVelocity *= 0.0f;
		
		RenderInfo render = player->GetRender();
		glm::quat resetOrientation;
		render.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		render.orientation = resetOrientation;

		player->SetPhysics(physics);
		player->SetRender(render);

		//bind this json generation to a GelCommandBuilder <3
		cCommandGroup cmdGrp;
		{
			rapidjson::Document initDoc;

			rapidjson::Value initData;
			initData.SetObject();

			rapidjson::Value duration(rapidjson::kNumberType);
			rapidjson::Value easeInRatio(rapidjson::kNumberType);
			rapidjson::Value easeOutRatio(rapidjson::kNumberType);

			duration.SetFloat(1.4f);
			easeInRatio.SetFloat(0.05f);
			easeOutRatio.SetFloat(0.05f);

			rapidjson::Value destination(rapidjson::kObjectType);
			{
				rapidjson::Value destination_x(rapidjson::kNumberType);
				rapidjson::Value destination_y(rapidjson::kNumberType);
				rapidjson::Value destination_z(rapidjson::kNumberType);

				destination_x.SetFloat(0.0f);
				destination_y.SetFloat(8.0f);
				destination_z.SetFloat(0.0f);

				// adding elements to contacts array.
				destination.AddMember("x", destination_x, initDoc.GetAllocator());
				destination.AddMember("y", destination_y, initDoc.GetAllocator());
				destination.AddMember("z", destination_z, initDoc.GetAllocator());
			}
			initData.AddMember("destination", destination, initDoc.GetAllocator());
			initData.AddMember("easeInRatio", easeInRatio, initDoc.GetAllocator());
			initData.AddMember("easeOutRatio", easeOutRatio, initDoc.GetAllocator());
			initData.AddMember("duration", duration, initDoc.GetAllocator());

			iCommand* p_comMoveB = new cMoveCommand();
			p_comMoveB->SetGameObject(player);
			p_comMoveB->Init(initData);
			//myCG.AddSerial( p_comMoveB );
			//cmdGrp.AddParallel(p_comMoveB);
			cmdGrp.AddParallelCommand(p_comMoveB);
		}
		GelCommandManager::sharedInstance().AddCommandGroup(cmdGrp);
		
		//LightInfo light = player->GetLight();
		//if (light.typeAndParams.x == 1) //spot
		//{
		//	light.typeAndParams.x = 0; //change to point 0
		//}
		//else if (light.typeAndParams.x == 0)
		//{
		//	light.typeAndParams.x = 1;
		//}
		//player->SetLight(light);
	}

	glm::vec4 eyeOut;
	gelCameraManager::sharedInstance().mainCamera->GetEyePosition(eyeOut);
	GelPosition playerPosition = gelObjectManager::sharedInstance().GetMainPlayer()->GetRender().position;

	std::stringstream ssTitle;
	ssTitle << "Camera: "
		<< eyeOut.x << ", "
		<< eyeOut.y << ", "
		<< eyeOut.z
		<< "  Player: "
		<< playerPosition.x << ", "
		<< playerPosition.y << ", "
		<< playerPosition.z;
	glfwSetWindowTitle(window, ssTitle.str().c_str());

	return;
}

float GelsenizeCore::GetPlaygroundScale(void)
{
	return this->playgroundScale;
}

float GelsenizeCore::GetPlayerSpeed(void)
{
	return this->playerSpeed;
}

float GelsenizeCore::GetAttentionRadius(void)
{
	return this->attentionRadius;
}

float GelsenizeCore::GetDangerRadius(void)
{
	return this->dangerRadius;
}

GLFWwindow * GelsenizeCore::GetWindow(void)
{
	return this->window;
}

GLuint GelsenizeCore::GetModelLocation(void)
{
	return this->locID_matModel;
}

GLuint GelsenizeCore::GetViewLocation(void)
{
	return this->locID_matView;
}

GLuint GelsenizeCore::GetProjLocation(void)
{
	return this->locID_matProj;
}

GLuint GelsenizeCore::ObjectAlphaID(void)
{
	return this->uniformId_object_alpha;
}

GLuint GelsenizeCore::DiscardTextureBlackID(void)
{
	return this->uniformLoc_bIsDiscardTextureBlack;
}

GLuint GelsenizeCore::TexSamp2DID(int index)
{
	if (index >= (int)textureCount)
	{
		return this->uniformLoc_texSamp2D[0];
	}
	else
	{
		return this->uniformLoc_texSamp2D[index];
	}
}

GLuint GelsenizeCore::UsedTexNumID(void)
{
	return this->uniformLoc_usedTexNum;
}

GLuint GelsenizeCore::CubemapSkyBoxSamplerID(void)
{
	return this->uniformLoc_myCubemapSkyBoxSampler;
}

GLuint GelsenizeCore::IsCubeMapObjectID(void)
{
	return this->uniformLoc_bIsCubeMapObject;
}

GLuint GelsenizeCore::UseLightingID(void)
{
	return this->uniformLoc_bUseLighting;
}

GLuint GelsenizeCore::IsImposterBlackIsAlphaID(void)
{
	return this->uniformLoc_bIsImposterBlackIsAlpha;
}

GLuint GelsenizeCore::UseTextureAsDiffuseID(void)
{
	return this->uniformLoc_bUseTextureAsDiffuse;
}

GLuint GelsenizeCore::AlphaID(void)
{
	return this->uniformId_alpha;
}

GLuint GelsenizeCore::ModelColorID(void)
{
	return this->uniformId_ModelColor;
}

GLuint GelsenizeCore::isWireframeID(void)
{
	return this->uniformId_isWireframe;
}

GLuint GelsenizeCore::UseGlobalAlphaValue(void)
{
	return this->uniformId_use_alpha;
}

GLuint GelsenizeCore::GlobalAlphaValue(void)
{
	return this->uniformId_alpha;
}

GLuint GelsenizeCore::DebugColorID(void)
{
	return this->uniformId_userDebugColorsNoLighting;
}

GLuint GelsenizeCore::EyeLocationID(void)
{
	return this->uniformId_eyeLocation;
}



// game loop below




void GelsenizeCore::StartGameLoop(void)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(1200, 800, "Gelsenize Engine", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	std::cout << glGetString(GL_VENDOR) << " "
		<< glGetString(GL_RENDERER) << ", "
		<< glGetString(GL_VERSION) << std::endl;
	std::cout << "Shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	//	std::cout << glGetString(GL_EXTENSIONS) << std::endl;

	//read shader files and compile the shaders
	ProcessShaderFiles();

	GLuint programID = gelShaderManager::sharedInstance().getIDWithName("default");
	
	locID_matModel = glGetUniformLocation(programID, "matModel");
	locID_matView = glGetUniformLocation(programID, "matView");
	locID_matProj = glGetUniformLocation(programID, "matProj");

	//this turned into light manager's setup
	locID_lightPosition = glGetUniformLocation(programID, "myLightPosition");
	locID_lightDiffuse = glGetUniformLocation(programID, "myLightDiffuse");
	//now new code
	gelLightManager::sharedInstance().SetupUniforms(programID);

	uniformId_isWireframe = glGetUniformLocation(programID, "isWireframe"); //unused for now
	uniformId_ModelColor = glGetUniformLocation(programID, "objectColour");
	uniformId_userDebugColorsNoLighting = glGetUniformLocation(programID, "bUseDebugDiffuseNoLighting");
	uniformId_eyeLocation = glGetUniformLocation(programID, "eyeLocation");
	uniformId_alpha = glGetUniformLocation(programID, "globalAlphaValue");
	uniformId_use_alpha= glGetUniformLocation(programID, "useGlobalAlphaValue");
	uniformId_object_alpha = glGetUniformLocation(programID, "alpha");

	uniformLoc_texSamp2D[0] = glGetUniformLocation(programID, "texSamp2D_00");
	uniformLoc_texSamp2D[1] = glGetUniformLocation(programID, "texSamp2D_01");
	uniformLoc_texSamp2D[2] = glGetUniformLocation(programID, "texSamp2D_02");
	uniformLoc_texSamp2D[3] = glGetUniformLocation(programID, "texSamp2D_03");

	uniformLoc_bUseTextureAsDiffuse = glGetUniformLocation(programID, "bUseTextureAsDiffuse");
	uniformLoc_bUseLighting = glGetUniformLocation(programID, "bUseLighting");
	uniformLoc_bIsDiscardTextureBlack = glGetUniformLocation(programID, "bIsDiscardTextureBlack");
	uniformLoc_bIsImposterBlackIsAlpha = glGetUniformLocation(programID, "bIsImposterBlackIsAlpha");

	uniformLoc_bIsCubeMapObject = glGetUniformLocation(programID, "bIsCubeMapObject");
	uniformLoc_myCubemapSkyBoxSampler = glGetUniformLocation(programID, "myCubemapSkyBoxSampler");

	uniformLoc_usedTexNum = glGetUniformLocation(programID, "usedTexNum");

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//setup before reading objects
	gelObjectManager::sharedInstance().Setup();
	gelSoundManager::sharedInstance().Setup();
	gelMeshManager::sharedInstance().Setup();
	gelRenderManager::sharedInstance().Setup();

	gelMeshManager::sharedInstance().ScaleEverythingToBoundingBoxOnLoad(true, 10.0f);

	//read /resources/gameFile/gameFile.txt to get:
	//  the mesh list and load them into the buffer
	//  player model, position, physics, etc
	//  object list and their info
	//ReadGameFiles(); //old .txt fileformat
	ReadGameFiles_2();// json

	//add custom lights
	g_ambientLight0 = gelObjectFactory::sharedInstance().createGameObject("light_source");
	iGameObject *ambientLight1 = gelObjectFactory::sharedInstance().createGameObject("light_source");
	iGameObject *ambientLight2 = gelObjectFactory::sharedInstance().createGameObject("light_source");
	iGameObject *ambientLight3 = gelObjectFactory::sharedInstance().createGameObject("light_source");
	RenderInfo render;
	MeshInfo mesh;
	mesh.name = "isosphere";
	float addThis = -3.0f;
	render.position.x = 52.0f;
	render.position.y = 10.0f;
	render.position.z = -50.0f;
	render.color.r = 1.0f;
	render.color.g = 1.0f;
	render.color.b = 1.0f;
	render.scale = 0.2f;
	float model_radius = gelMeshManager::sharedInstance().LookUpMeshRadius(mesh.name);
	render.SetRadius(model_radius);
	g_ambientLight0->SetRender(render);
	g_ambientLight0->SetMesh(mesh);
	
	render.position.x = this->playgroundScale/10.0f +addThis;
	render.position.z = 0.0f -addThis;

	ambientLight1->SetRender(render);
	ambientLight1->SetMesh(mesh);

	render.position.x = this->playgroundScale / 10.0f + addThis;
	render.position.z = this->playgroundScale / 10.0f + addThis;

	ambientLight2->SetRender(render);
	ambientLight2->SetMesh(mesh);

	render.position.x = 0.0f - addThis;
	render.position.z = this->playgroundScale / 10.0f + addThis;
	ambientLight3->SetRender(render);
	ambientLight3->SetMesh(mesh);

	LightInfo lightAmb;
	lightAmb.bIsEnabled = true;
	lightAmb.position = glm::vec3(render.position.x, render.position.y, render.position.z);	// Likely not needed as vec3 c'tor sets to zeros
	lightAmb.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	lightAmb.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	lightAmb.specular = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	lightAmb.attenConst = 0.0f;
	lightAmb.attenLinear = 0.01f;
	lightAmb.attenQuad = 0.0f;

	lightAmb.typeAndParams.x = 1; //set to spotlight
	lightAmb.typeAndParams.y = 0.9999f;
	lightAmb.typeAndParams.z = 0.7000f;
	lightAmb.direction = glm::vec4(1.0f, -1.0f, 1.0f, 0.0f);

	g_ambientLight0->SetLight(lightAmb);
	ambientLight1->SetLight(lightAmb);
	ambientLight2->SetLight(lightAmb);


	lightAmb.bIsEnabled = true;
	lightAmb.position = glm::vec3(render.position.x, render.position.y, render.position.z);	// Likely not needed as vec3 c'tor sets to zeros
	lightAmb.diffuse = glm::vec3(0.1f, 0.1f, 0.1f);
	lightAmb.ambient = glm::vec3(0.40f, 0.40f, 0.40f);
	lightAmb.specular = glm::vec4(0.2f, 0.2f, 0.2f, 0.6f);
	lightAmb.attenConst = 0.0f;
	lightAmb.attenLinear = 0.0f;
	lightAmb.attenQuad = 0.0f;

	lightAmb.typeAndParams.x = 2; //set to directional
	//lightAmb.typeAndParams.y = 0.7;
	//lightAmb.typeAndParams.z = 0.5;
	//lightAmb.direction = glm::vec4(0, 1.0f, 1.0f, 0.0f);

	render.position.x = 500.0f;
	render.position.y = 250.0f;
	render.position.z = 500.0f;
	render.scale = 50.0f;
	render.useTextureAsDiffuse = false;
	render.useDebugColorsNoLighting = true;
	render.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	ambientLight3->SetRender(render);

	ambientLight3->SetLight(lightAmb);
	/*lightAmb.ambient = glm::vec3(0.65f, 0.65f, 0.65f);
	lightAmb.attenConst = 0.15f;
	lightAmb.attenLinear = 3.0f;
	lightAmb.attenQuad = 3.0f;
	*/

	//gelObjectManager::sharedInstance().AddGameObject(g_ambientLight0);
	//gelObjectManager::sharedInstance().AddGameObject(ambientLight1);
	//gelObjectManager::sharedInstance().AddGameObject(ambientLight2);
	gelObjectManager::sharedInstance().AddGameObject(ambientLight3);
	
	MeshInfo particleMesh;
	particleMesh.name = "sphere";
	particleMesh.textureName_0 = "fire";
	particleMesh.usedTextureNum = 1;
	// Create a particle emitter
	cParticleEmitter *emitter = new cParticleEmitter();
	cParticleEmitter::ParticleInfo particleInfo;
	// Change stuff in the description, perhaps?
	particleInfo.lifetimeMin = 0.2f;
	particleInfo.lifetimeMax = 0.4f;
	particleInfo.initVelMin = glm::vec3(-0.07f, 0.1f, -0.07f);
	particleInfo.initVelMax = glm::vec3(0.07f, 0.15f, 0.07f);
	particleInfo.ongoingForce = glm::vec3(0.0f, 0.1f, 0.0f);
	particleInfo.isImposter = true;
	RenderInfo render0;
	render0.scale = 0.18f;
	render0.discardTextureBlack = true;
	render0.position = glm::vec3(45.0f, 1.64f, -21.0f);
	emitter->SetRender(render0);
	emitter->SetMesh(particleMesh);
	//emitter->AllocateParticles(250, particleInfo /*, true*/);

	LightInfo beaconFire;
	beaconFire.bIsEnabled = true;
	beaconFire.position = glm::vec3(render0.position.x, render0.position.y, render0.position.z);	// Likely not needed as vec3 c'tor sets to zeros
	beaconFire.diffuse = glm::vec3(1.0f, 0.01f, 0.01f);
	beaconFire.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	beaconFire.specular = glm::vec4(0.2f, 0.2f, 0.2f, 10.0f);
	beaconFire.attenConst = 0.0f;
	beaconFire.attenLinear = 0.2f;
	beaconFire.attenQuad = 0.04f;
	beaconFire.typeAndParams.x = 0;
	emitter->SetLight(beaconFire);

	//gelObjectManager::sharedInstance().AddGameObject(emitter);

	// Create a particle emitter
	cParticleEmitter *emitter2 = new cParticleEmitter();
	RenderInfo render2;
	render2.scale = 0.18f;
	render2.discardTextureBlack = true;
	render2.position = glm::vec3(45.0f, 1.64f, 21.0f);
	emitter2->SetRender(render2);
	emitter2->SetMesh(particleMesh);
	//emitter2->AllocateParticles(250, particleInfo /*, true*/);

	beaconFire.position = glm::vec3(render2.position.x, render2.position.y, render2.position.z);	// Likely not needed as vec3 c'tor sets to zeros

	emitter2->SetLight(beaconFire);
	//gelObjectManager::sharedInstance().AddGameObject(emitter2);

	// Create a particle emitter
	cParticleEmitter *emitter3 = new cParticleEmitter();
	cParticleEmitter::ParticleInfo particleInfo3;
	// Change stuff in the description, perhaps?
	particleInfo3.lifetimeMin = 0.2f;
	particleInfo3.lifetimeMax = 0.5f;
	particleInfo3.randomizeColours = true;
	RenderInfo render3;
	render3.position = glm::vec3(30.0f, 5.0f, 30.0f);
	emitter3->SetRender(render3);
	emitter3->SetMesh(particleMesh);
	//emitter3->AllocateParticles(2, particleInfo3 /*, true*/);
	//gelObjectManager::sharedInstance().AddGameObject(emitter3);


	//add battlestar galactica and little object for collision test
	iGameObject* battlestar = gelObjectFactory::sharedInstance().createGameObject("object");

	MeshInfo battlestarMesh;
	battlestarMesh.name = "Battlestar";

	RenderInfo battlestarRender;
	battlestarRender.position = glm::vec3(50.0f, 5.0f, 50.0f);
	battlestarRender.useTextureAsDiffuse = false;
	battlestarRender.scale = 100.0f;
	battlestarRender.color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);

	CollisionInfo battlestarCollision;
	battlestarCollision.contactColor = glm::vec4(1.0f, 1.0f, 0.1f, 1.0f);
	battlestarCollision.SetCollisionMode_LocalAABB("Battlestar_Collision");

	battlestar->SetRender(battlestarRender);
	battlestar->SetMesh(battlestarMesh);
	battlestar->SetCollision(battlestarCollision);

	gelObjectManager::sharedInstance().AddGameObject(battlestar);

	iGameObject* plane1 = gelObjectFactory::sharedInstance().createGameObject("object");
	iGameObject* plane2 = gelObjectFactory::sharedInstance().createGameObject("object");
	iGameObject* plane3 = gelObjectFactory::sharedInstance().createGameObject("object");

	MeshInfo planeMesh;
	planeMesh.name = "plane1";

	RenderInfo planeRender;
	planeRender.position = glm::vec3(50.0f, 5.0f, 50.0f);
	planeRender.useTextureAsDiffuse = false;
	planeRender.scale = 1.0f;
	planeRender.color = glm::vec4(0.0f, 0.1f, 1.0f, 1.0f);

	CollisionInfo planeCollision;
	planeCollision.type = CollisionInfo::TYPE::SPHERE_ARRAY;
	planeCollision.debug_mode = CollisionInfo::DEBUG_MODE::DRAW_COLLISION_OBJECT;
	std::vector<CollisionInfo::CollisionSphere> sphereArray;
	GelCollisionManager::sharedInstance().GetSphereArrayForAlias(planeMesh.name, sphereArray);
	planeCollision.SetCollisionMode_SphereArray(sphereArray);
	planeCollision.contactColor = glm::vec4(1.0f, 1.0f, 0.1f, 1.0f);

	PhysicsInfo planePhysics; //aerodynamics?
	planePhysics.enabled = true;

	planeRender.position = glm::vec3(10.0f, 5.0f, 10.0f);
	planePhysics.rotationalVelocity = glm::vec3(0.4f, 0.0f, 0.0f);

	plane1->SetRender(planeRender);
	plane1->SetMesh(planeMesh);
	plane1->SetCollision(planeCollision);
	plane1->SetPhysics(planePhysics);

	planeRender.position = glm::vec3(5.0f, 3.0f, 5.0f);
	planePhysics.rotationalVelocity = glm::vec3(0.0f, 0.4f, 0.0f);

	plane2->SetRender(planeRender);
	plane2->SetMesh(planeMesh);
	plane2->SetCollision(planeCollision);
	plane2->SetPhysics(planePhysics);

	planeRender.position = glm::vec3(15.0f, 7.0f, 15.0f);
	planePhysics.rotationalVelocity = glm::vec3(0.0f, 0.0f, 0.4f);

	plane3->SetRender(planeRender);
	plane3->SetMesh(planeMesh);
	plane3->SetCollision(planeCollision);
	plane3->SetPhysics(planePhysics);

	gelObjectManager::sharedInstance().AddGameObject(plane1);
	//gelObjectManager::sharedInstance().AddGameObject(plane2);
	//gelObjectManager::sharedInstance().AddGameObject(plane3);

	//gelSoundManager::sharedInstance().StartBGSound();

	//GENERATE GAME WORLD

	////plants and animals

	////create 10000 slot sized grid representing vector

	////then exhaustively randomize the vector and plants and animals
	////this way, none of the objects collide at start.

	////1000x1000
	//std::vector<int>coordinateVector;
	////                       i,k
	//// 100000                x,y
	//// 000100 : max coord : 99,99 which is 995,995 really)
	//// 100100
	//for (int i = 0; i < 100; i++)
	//{
	//	int idmask = 100100;
	//	idmask += i*1000;
	//	for (int k = 0; k < 100; k++)
	//	{
	//		idmask += k;
	//		//coordinateVector[k + i * 100] = idmask;
	//		coordinateVector.push_back(idmask);
	//		idmask -= k;
	//	}
	//	idmask -= i * 1000;
	//}
	//
	////improved loop
	//std::map<std::string, int> populationDictionary;
	//populationDictionary["strawberry"] = 165;
	//populationDictionary["pineapple"] = 165;
	//populationDictionary["watermelon"] = 170;

	//populationDictionary["bunny"] = 20;
	//populationDictionary["fox"] = 20;
	//populationDictionary["tiger"] = 20;

	////just the new animal and plant :)
	//populationDictionary["lettuce"] = 170;
	//populationDictionary["unicorn"] = 20;

	//typedef std::map<std::string, int>::iterator it_popul;
	//for (it_popul iterator = populationDictionary.begin(); iterator != populationDictionary.end(); iterator++)
	//{

	//	//list of triangle objects that are under the same mesh model; grouped for the same array of triangles
	//	std::string type = iterator->first;
	//	int objectCount = iterator->second;

	//	for (int i = 0; i < objectCount; i++)
	//	{
	//		//build an animal/plant
	//		iGameObject* object = gelObjectBuilder::sharedInstance().buildWorldObject(type);
	//		if (object)
	//		{
	//			int randIndex = rand() % (int)coordinateVector.size();
	//			RenderInfo render = object->GetRender();

	//			//using the known format of the mask, get the x,z coordinates
	//			int coordinateID = coordinateVector[randIndex]; //1xx1yy
	//			render.position.x = 10 * ((coordinateID / 1000) % 100) + 5.0f;
	//			render.position.z = 10 * (coordinateID % 100) + 5.0f;
	//			render.position.y = 0.0f;
	//			object->SetRender(render);

	//			gelObjectManager::sharedInstance().AddWorldObject(object);
	//			//delete the used index so that it wont place another object on top of the older one
	//			coordinateVector.erase(coordinateVector.begin() + randIndex);
	//		}
	//	}
	//}

	gelLightManager::sharedInstance().LoadLights();

	gelCameraManager::sharedInstance().AttachToGameObject(gelObjectManager::sharedInstance().GetMainPlayer());

	tickTime = 1.0f;
	tickAccumulator = 0.0f;
	//game loop
	GameLoop();

	gelSoundManager::sharedInstance().CleanUp();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void GelsenizeCore::GameLoop(void)
{
	float collisionDelta = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		glClearDepth(1.0f);
		glClearColor(0.0f, 0.41f, 0.55f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static const float MAXDELTATIME = 0.033f;	// 33 ms
		float deltaTime = this->GetDeltaTime();
		if (deltaTime > MAXDELTATIME)
		{
			deltaTime = MAXDELTATIME;
		}

		//update spotlight to follow player
		LightInfo ambi0 = g_ambientLight0->GetLight();
		//RenderInfo render_ambi0 = g_ambientLight0->GetRender();
		glm::vec3 playerPos = gelObjectManager::sharedInstance().GetMainPlayer()->GetRender().position;
		ambi0.direction = glm::vec4(playerPos - ambi0.position, 0.0f);
		//ambi0.position = GelPosition(playerPos.x, playerPos.y + 10.0f, playerPos.z);
		//render_ambi0.position = GelPosition(playerPos.x, playerPos.y + 10.0f, playerPos.z);
		g_ambientLight0->SetLight(ambi0);
		//g_ambientLight0->SetRender(render_ambi0);
		
		//apply world constraints
		if (0)
		{
			std::vector<iGameObject*>gameObjects = gelObjectManager::sharedInstance().GetGameObjects();
			for (int index = 0; index != gameObjects.size(); index++)
			{
				iGameObject* gameObject = gameObjects[index];
				if (gameObject->ObjectType() == "flock")
				{
					std::vector<iGameObject*>flockMembers;
					((cFlock*)gameObject)->GetMembers(flockMembers);
					for (int k = 0; k != flockMembers.size(); k++)
					{
						iGameObject* flockMember = flockMembers[k];
						RenderInfo render = flockMember->GetRender();
						float playgroundSize = playgroundScale;
						if (render.position.x <= 0.0f)
						{
							render.position.x = playgroundSize;
						}
						else if (render.position.x > playgroundSize)
						{
							render.position.x = 0.0f;
						}

						if (render.position.z <= 0.0f)
						{
							render.position.z = playgroundSize;
						}
						else if (render.position.z > playgroundSize)
						{
							render.position.z = 0.0f;
						}

						if (render.position.y <= 1.0f)
						{
							render.position.y = 1.0f;
						}

						flockMember->SetRender(render);
					}
				}

				if (gameObject->GetMesh().name != "plane1")
				{
					continue;
				}

				RenderInfo render = gameObject->GetRender();
				float playgroundSize = playgroundScale;
				if (render.position.x <= 0.0f)
				{
					render.position.x = playgroundSize;
				}
				else if (render.position.x > playgroundSize)
				{
					render.position.x = 0.0f;
				}

				if (render.position.z <= 0.0f)
				{
					render.position.z = playgroundSize;
				}
				else if (render.position.z > playgroundSize)
				{
					render.position.z = 0.0f;
				}

				if (render.position.y <= 1.0f)
				{
					render.position.y = 1.0f;
				}

				gameObject->SetRender(render);
			}
		}
		

		//Let entropy take action and converge
		//tickAccumulator += deltaTime;
		//while (tickAccumulator - tickTime > 0.0f)
		//{
		//	//consume 1 tick and let the mediator tick its own 'world', affecting its habitants)
		//	tickAccumulator -= tickTime;

		//	if (1)
		//	{
		//		//tick
		//		//gelWorldManager::sharedInstance().Tick(tickTime);
		//		//tock
		//		//gelWorldManager::sharedInstance().Tick(tickTime);

		//		//g_tickCount++;
		//	}
		//}
		//gelWorldManager::sharedInstance().Update(deltaTime);

		//update controls (user control)
		//update AI (gameobject control)
		gelObjectManager::sharedInstance().Update(deltaTime);
		GelCommandManager::sharedInstance().Update(deltaTime);

		collisionDelta += deltaTime;
		if (collisionDelta >= 1.0f / 120.0f)
		{
			collisionDelta -= 1.0f / 120.0f;
			
			GelCollisionManager::sharedInstance().Update(collisionDelta);
			//CollisionStep(deltaTime);


			//soon abstraction
			PhysicsStep(deltaTime);
		}

		//Camera
		gelCameraManager::sharedInstance().Update(deltaTime);

		//not used 
		//Update Light Uniforms Values
		glUniform3f(locID_lightPosition, 0.0f, 10.0f, 0.0f); //light position
		glUniform3f(locID_lightDiffuse, 1.0f, 1.0f, 1.0f); //white light

		gelLightManager::sharedInstance().Update(deltaTime); 

		//Draw Scene
		gelRenderManager::sharedInstance().Update(deltaTime);

		//Update Sounds, maybe somewhere else?
		gelSoundManager::sharedInstance().Update(deltaTime);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


//game loop above



bool GelsenizeCore::ProcessShaderFiles(void)
{
	gelShaderManager::cShader vertexShader;
	gelShaderManager::cShader fragmentShader;

	vertexShader.fileName = "resources/shaders/blinn_phongVert.glsl";
	fragmentShader.fileName = "resources/shaders/blinn_phongFrag.glsl";

	//vertexShader.fileName = "resources/shaders/Shader.vert";
	//fragmentShader.fileName = "resources/shaders/Shader.frag";
	if (!gelShaderManager::sharedInstance().createProgramFromFile("default", vertexShader, fragmentShader))
	{	// Oh no!
		std::cout << gelShaderManager::sharedInstance().getLastError();
		// TODO: Shut it all down... 
		return false;
	}
	//no error checking right now
	return true;
}

bool GelsenizeCore::ReadGameFiles_2(void)
{
	//read whole file as string 
	std::ifstream gameFile("resources/gameFile/gameFile.json");
	std::string fileContent((std::istreambuf_iterator<char>(gameFile)), std::istreambuf_iterator<char>());
	const char* fileContent_charArr = fileContent.c_str();
	rapidjson::Document document;

	document.Parse(fileContent_charArr);

	//not doing eloborate validation for the json currently, 
	if (document.HasMember("model_list"))
	{
		const rapidjson::Value& modelListValue = document["model_list"];
		if (modelListValue.IsArray())
		{
			for (rapidjson::SizeType i = 0; i < modelListValue.Size(); i++)
			{
				std::string alias = modelListValue[i]["name"].GetString();
				std::string fileName = modelListValue[i]["path"].GetString();

				bool isCollisionModel = false;
				float collisionScale = 1.0f;

				if (modelListValue[i].HasMember("generate_collision_data"))
				{
					isCollisionModel = modelListValue[i]["generate_collision_data"].GetBool();
				}
				if (modelListValue[i].HasMember("collision_scale"))
				{
					collisionScale = modelListValue[i]["collision_scale"].GetInt();
				}

				if (modelListValue[i].HasMember("collision_sphere_array"))
				{
					const rapidjson::Value& sphererArrayValueValue = modelListValue[i]["collision_sphere_array"];
					if (sphererArrayValueValue.IsArray())
					{
						std::vector< CollisionInfo::CollisionSphere > collisionSphereVector;
						for (rapidjson::SizeType k = 0; k < sphererArrayValueValue.Size(); k++)
						{
							CollisionInfo::CollisionSphere sphere;
							sphere.center_offset.x = sphererArrayValueValue[k]["offset_center"]["x"].GetFloat();
							sphere.center_offset.y = sphererArrayValueValue[k]["offset_center"]["y"].GetFloat();
							sphere.center_offset.z = sphererArrayValueValue[k]["offset_center"]["z"].GetFloat();
							sphere.radius   = sphererArrayValueValue[k]["radius"].GetFloat();
							collisionSphereVector.push_back(sphere);
						}
						GelCollisionManager::sharedInstance().SetSphereArrayForAlias(alias, collisionSphereVector);
					}
				}

				//error check later and different shader
				gelMeshManager::sharedInstance().LoadPlyIntoGLBuffer(gelShaderManager::sharedInstance().getIDWithName("default"), alias, fileName, isCollisionModel, collisionScale);
			}
		}
	}

	//get textures
	if (document.HasMember("texture_list"))
	{
		GelTextureManager::sharedInstance().SetBasePath("resources/textures");
		const rapidjson::Value& textureListValue = document["texture_list"];
		if (textureListValue.IsArray())
		{
			unsigned int loopCount = textureListValue.Size();
			if (textureCount < textureListValue.Size())
			{
				loopCount = textureCount;
			}

			for (rapidjson::SizeType i = 0; i < loopCount; i++)
			{
				std::string alias = textureListValue[i]["name"].GetString();
				std::string fileName = textureListValue[i]["path"].GetString();
				//also get shader name too mb
				//error check later and different shader
				GelTextureManager::sharedInstance().Create2DTextureFromBMPFile(alias, fileName, this->uniformLoc_texSamp2D[i], i, true);
			}
		}
	}

	if (document.HasMember("skybox_textures"))
	{
		const rapidjson::Value& skyboxValue = document["skybox_textures"];
		// Clear any gl errors
		GLenum ignoreerorr = glGetError();

		GelTextureManager::sharedInstance().SetBasePath("resources/textures/skybox");
		GLenum glErrorEnum = 0;
		std::string errorString, errorDetails;
		if (!GelTextureManager::sharedInstance().CreateNewCubeTextureFromBMPFiles("skybox",
			skyboxValue["right"].GetString(),
			skyboxValue["left"].GetString(),
			skyboxValue["top"].GetString(),
			skyboxValue["bottom"].GetString(),
			skyboxValue["front"].GetString(),
			skyboxValue["back"].GetString(),
			true, glErrorEnum,
			errorString,
			errorDetails))
		{
			std::cout << "Trouble loading skybox" << std::endl;
			std::cout << "Error: " << errorString << std::endl;
			std::cout << "Details: " << errorDetails << std::endl;
		}
	}

	int curiousCount = 0;
	int angryCount = 0;
	float averageSpeed = 0.0f;

	if (document.HasMember("project_setup"))
	{
		curiousCount = document["project_setup"]["curious_count"].GetInt();
		angryCount = document["project_setup"]["angry_count"].GetInt();
		averageSpeed = document["project_setup"]["average_speed"].GetFloat();
		this->playerSpeed = document["project_setup"]["player_speed"].GetFloat();
		this->attentionRadius = document["project_setup"]["attention_radius"].GetFloat();
		this->dangerRadius = document["project_setup"]["danger_radius"].GetFloat();

		int flockCount = document["project_setup"]["flock_count"].GetInt();;

		//flock creation

		iGameObject *flock = new cFlock(flockCount, glm::vec3(10.0f, 16.0f, 10.0f));
		RenderInfo renderFlock;
		//PhysicsInfo physicsFlock;
		//physicsFlock.enabled = true;
		renderFlock.position = glm::vec3(10.0f, 16.0f, 10.0f);
		flock->SetRender(renderFlock);
		//flock->SetPhysics(physicsFlock);
		//gelObjectManager::sharedInstance().AddGameObject(flock);

	}

	if (document.HasMember("player"))
	{
		const rapidjson::Value& playerValue = document["player"];
		std::string objectType = playerValue["object_type"].GetString();
		std::string meshName = playerValue["mesh_name"].GetString();

		MeshInfo playerMesh;
		playerMesh.name = meshName;

		RenderInfo playerRender;

		if (playerValue.HasMember("texture_name_0"))
		{
			playerMesh.textureName_0 = playerValue["texture_name_0"].GetString();
		}
		else
		{
			playerRender.useTextureAsDiffuse = false;
		}
		
		if (playerValue.HasMember("position"))
		{
			playerRender.position = GelPosition(
				playerValue["position"]["x"].GetFloat(),
				playerValue["position"]["y"].GetFloat(),
				playerValue["position"]["z"].GetFloat());
		}
		
		if (playerValue.HasMember("rotation"))
		{
			/*objectRender.rotation = GelRotation(
			playerValue["rotation"]["x"].GetFloat(),
			playerValue["rotation"]["y"].GetFloat(),
			playerValue["rotation"]["z"].GetFloat());*/
		}

		if (playerValue.HasMember("orientation"))
		{
			playerRender.orientation.x = playerValue["orientation"]["x"].GetFloat();
			playerRender.orientation.y = playerValue["orientation"]["y"].GetFloat();
			playerRender.orientation.z = playerValue["orientation"]["z"].GetFloat();
			playerRender.orientation.w = playerValue["orientation"]["w"].GetFloat();
		}


		if (playerValue.HasMember("color"))
		{
			playerRender.color = glm::vec4(
				playerValue["color"]["red"].GetFloat(),
				playerValue["color"]["green"].GetFloat(),
				playerValue["color"]["blue"].GetFloat(),
				playerValue["color"]["alpha"].GetFloat());
		}
		
		playerRender.scale = playerValue["scale"].GetFloat();
		playerRender.isWireframe = playerValue["wireframe"].GetBool();

		playerRender.isAABBDrawn = true;

		PhysicsInfo playerPhysics;
		

		if (playerValue.HasMember("physics"))
		{
			const rapidjson::Value& playerPhysicsValue = playerValue["physics"];

			playerPhysics.enabled = true;// playerPhysicsValue["enabled"].GetBool();
			playerPhysics.useSphericalBody = playerPhysicsValue["use_spherical_body"].GetBool();
			
			//playerPhysics.rotationalVelocity = glm::vec3(0.0f, 2.0f, 0.0f);
			
			if (playerPhysicsValue.HasMember("velocity"))
			{
				playerPhysics.velocity= GelPosition(
					playerPhysicsValue["velocity"]["x"].GetFloat(),
					playerPhysicsValue["velocity"]["y"].GetFloat(),
					playerPhysicsValue["velocity"]["z"].GetFloat());
			}

			if (playerPhysicsValue.HasMember("acceleration"))
			{
				playerPhysics.acceleration = GelPosition(
					playerPhysicsValue["acceleration"]["x"].GetFloat(),
					playerPhysicsValue["acceleration"]["y"].GetFloat(),
					playerPhysicsValue["acceleration"]["z"].GetFloat());
			}
		}

		LightInfo lightAmb;
		lightAmb.bIsEnabled = true;
		lightAmb.position = glm::vec3(playerRender.position.x, playerRender.position.y, playerRender.position.z);	// Likely not needed as vec3 c'tor sets to zeros
		lightAmb.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		lightAmb.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		lightAmb.specular = glm::vec4(0.7f, 0.7f, 0.7f, 100.0f);
		lightAmb.attenConst = 0.0f;
		lightAmb.attenLinear = 0.001f;
		lightAmb.attenQuad = 0.03f;

		lightAmb.typeAndParams.x = 1; //set to spotlight
		lightAmb.typeAndParams.y = 0.999f;
		lightAmb.typeAndParams.z = 0.990f;
		lightAmb.direction = glm::vec4(1.0f, -1.0f, 0.0f, 0.0f);

		//get model radius
		float model_radius = gelMeshManager::sharedInstance().LookUpMeshRadius(playerMesh.name);
		playerRender.SetRadius(model_radius);

		iGameObject* player = gelObjectFactory::sharedInstance().createGameObject(objectType);
		if (player != 0)
		{
			CollisionInfo collision;
			collision.type = CollisionInfo::TYPE::SPHERE_ARRAY;
			collision.debug_mode = CollisionInfo::DEBUG_MODE::DRAW_COLLISION_OBJECT;
			std::vector<CollisionInfo::CollisionSphere> sphereArray;
			GelCollisionManager::sharedInstance().GetSphereArrayForAlias(meshName, sphereArray);
			collision.SetCollisionMode_SphereArray(sphereArray);
			player->SetCollision(collision);

			player->SetLight(lightAmb);

			player->SetMesh(playerMesh);
			player->SetRender(playerRender);
			player->SetPhysics(playerPhysics);
			gelObjectManager::sharedInstance().SetMainPlayerID(player->getObjectID()); //1 player only atm
			gelObjectManager::sharedInstance().AddGameObject(player);
		}
	}

	if (document.HasMember("objects"))
	{
		const rapidjson::Value& objectsValue = document["objects"];
		if (objectsValue.HasMember("playground"))
		{
			const rapidjson::Value& playgroundValue = objectsValue["playground"];

			std::string objectType = playgroundValue["object_type"].GetString();
			std::string meshName = playgroundValue["mesh_name"].GetString();

			MeshInfo objectMesh;
			objectMesh.name = meshName;
			objectMesh.textureName_0 = "pebbles";
			

			//TODO: move those to a "render" or sth like physics
			RenderInfo objectRender;
			objectRender.useTextureAsDiffuse = false;
			if (playgroundValue.HasMember("position"))
			{
				objectRender.position = GelPosition(
					playgroundValue["position"]["x"].GetFloat(),
					playgroundValue["position"]["y"].GetFloat(),
					playgroundValue["position"]["z"].GetFloat());
			}

			if (playgroundValue.HasMember("texture_name_0"))
			{
				objectMesh.textureName_0 = playgroundValue["texture_name_0"].GetString();
			}

			if (playgroundValue.HasMember("rotation"))
			{
				/*objectRender.rotation = GelRotation(
				playgroundValue["rotation"]["x"].GetFloat(),
				playgroundValue["rotation"]["y"].GetFloat(),
				playgroundValue["rotation"]["z"].GetFloat());*/
			}

			if (playgroundValue.HasMember("orientation"))
			{
				objectRender.orientation.x = playgroundValue["orientation"]["x"].GetFloat();
				objectRender.orientation.y = playgroundValue["orientation"]["y"].GetFloat();
				objectRender.orientation.z = playgroundValue["orientation"]["z"].GetFloat();
				objectRender.orientation.w = playgroundValue["orientation"]["w"].GetFloat();
			}

			if (playgroundValue.HasMember("color"))
			{
				objectRender.color = glm::vec4(
					playgroundValue["color"]["red"].GetFloat(),
					playgroundValue["color"]["green"].GetFloat(),
					playgroundValue["color"]["blue"].GetFloat(),
					playgroundValue["color"]["alpha"].GetFloat());
			}

			playgroundScale = playgroundValue["scale"].GetFloat();
			objectRender.scale = playgroundScale;
			objectRender.isWireframe = playgroundValue["wireframe"].GetBool();

			//get model radius
			float model_radius = gelMeshManager::sharedInstance().LookUpMeshRadius(objectMesh.name);
			objectRender.SetRadius(model_radius);

			iGameObject* object = gelObjectFactory::sharedInstance().createGameObject(objectType);
			if (object != 0)
			{
				object->SetMesh(objectMesh);
				object->SetRender(objectRender);
				gelObjectManager::sharedInstance().AddGameObject(object);
			}

		}

		if (objectsValue.HasMember("game_object_list"))
		{
			const rapidjson::Value& objectListValue = objectsValue["game_object_list"];
			if (objectListValue.IsArray())
			{
				for (rapidjson::SizeType i = 0; i < objectListValue.Size(); i++)
				{
					const rapidjson::Value& objectValue = objectListValue[i];

					std::string objectType = objectValue["object_type"].GetString();
					std::string meshName = objectValue["mesh_name"].GetString();

					MeshInfo objectMesh;
					objectMesh.name = meshName;

					if (objectValue.HasMember("texture_name_0"))
					{
						objectMesh.textureName_0 = objectValue["texture_name_0"].GetString();
					}

					//TODO: move those to a "render" or sth like physics
					RenderInfo objectRender;
					if (objectValue.HasMember("position"))
					{
						objectRender.position = GelPosition(
							objectValue["position"]["x"].GetFloat(),
							objectValue["position"]["y"].GetFloat(),
							objectValue["position"]["z"].GetFloat());
					}

					if (objectValue.HasMember("rotation"))
					{
						/*objectRender.rotation = GelRotation(
							objectValue["rotation"]["x"].GetFloat(),
							objectValue["rotation"]["y"].GetFloat(),
							objectValue["rotation"]["z"].GetFloat());*/
					}

					if (objectValue.HasMember("orientation"))
					{
						objectRender.orientation.x = objectValue["orientation"]["x"].GetFloat();
						objectRender.orientation.y = objectValue["orientation"]["y"].GetFloat();
						objectRender.orientation.z = objectValue["orientation"]["z"].GetFloat();
						objectRender.orientation.w = objectValue["orientation"]["w"].GetFloat();
					}

					if (objectValue.HasMember("color"))
					{
						objectRender.color = glm::vec4(
							objectValue["color"]["red"].GetFloat(),
							objectValue["color"]["green"].GetFloat(),
							objectValue["color"]["blue"].GetFloat(),
							objectValue["color"]["alpha"].GetFloat());
					}

					objectRender.scale = objectValue["scale"].GetFloat();
					objectRender.isWireframe = objectValue["wireframe"].GetBool();

					PhysicsInfo objectPhysics;

					if (objectValue.HasMember("physics"))
					{
						const rapidjson::Value& objectPhysicsValue = objectValue["physics"];

						objectPhysics.enabled = objectPhysicsValue["enabled"].GetBool();
						objectPhysics.useSphericalBody = objectPhysicsValue["use_spherical_body"].GetBool();

						if (objectPhysicsValue.HasMember("velocity"))
						{
							objectPhysics.velocity = GelVelocity(
								objectPhysicsValue["velocity"]["x"].GetFloat(),
								objectPhysicsValue["velocity"]["y"].GetFloat(),
								objectPhysicsValue["velocity"]["z"].GetFloat());
						}

						if (objectPhysicsValue.HasMember("acceleration"))
						{
							objectPhysics.acceleration = GelAcceleration(
								objectPhysicsValue["acceleration"]["x"].GetFloat(),
								objectPhysicsValue["acceleration"]["y"].GetFloat(),
								objectPhysicsValue["acceleration"]["z"].GetFloat());
						}
					}

					//get model radius
					float model_radius = gelMeshManager::sharedInstance().LookUpMeshRadius(objectMesh.name);
					objectRender.SetRadius(model_radius);

					iGameObject* object = gelObjectFactory::sharedInstance().createGameObject(objectType);
					if (object != 0)
					{
						object->SetMesh(objectMesh);
						object->SetRender(objectRender);
						object->SetPhysics(objectPhysics);
						gelObjectManager::sharedInstance().AddGameObject(object);
					}
				}
			}
		}
		
		//get a camera
		cCamera *camera = new cCamera();

		if (document.HasMember("camera"))
		{
			const rapidjson::Value& cameraValue = document["camera"];
			if (cameraValue.HasMember("offset"))
			{
				glm::vec3 offset(
					cameraValue["offset"]["x"].GetFloat(),
					cameraValue["offset"]["y"].GetFloat(),
					cameraValue["offset"]["z"].GetFloat());
				camera->SetOffset(offset);
			}
		}
		gelCameraManager::sharedInstance().mainCamera = camera;

		//now create objects
		for (int i = 0; i < curiousCount; i++)
		{
			iGameObject* gameObject = gelObjectBuilder::sharedInstance().buildSeekerObject("curious", averageSpeed);

			MeshInfo tmesh;
			tmesh.name = "plane1";
			tmesh.textureName_0 = "pebbles";
			tmesh.textureName_1 = "fenceAlphaMask";
			tmesh.usedTextureNum = 2;
			gameObject->SetMesh(tmesh);

			RenderInfo trender = gameObject->GetRender();
			trender.useTextureAsDiffuse = true;
			float randX = OCHelper::getRandFloat(0.0f, this->playgroundScale);
			float randZ = OCHelper::getRandFloat(0.0f, this->playgroundScale);
			trender.position = GelPosition(randX, 5.0f, randZ);
			trender.scale = 2.0f;
			trender.alpha = 1.0f;
			gameObject->SetRender(trender);

			CollisionInfo collision;
			collision.type = CollisionInfo::TYPE::SPHERE_ARRAY;
			std::vector<CollisionInfo::CollisionSphere> sphereArray;
			GelCollisionManager::sharedInstance().GetSphereArrayForAlias(tmesh.name, sphereArray);
			collision.SetCollisionMode_SphereArray(sphereArray);
			gameObject->SetCollision(collision);

			PhysicsInfo tphysics = gameObject->GetPhysics();
			tphysics.enabled = true;
			gameObject->SetPhysics(tphysics);
			gelObjectManager::sharedInstance().AddGameObject(gameObject);
		}
		for (int i = 0; i < angryCount; i++)
		{
			iGameObject* gameObject = gelObjectBuilder::sharedInstance().buildSeekerObject("angry", averageSpeed);

			MeshInfo tmesh;
			tmesh.name = "plane1";
			tmesh.textureName_0 = "brick";
			tmesh.textureName_1 = "fenceAlphaMask";
			tmesh.usedTextureNum = 2;
			
			gameObject->SetMesh(tmesh);

			RenderInfo trender = gameObject->GetRender();
			trender.useTextureAsDiffuse = false;
			trender.position = GelPosition(OCHelper::getRandFloat(0.0f, this->playgroundScale), 5.0f, OCHelper::getRandFloat(0.0f, this->playgroundScale));
			trender.scale = 2.0f;
			trender.alpha = 1.0f;
			//trender.AABBwidth = 10.0f; test
			//trender.isAABBDrawn = true;
			gameObject->SetRender(trender);
			
			CollisionInfo collision;
			collision.type = CollisionInfo::TYPE::SPHERE_ARRAY; 
			std::vector<CollisionInfo::CollisionSphere> sphereArray;
			GelCollisionManager::sharedInstance().GetSphereArrayForAlias(tmesh.name, sphereArray);
			collision.SetCollisionMode_SphereArray(sphereArray);
			gameObject->SetCollision(collision);

			PhysicsInfo tphysics = gameObject->GetPhysics();
			tphysics.enabled = true;
			gameObject->SetPhysics(tphysics);
			gelObjectManager::sharedInstance().AddGameObject(gameObject);
		}

	}
	return true;
	//json file read
	//TODOs
	//read shader file later too, think about this when begin using multiple shaders
	
	//read assets

	//read game objects 

	//read camera

	//read player

	//read lighting

	//soon-> read textures?
}

//maybe its manager soon?
bool GelsenizeCore::ReadGameFiles(void)
{
	std::string fileName = "resources/gameFile/gameFile.txt";
	// Note: "C-style" string conversion
	std::ifstream gameFile(fileName.c_str());
	if (!gameFile.is_open())
	{
		//std::cout << "Can't open file" << std::endl;
		return false;
	}

	//sound
	int streamSoundCount = 0;
	int sampleSoundCount = 0;

	int modelCount = 0;
	int playerCount = 0;

	std::string readContent;

	//move head to stream_sound
	do {
		gameFile >> readContent;
	} while (readContent != "stream_sound");

	gameFile >> streamSoundCount;

	for (int i = 0; i < streamSoundCount; i++)
	{
		std::string soundAlias;
		std::string soundPath;
		gameFile >> soundAlias >> soundPath;

		gelSoundManager::sharedInstance().CreateStreamSound(soundAlias, soundPath);
	}

	//move head to stream_sound
	do {
		gameFile >> readContent;
	} while (readContent != "sample_sound");

	gameFile >> sampleSoundCount;

	for (int i = 0; i < sampleSoundCount; i++)
	{
		std::string soundAlias;
		std::string soundPath;
		float minDist;
		float maxDist;
		gameFile >> soundAlias >> soundPath >> minDist >> maxDist;

		gelSoundManager::sharedInstance().CreateSampleSound(soundAlias, soundPath, minDist, maxDist);
	}


	//move head to model
	do {
		gameFile >> readContent;
	} while (readContent != "model");

	gameFile >> modelCount;

	for (int i = 0; i < modelCount; i++)
	{
		std::string alias;
		std::string fileName;

		gameFile >> alias >> fileName;

		//error check later and different shader
		gelMeshManager::sharedInstance().LoadPlyIntoGLBuffer(gelShaderManager::sharedInstance().getIDWithName("default"), alias, fileName);
	}

	//move head to player
	do {
		gameFile >> readContent;
	} while (readContent != "player");

	gameFile >> playerCount;

	for (int i = 0; i < playerCount; i++)
	{
		MeshInfo mesh;
		RenderInfo render;
		PhysicsInfo physics;

		std::string str;
		gameFile >> str; //Read "player_start"

		std::string objectType = "";
		//mesh name
		gameFile >> mesh.name >> objectType;

		//position & scale
		//float sound_cd;
		gameFile >> render.position.x >> render.position.y >> render.position.z >> render.scale >> physics.useSphericalBody;// >> sound_cd;
		
		//color & wireFrame
		gameFile >> render.color.r >> render.color.g >> render.color.b >> render.color.a >> render.isWireframe;

		//rotation
		gameFile >> render.orientation.x >> render.orientation.y >> render.orientation.z; //>> render.orientation.w;
		render.orientation.w = 1;

		//velocity
		gameFile >> physics.velocity.x >> physics.velocity.y >> physics.velocity.z >> physics.enabled;

		//acceleration
		gameFile >> physics.acceleration.x >> physics.acceleration.y >> physics.acceleration.z;

		//get model radius
		float model_radius = gelMeshManager::sharedInstance().LookUpMeshRadius(mesh.name);
		render.SetRadius(model_radius);

		//mesh.soundCooldown_delta = mesh.soundCooldown;

		iGameObject* player = gelObjectFactory::sharedInstance().createGameObject(objectType);
		if (player != 0)
		{
			player->SetMesh(mesh);
			player->SetRender(render);
			player->SetPhysics(physics);
			//asdf
			gelObjectManager::sharedInstance().SetMainPlayerID(player->getObjectID()); //1 player only atm
			gelObjectManager::sharedInstance().AddGameObject(player);
		}
		gameFile >> str; //Read "player_end"
	}

	do
	{
		gameFile >> readContent;
	} while (readContent != "camera");

	//gameFile >> uselessPlyContent;

	do
	{
		gameFile >> readContent;
	} while (readContent != "object");

	int objectCount = 0;
	gameFile >> objectCount;

	for (int i = 0; i < objectCount; i++)
	{
		std::string str;
		gameFile >> str; //Read "object_start"

		MeshInfo mesh;
		RenderInfo render;
		PhysicsInfo physics;

		//render.useDebugColorsNoLighting = true;

		render.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		std::string objectType = "";

		//mesh name
		gameFile >> mesh.name >> objectType;

		//float sound_cd;
		//position & scale
		gameFile >> render.position.x >> render.position.y >> render.position.z >> render.scale >> physics.useSphericalBody;// >> sound_cd;
		
		//color & wireFrame
		gameFile >> render.color.r >> render.color.g >> render.color.b >> render.color.a >> render.isWireframe;

		//rotation
		gameFile >> render.orientation.x >> render.orientation.y >> render.orientation.z; //>> render.orientation.w;
		render.orientation.w = 1;

		//velocity
		gameFile >> physics.velocity.x >> physics.velocity.y >> physics.velocity.z >> physics.enabled;

		//acceleration
		gameFile >> physics.acceleration.x >> physics.acceleration.y >> physics.acceleration.z;

		//get model radius
		float model_radius = gelMeshManager::sharedInstance().LookUpMeshRadius(mesh.name);
		render.SetRadius(model_radius);

		//mesh.soundCooldown_delta = mesh.soundCooldown;

		//render.useDebugColorsNoLighting = false;

		iGameObject* gameObject = gelObjectFactory::sharedInstance().createGameObject(objectType);
		if (gameObject != 0)
		{
			gameObject->SetMesh(mesh);
			gameObject->SetRender(render);
			gameObject->SetPhysics(physics);
			gelObjectManager::sharedInstance().AddGameObject(gameObject);
		}

		gameFile >> str; //Read "object_end"
	}
	gameFile.close();
	return true;
}

void GelsenizeCore::LoadGameObjects(void)
{
}

float GelsenizeCore::GetDeltaTime(void)
{
	static double lastTime = 0;
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	lastTime = glfwGetTime();
	return deltaTime;
}

float getRandFloat(float LO, float HI)
{
	float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	return r3;
}

float distanceBetweenPoints(glm::vec3 A, glm::vec3 B)
{
	float deltaX = A.x - B.x;
	float deltaY = A.y - B.y;
	float deltaZ = A.z - B.z;

	return sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);

	// ***************************************************
	// Note that glm already has a function for this:
	//return glm::distance( A, B );
	// ***************************************************
}

float distanceBetweenSpheres(iGameObject* pBallA, iGameObject* pBallB)
{
	RenderInfo renderA = pBallA->GetRender();
	RenderInfo renderB = pBallB->GetRender();

	float deltaX = renderA.position.x - renderB.position.x;
	float deltaY = renderA.position.y - renderB.position.y;
	float deltaZ = renderA.position.z - renderB.position.z;

	return sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);
}

// Narrow phase 
bool testSphereSphereCollision(iGameObject* pBallA, iGameObject* pBallB)
{
	RenderInfo renderA = pBallA->GetRender();
	RenderInfo renderB = pBallB->GetRender();

	float totalRadius = renderA.GetRadius() + renderB.GetRadius();
	if (distanceBetweenSpheres(pBallA, pBallB) <= totalRadius)
	{
		return true;
	}
	return false;
}

glm::vec3 getTransformedVertex(glm::vec3 vertex, iGameObject* gameObject)
{
	RenderInfo render = gameObject->GetRender();
	//glm::vec4 transform = render.transform * glm::vec4(vertex, 0.0f);
	glm::mat4x4 transform = glm::translate(render.transform, glm::vec3(vertex.x, vertex.y, vertex.z));
	return glm::vec3(transform[3].x, transform[3].y, transform[3].z);
}

//abstraction soon
void CollisionStep(float deltaTime)
{
	// Clear the contact list toggle back colors
	for (int index = 0; index != ::g_vec_Contacts.size(); index++)
	{
		// Change colour of any balls that are touching...
		unsigned int ID = ::g_vec_Contacts[index].objectID;
		iGameObject* pCurGO = gelObjectManager::sharedInstance().FindGameObjectWithID(ID);
		if (pCurGO)
		{
			RenderInfo renderCurrent = pCurGO->GetRender();
			renderCurrent.ResetToggleColor();
			pCurGO->SetRender(renderCurrent);
		}
	}
	g_vec_Contacts.clear();

	// Brute force narrow phase detection

	std::vector<iGameObject*> gameObjects = gelObjectManager::sharedInstance().GetGameObjects();

	//lets assume there is a special object for spherical objects, we can use the useSphericalBody flag here,
	//since the "sphere" mesh is relatively close to a perfect sphere... :p

	//lets do a trick here for this time to filter out the balls and "non ball" triangle objects
	std::vector<iGameObject*> balls;
	std::vector<iGameObject*> environmentObjects; //only ground right now

	for (int index = 0; index != gameObjects.size(); index++)
	{
		iGameObject* gameObject = gameObjects[index];
		PhysicsInfo physics = gameObject->GetPhysics();
		if (physics.useSphericalBody)
		{
			//ball
			//will be subject to:
			//   - Sphere -> Sphere
			//   - Sphere -> Triangle
			balls.push_back(gameObject);
		}
		else
		{
			//triangle
			//will be subject to:
			//   - Sphere -> Triangle
			//   * Triangle -> Triangle (*:maybe not just yet)
			environmentObjects.push_back(gameObject);
		}
	}

	//make a map of all objects and sort them out by using a map,
	//then iterate through the map and get a single triangle array.
	std::map< std::string, std::vector< iGameObject* > > meshToObjectMap;

	for (int i = 0; i != environmentObjects.size(); i++)
	{
		iGameObject* triangleObject = environmentObjects[i];
		std::string meshName = triangleObject->GetMesh().name;

		//search
		std::map< std::string, std::vector< iGameObject* > >::iterator mapIterator =
			meshToObjectMap.find(meshName);

		if (mapIterator == meshToObjectMap.end())
		{
			//not found, add an array
			std::vector<iGameObject*> vecObjects;
			vecObjects.push_back(triangleObject);
			meshToObjectMap[meshName] = vecObjects;
		}
		else
		{
			//found
			std::vector<iGameObject*> vecObjects = mapIterator->second;
			vecObjects.push_back(triangleObject);
			meshToObjectMap[meshName] = vecObjects;
		}
	}

	// 1. For each ball, check for collision with other balls
	for (int indexOut = 0; indexOut != balls.size(); indexOut++)
	{
		iGameObject* pOuterObject = balls[indexOut];
		RenderInfo renderOut = pOuterObject->GetRender();

		for (int indexIn = 0; indexIn != balls.size(); indexIn++)
		{
			iGameObject* pInnerObject = balls[indexIn];
			RenderInfo renderIn = pInnerObject->GetRender();
			GelVelocity innerVelocity = pInnerObject->GetPhysics().velocity;
			// Is this the same object
			if (pOuterObject != pInnerObject)
			{
				if (testSphereSphereCollision(pOuterObject, pInnerObject))
				{
					//let the contact point be the point between these two spheres
					cContact curContact;
					curContact.objectID = pOuterObject->getObjectID();
					curContact.collisionType = cContact::WITH_SPHERE;

					//Get mid point, a simple "contact" point
					glm::vec4 contactPoint = (pOuterObject->GetRender().transform[3] + pInnerObject->GetRender().transform[3] ) * 0.5f;
					curContact.contactXYZ = glm::vec3(contactPoint.x, contactPoint.y, contactPoint.z);

					curContact.contactVelocity = glm::vec3(innerVelocity.x, innerVelocity.y, innerVelocity.z);

					//when they just touch distance is R1+R2, calculate penetration depth from that
					float zeroDepthCollisionDistance = pOuterObject->GetRender().GetRadius() + pInnerObject->GetRender().GetRadius();
					curContact.penetrationDepth = zeroDepthCollisionDistance - distanceBetweenSpheres(pOuterObject, pInnerObject);

					//calculate normal
					glm::vec3 contactNormal(
						renderOut.position.x - renderIn.position.x,
						renderOut.position.y - renderIn.position.y,
						renderOut.position.z - renderIn.position.z);
					contactNormal = glm::normalize(contactNormal);
					curContact.normal = contactNormal;

					::g_vec_Contacts.push_back(curContact);
				}
			}
		}
	}

	// 2. For each ball, check for collision with triangles (environment)
	for (int indexOut = 0; indexOut != balls.size(); indexOut++)
	{
		iGameObject* ball = balls[indexOut];
		RenderInfo ballRender = ball->GetRender();

		//http://stackoverflow.com/questions/4844886/how-to-loop-through-a-c-map
		//loop through map,   ("bunny") ->  (gameObjects that have "bunny" as meshName)
		typedef std::map< std::string, std::vector< iGameObject* > >::iterator it_type;
		for (it_type iterator = meshToObjectMap.begin(); iterator != meshToObjectMap.end(); iterator++) {

			//list of triangle objects that are under the same mesh model; grouped for the same array of triangles
			std::string meshName = iterator->first;
			std::vector<iGameObject*> vecObjects = iterator->second;

			//get triangle array for the model
			std::vector<GelTriangle> meshTriangles;
			if (!gelMeshManager::sharedInstance().GetTriangleArrayForMesh(meshName, meshTriangles))
			{
				//got false
				continue;//just for this model.
			}

			//for each triangle of the triangle_array, check collision for each object
			for (int triangleIndex = 0; triangleIndex != meshTriangles.size(); triangleIndex++)
			{
				GelTriangle triangle = meshTriangles[triangleIndex];

				for (int index = 0; index != vecObjects.size(); index++)
				{
					iGameObject* gameObject = vecObjects[index];
					RenderInfo render = gameObject->GetRender();

					glm::vec3 transformedVertex1 = getTransformedVertex(triangle.v1, gameObject);
					glm::vec3 transformedVertex2 = getTransformedVertex(triangle.v2, gameObject);
					glm::vec3 transformedVertex3 = getTransformedVertex(triangle.v3, gameObject);

					glm::vec3 ballCentre(
						ballRender.position.x,
						ballRender.position.y,
						ballRender.position.z);

					//get the closest point of the ball centre, out of the 3 transformed vertices
					/*glm::vec3 closestPoint = ClosestPtPointTriangle(
						ballCentre,
						transformedVertex1,
						transformedVertex2,
						transformedVertex3);*/
					glm::vec3 closestPoint;
						//DONT FORGET UNCOMMENT ABOVE IF YOU WISH TO USE THIS AGAIN

					//check distance between ball and closest point to see if there is a penetration
					float distanceToCenterOfClosestPoint = distanceBetweenPoints(ballCentre, closestPoint);
					float radius = ball->GetRender().GetRadius();

					if (distanceToCenterOfClosestPoint <= radius)
					{
						//found a contact
						cContact curContact;
						curContact.objectID = ball->getObjectID();
						curContact.collisionType = cContact::WITH_TRIANGLE;
						curContact.contactXYZ = closestPoint;
						curContact.penetrationDepth = radius - distanceToCenterOfClosestPoint;

						//calculate normal
						curContact.normal = glm::normalize(ballCentre - closestPoint);

						::g_vec_Contacts.push_back(curContact);
					}
				}// for each object of a mesh name group
			}// for each triangle
		}// for each available mesh name
	}// for each ball

	// Calculate reponse....
	if (!::g_vec_Contacts.empty())
	{
		//make a new map
		std::map< unsigned int, std::vector< cContact > > idToContactVecMap;
		for (int i = 0; i != ::g_vec_Contacts.size(); i++)
		{
			cContact contact= ::g_vec_Contacts[i];
			
			//search
			std::map< unsigned int, std::vector< cContact > >::iterator mapIterator =
				idToContactVecMap.find(contact.objectID);

			if (mapIterator == idToContactVecMap.end())
			{
				//not found, add an array
				std::vector<cContact> vecObjects;
				vecObjects.push_back(contact);
				idToContactVecMap[contact.objectID] = vecObjects;
			}
			else
			{
				//found
				std::vector<cContact> vecObjects = mapIterator->second;
				vecObjects.push_back(contact);
				idToContactVecMap[contact.objectID] = vecObjects;
			}
		}

		//loop through map,   ("bunny") ->  (gameObjects that have "bunny" as meshName)
		typedef std::map< unsigned int, std::vector< cContact > >::iterator it_contact;
		for (it_contact iterator = idToContactVecMap.begin(); iterator != idToContactVecMap.end(); iterator++) {
			unsigned int ID = iterator->first;
			std::vector<cContact> contactsOfID = iterator->second;

			iGameObject* pCurGO = gelObjectManager::sharedInstance().FindGameObjectWithID(ID);
			if (pCurGO)
			{
				RenderInfo renderCurrent = pCurGO->GetRender();
				PhysicsInfo physicsCurrent = pCurGO->GetPhysics();
				renderCurrent.ToggleColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

				bool triangleReflection = false;
				bool sphereReflection = false;

				for (int index = 0; index != contactsOfID.size(); index++)
				{
					//for each contact
					cContact contact = contactsOfID[index];
					if (contact.collisionType == cContact::WITH_SPHERE)
					{
						
						glm::vec3 velocityVec;
						velocityVec = glm::vec3(physicsCurrent.velocity.x, physicsCurrent.velocity.y, physicsCurrent.velocity.z);
						glm::vec3 reflectedVelocity;
						reflectedVelocity = glm::reflect(velocityVec, contact.normal);

						//lose some energy while reflecting
						//half of it goes to other sphere, half of it reflects.
						physicsCurrent.velocity.x = reflectedVelocity.x * 0.5f + contact.contactVelocity.x * 0.5f;
						physicsCurrent.velocity.y = reflectedVelocity.y * 0.5f + contact.contactVelocity.y * 0.5f;
						physicsCurrent.velocity.z = reflectedVelocity.z * 0.5f + contact.contactVelocity.z * 0.5f;

						renderCurrent.position.x += reflectedVelocity.x * deltaTime * 0.9f;
						renderCurrent.position.y += reflectedVelocity.y * deltaTime * 0.9f;
						renderCurrent.position.z += reflectedVelocity.z * deltaTime * 0.9f;
					}
					else if (contact.collisionType == cContact::WITH_TRIANGLE)
					{
						//only allowed to reflect once in each cycle with a triangle
						if (triangleReflection)
						{
							continue;
						}
						triangleReflection = true;

						glm::vec3 velocityVec;
						velocityVec = glm::vec3(physicsCurrent.velocity.x, physicsCurrent.velocity.y, physicsCurrent.velocity.z);
						glm::vec3 reflectedVelocity;
						reflectedVelocity = glm::reflect(velocityVec, contact.normal);

						//lose some energy while reflecting
						physicsCurrent.velocity.x = reflectedVelocity.x * 0.9f;
						physicsCurrent.velocity.y = reflectedVelocity.y * 0.9f;
						physicsCurrent.velocity.z = reflectedVelocity.z * 0.9f;

						renderCurrent.position.x += reflectedVelocity.x * deltaTime * 0.9f;
						renderCurrent.position.y += reflectedVelocity.y * deltaTime * 0.9f;
						renderCurrent.position.z += reflectedVelocity.z * deltaTime * 0.9f;
					}
				}

				pCurGO->SetRender(renderCurrent);
				pCurGO->SetPhysics(physicsCurrent);
			}
		}
	}
	return;
}
//
void PhysicsStep(float deltaTime)		// 24 Hz  30 Hz  60 Hz  100 Hz 60Hz  50,000 fps
{
	std::vector<iGameObject*> gameObjects = gelObjectManager::sharedInstance().GetGameObjects();

	glm::vec3 gravityForce(0.0f, 0.0f, 0.0f);


	// Euler... 
	for (int index = 0; index != gameObjects.size(); index++)
	{
		// f = m*a
		// Velocity = 1 m per second
		// x = x + velocity*time
		// v = v + accel*time

		// each second 
		iGameObject* pCurGO = gameObjects[index];

		if (pCurGO->ObjectType() == "flock")
		{
			std::vector<iGameObject*> flockMembers;
			((cFlock*)pCurGO)->GetMembers(flockMembers);
			for (int k = 0; k != flockMembers.size(); k++)
			{
				iGameObject* member = flockMembers[k];

				RenderInfo renderCurrent = member->GetRender();
				PhysicsInfo physicsCurrent = member->GetPhysics();

				physicsCurrent.velocity.x += (physicsCurrent.acceleration.x + gravityForce.x) * deltaTime;
				physicsCurrent.velocity.y += (physicsCurrent.acceleration.y + gravityForce.y) * deltaTime;
				physicsCurrent.velocity.z += (physicsCurrent.acceleration.z + gravityForce.z) * deltaTime;

				renderCurrent.position.x += physicsCurrent.velocity.x * deltaTime;
				renderCurrent.position.y += physicsCurrent.velocity.y * deltaTime;
				renderCurrent.position.z += physicsCurrent.velocity.z * deltaTime;

				// Represents the change over 1 second.
				glm::quat qRotChangeZeroSeconds;		// No rotational change
				glm::quat qRotChange(physicsCurrent.rotationalVelocity);

				glm::quat qRotChangeThisFrame = glm::slerp(qRotChangeZeroSeconds,
					qRotChange,
					deltaTime);

				renderCurrent.orientation *= qRotChangeThisFrame;

				member->SetRender(renderCurrent);
				member->SetPhysics(physicsCurrent);
			}
			continue;
		}

		RenderInfo renderCurrent = pCurGO->GetRender();
		PhysicsInfo physicsCurrent = pCurGO->GetPhysics();

		if (physicsCurrent.enabled)
		{

			physicsCurrent.velocity.x += (physicsCurrent.acceleration.x + gravityForce.x) * deltaTime;
			physicsCurrent.velocity.y += (physicsCurrent.acceleration.y + gravityForce.y) * deltaTime;
			physicsCurrent.velocity.z += (physicsCurrent.acceleration.z + gravityForce.z) * deltaTime;

			renderCurrent.position.x += physicsCurrent.velocity.x * deltaTime;
			renderCurrent.position.y += physicsCurrent.velocity.y * deltaTime;
			renderCurrent.position.z += physicsCurrent.velocity.z * deltaTime;

			// Represents the change over 1 second.
			glm::quat qRotChangeZeroSeconds;		// No rotational change
			glm::quat qRotChange(physicsCurrent.rotationalVelocity);

			glm::quat qRotChangeThisFrame = glm::slerp(qRotChangeZeroSeconds,
				qRotChange,
				deltaTime);

			renderCurrent.orientation *= qRotChangeThisFrame;

			pCurGO->SetRender(renderCurrent);
			pCurGO->SetPhysics(physicsCurrent);

		}//if ( pCurShip->bIsUpdatedByPhysics )
	}

	std::vector<cContact> contactVec;
	GelCollisionManager::sharedInstance().GetContacts(contactVec);

	for (int i = 0; i != contactVec.size(); i++)
	{
		cContact contact = contactVec[i];
		iGameObject* contactObject = gelObjectManager::sharedInstance().FindGameObjectWithID(contact.objectID);
		PhysicsInfo physics = contactObject->GetPhysics();
		RenderInfo render = contactObject->GetRender();

		//stop acceleration
		physics.acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

		//reverse and clamp velocity
		//physics.velocity *= -0.85f;
		
		//or even better
		physics.velocity = glm::reflect(physics.velocity, contact.normal);

		//the after collision behaviour you mentioned at the class
		physics.rotationalVelocity = glm::vec3(getRandFloat(-1.8f, 1.8f), getRandFloat(-1.8f, 1.8f), getRandFloat(-1.8f, 1.8f));

		render.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		contactObject->SetPhysics(physics);
		contactObject->SetRender(render);
	}

	return;
}
