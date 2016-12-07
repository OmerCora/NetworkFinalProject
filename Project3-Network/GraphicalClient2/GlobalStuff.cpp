#include "GlobalStuff.h"

bool g_bMonopolyProceed = true;

GLFWwindow* gWindow = 0;

float gWindowRatio = 1.0f;

cCamera* gCamera = 0;
cPlayer* gPlayer = 0;
cEnemy* gEnemy = 0;
std::vector<cEntity*>gVec_Entities;
std::vector<cEntity*>gVec_Blocks;

float gPlayerMaxZ = 0;
float gEnemyYtype1MaxZ = 0;
float gEnemyYtype2MaxZ = 0;

cAreaInfo* gAreaInfo = 0;

//cPathNodeGrid* gPathNodeGrid = 0;

glm::vec3 gVelocity = glm::vec3(0.0, 0.0, 0.0);
bool bCollision = false;

float g_CameraX = 0.0f;
float g_CameraY = 19.2f;
float g_CameraZ = 0.0f;
float g_CameraMovementSpeed = 0.1f;

float g_TargetX = 0.0f;
float g_TargetY = 0.0f;
float g_TargetZ = 0.0f;


GLuint shadProgID = 0;
GLint locID_lightPosition[12];
GLint locID_lightDiffuse[12];
GLint locID_lightAttenuation[12];


glm::vec3 lightPos(0.0f, 8.0f, 0.0f);		
float lightConstAtten = 0.0f;
float lightLinearAtten = 0.02f;
float lightQuadAtten = 0.02;
//bool bDrawLightDebugSpheres = false;
//bool bDrawLightDebugLocation = false;
//bool bDebugLocationBox = false;

GLint locID_matModel = -1;		
GLint locID_matView = -1;
GLint locID_matProj = -1;

GLint UniformLoc_ID_objectColour = -1;
GLint UniformLoc_ID_isWireframe = -1;
GLint UniformLoc_ID_bUseObjColorNoTexture = -1;

GLint UniformLoc_alpha = -1;

GLint UniformLoc_texSamp2D_00 = -1;
GLint UniformLoc_texSamp2D_01 = -1;
GLint UniformLoc_texSamp2D_02 = -1;
GLint UniformLoc_texSamp2D_03 = -1;
GLint UniformLoc_texSampCube_SkyBox = -1;

GLint UniformLoc_bUseTextureAsDiffuse = -1;
GLint UniformLoc_bUseLighting = -1;

GLint UniformLoc_bIsImposterBlack = -1;


//std::vector< cGameObject* > g_vec_pGOs;

cMeshTypeManager* g_pTheMeshTypeManager = 0;

cShaderManager* g_pTheShaderManager = 0;

cBasicTextureManager* g_pTextureManager = 0;

cValueReader* g_pValueReader = 0;

/********** for cube box(skybox) implementation ************/
cEntity* g_pSkyBoxObject = 0;
GLint UniformLoc_bIsCubeMapObject = -1;
GLint UniformLoc_myCubemapSkyBoxSampler = -1;



bool GLErrorCheck(const char* codeFile, int line, const char* msg)
{
	GLenum validid = glGetError();

	//switch (validid)
	{
		if ((validid & GL_INVALID_ENUM) == GL_INVALID_ENUM)
		{
			std::cout << "GLERROR: " << msg << std::endl;
			std::cout << "\t code: " << codeFile << std::endl;
			std::cout << "\t line: " << line << std::endl;
			const char* errMsg = "\t GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.The offending command is ignored and has no other side effect than to set the error flag.";
			std::cout << errMsg << std::endl;
		}
		if ((validid & GL_INVALID_VALUE) == GL_INVALID_VALUE)
		{
			const char* errMsg = "\t GL_INVALID_VALUE: A numeric argument is out of range.The offending command is ignored and has no other side effect than to set the error flag.";
			std::cout << errMsg << std::endl;
		}

		if ((validid & GL_INVALID_OPERATION) == GL_INVALID_OPERATION)
		{
			const char* errMsg = "\t GL_INVALID_OPERATION: The specified operation is not allowed in the current state.The offending command is ignored and has no other side effect than to set the error flag.";
			std::cout << errMsg << std::endl;
		}

		if ((validid & GL_INVALID_FRAMEBUFFER_OPERATION) == GL_INVALID_FRAMEBUFFER_OPERATION)
		{
			const char* errMsg = "\t GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.The offending command is ignored and has no other side effect than to set the error flag.";
			std::cout << errMsg << std::endl;
		}

		if ((validid & GL_OUT_OF_MEMORY) == GL_OUT_OF_MEMORY)
		{
			const char* errMsg = "\t GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
			std::cout << errMsg << std::endl;
		}

		if ((validid & GL_STACK_UNDERFLOW) == GL_STACK_UNDERFLOW)
		{
			const char* errMsg = "\t GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.";
			std::cout << errMsg << std::endl;
		}

		if ((validid & GL_STACK_OVERFLOW) == GL_STACK_OVERFLOW)
		{
			const char* errMsg = "\t GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.";
			std::cout << errMsg << std::endl;
		}

	}

	return (validid != GL_NO_ERROR);
}
