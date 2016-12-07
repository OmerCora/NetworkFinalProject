#ifndef _GlobalStuff_HG_
#define _GlobalStuff_HG_

#include <glad/glad.h>		
#include <GLFW/glfw3.h>	
#include <glm/glm.hpp>

#include "cMeshTypeManager.h"	
#include "cShaderManager.h"
#include "Texture/cBasicTextureManager.h"	
#include "cValueReader.h"
#include "cCamera.h"
#include "cEntity.h"
#include "cEnemy.h"
#include "cPlayer.h"
#include "cAreaInfo.h"

#include <iostream>
#include <vector>

extern bool g_bMonopolyProceed;

extern GLFWwindow* gWindow;
extern float gWindowRatio;

extern cPlayer* gPlayer;
extern cEnemy* gEnemy;
extern std::vector<cEntity*>gVec_Entities;
extern cCamera* gCamera;
extern cAreaInfo* gAreaInfo;
//extern cPathNodeGrid* gPathNodeGrid;

extern std::vector<cEntity*>gVec_Blocks;

//will be used for collision detection with obstacles
//maxZ value because they are moving only forward
extern float gPlayerMaxZ;
extern float gEnemyYtype1MaxZ;
extern float gEnemyYtype2MaxZ;

extern void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
extern void mouseposcallback(GLFWwindow* window, double posX, double posY);
//extern void mouseScrollcallback(GLFWwindow* window, double xoffset, double yoffset);


// We should place this into a class...
extern float g_CameraX;
extern float g_CameraY;
extern float g_CameraZ;	
extern float g_CameraMovementSpeed;

extern float g_TargetX;
extern float g_TargetY;
extern float g_TargetZ;

extern GLuint shadProgID;

extern glm::vec3 lightPos;		
extern float lightConstAtten;
extern float lightLinearAtten;
extern float lightQuadAtten;

extern GLint locID_matModel;		
extern GLint locID_matView;
extern GLint locID_matProj;

extern GLint UniformLoc_ID_objectColour;
extern GLint UniformLoc_ID_isWireframe;
extern GLint UniformLoc_ID_bUseObjColorNoTexture;

extern GLint UniformLoc_alpha;

extern GLint UniformLoc_texSamp2D_00;
extern GLint UniformLoc_texSamp2D_01;
extern GLint UniformLoc_texSamp2D_02;
extern GLint UniformLoc_texSamp2D_03;
extern GLint UniformLoc_texSampCube_SkyBox;
extern GLint UniformLoc_bUseTextureAsDiffuse;
extern GLint UniformLoc_bUseLighting;

extern GLint UniformLoc_bIsImposterBlack;

/********** for cube box implementation ************/
extern cEntity* g_pSkyBoxObject;
extern GLint UniformLoc_bIsCubeMapObject;
extern GLint UniformLoc_myCubemapSkyBoxSampler;


//extern std::vector< cGameObject* > g_vec_pGOs;

extern cMeshTypeManager* g_pTheMeshTypeManager;

extern cShaderManager* g_pTheShaderManager;

extern cBasicTextureManager* g_pTextureManager;

extern cValueReader* g_pValueReader;


extern GLint locID_lightPosition[12];
extern GLint locID_lightDiffuse[12];
extern GLint locID_lightAttenuation[12];

extern void LightShaderInit();
extern void LightSetting();

 
void DrawObject( cEntity* pCurrEntity, bool bUseLighting = true );

void DrawSkyBox(void);



bool GLErrorCheck(const char* codeFile, int line, const char* msg);
#define dGLErrorCheck(additionalMsg) GLErrorCheck(__FILE__, __LINE__, additionalMsg)




#endif
