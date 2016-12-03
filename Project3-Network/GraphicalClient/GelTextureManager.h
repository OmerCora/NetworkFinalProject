#ifndef _cBasicTextureManager_HG_
#define _cBasicTextureManager_HG_

#include <string>
#include <map>
#include "CTextureFromBMP.h"
#include "iManager.h"

class GelTextureManager : public iManager
{
public:
	//Singleton 
	static GelTextureManager& sharedInstance()
	{
		static GelTextureManager instance;
		return instance;
	}

	virtual void Update(float deltaTime);
	void Setup(void);

	bool Create2DTextureFromBMPFile(std::string alias, std::string textureFileName, GLuint uniformNum, GLuint unit, bool bGenerateMIPMap );
	bool CreateNewCubeTextureFromBMPFiles(std::string cubeMapName,
		std::string posX_fileName, std::string negX_fileName,
		std::string posY_fileName, std::string negY_fileName,
		std::string posZ_fileName, std::string negZ_fileName,
		/*GLenum textureUnit,*/ bool bIsSeamless,
		GLenum &errorEnum, std::string &errorString, std::string &errorDetails);

	// returns 0 on error
	GLuint GetTextureIDFromName( std::string alias );

	void SetBasePath(std::string basepath);

	void BindTexture(std::string alias, GLuint unit);

private:
	GelTextureManager() {};
	GelTextureManager(GelTextureManager const&); //dont implement - to build
	void operator=(GelTextureManager const&); //dont implement - to build


	struct TextureInfo
	{
		unsigned int textureUnit;
		unsigned int uniformID;
		CTextureFromBMP* texture;
	};

	std::string m_basePath;
	std::string m_lastError;
	void m_appendErrorString( std::string nextErrorText );
	void m_appendErrorStringLine( std::string nextErrorTextLine );

	std::map< std::string, TextureInfo > m_map_TexNameToTexture;


};

#endif
