#ifndef _GELSENIZEMANAGER_HG_
#define _GELSENIZEMANAGER_HG_

#include "externals.h"
#include <string>

//main game loop manager
class GelsenizeCore
{
public:
	//Singleton
	static GelsenizeCore& sharedInstance()
	{
		static GelsenizeCore instance;
		return instance;
	}

	float GetPlaygroundScale(void);
	float GetPlayerSpeed(void);
	float GetAttentionRadius(void);
	float GetDangerRadius(void);

	//TODO: awaiting abstraction
	GLFWwindow* GetWindow(void);
	GLuint GetModelLocation(void);
	GLuint GetViewLocation(void);
	GLuint GetProjLocation(void);

	GLuint ObjectAlphaID(void);
	GLuint DiscardTextureBlackID(void);
	GLuint TexSamp2DID(int index);
	GLuint UsedTexNumID(void);
	GLuint CubemapSkyBoxSamplerID(void);
	GLuint IsCubeMapObjectID(void);
	GLuint UseLightingID(void);
	GLuint IsImposterBlackIsAlphaID(void);
	GLuint UseTextureAsDiffuseID(void);
	GLuint AlphaID(void);
	GLuint ModelColorID(void);
	GLuint isWireframeID(void);
	GLuint UseGlobalAlphaValue(void);
	GLuint GlobalAlphaValue(void);
	GLuint DebugColorID(void);
	GLuint EyeLocationID(void);

	void StartGameLoop(void);
	//~GelsenizeManager() {}
private:
	GelsenizeCore() {};
	GelsenizeCore(GelsenizeCore const&); //dont implement - to build
	void operator=(GelsenizeCore const&); //dont implement - to build
	
	bool ProcessShaderFiles(void);
	bool ReadGameFiles(void);
	bool ReadGameFiles_2(void); //json
	void LoadGameObjects(void);

	void GameLoop(void);

	//playground
	float playgroundScale;
	float playerSpeed;
	float attentionRadius;
	float dangerRadius;

	//accumulates ticks
	float tickAccumulator;
	float GetDeltaTime(void);
	float tickTime;

	//?>stay
	GLFWwindow* window;

	GLuint locID_matModel; 
	GLuint locID_matView;
	GLuint locID_matProj;
	GLuint locID_lightPosition;
	GLuint locID_lightDiffuse;

	GLuint uniformId_ModelColor;// = 0;
	GLuint uniformId_isWireframe;// = 0;
	GLuint uniformId_userDebugColorsNoLighting;// = 0;
	GLuint uniformId_eyeLocation;// = 0;
	GLuint uniformId_alpha;
	GLuint uniformId_use_alpha;
	GLuint uniformId_object_alpha;

	//texture
	unsigned int textureCount = 4;
	GLint uniformLoc_texSamp2D[4] = {-1,-1,-1,-1};

	GLuint uniformLoc_bUseTextureAsDiffuse = -1;
	GLuint uniformLoc_bUseLighting = -1;
	GLuint uniformLoc_bIsDiscardTextureBlack = -1;

	GLuint uniformLoc_bIsImposterBlackIsAlpha = -1;

	GLuint uniformLoc_bIsCubeMapObject = -1;
	GLuint uniformLoc_myCubemapSkyBoxSampler = -1;
	GLuint uniformLoc_usedTexNum = -1;
};
#endif // !_GELSENIZEMANAGER_HG_

