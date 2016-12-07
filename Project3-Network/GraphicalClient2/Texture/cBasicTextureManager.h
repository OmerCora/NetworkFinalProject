#ifndef _cBasicTextureManager_HG_
#define _cBasicTextureManager_HG_

#include <string>
#include <map>
#include "CTextureFromBMP.h"

class cBasicTextureManager
{
public:
	bool Create2DTextureFromBMPFile(std::string textureFileName, bool bGenerateMIPMap);
	bool CreateCubeTextureFromBMPFile( std::string textureFileName );
	bool CreateNewCubeTextureFromBMPFiles(std::string cubeMapName,
										std::string posX_fileName, std::string negX_fileName,
										std::string posY_fileName, std::string negY_fileName,
										std::string posZ_fileName, std::string negZ_fileName,
										/*GLenum textureUnit,*/ bool bIsSeamless,
										GLenum &errorEnum, std::string &errorString, std::string &errorDetails);


	// returns 0 on error
	GLuint getTextureIDFromName( std::string textureFileName );

	void SetBasePath(std::string basepath);

private:
	std::string m_basePath;
	std::string m_lastError;
	void m_appendErrorString( std::string nextErrorText );
	void m_appendErrorStringLine( std::string nextErrorTextLine );

	std::map< std::string, CTextureFromBMP* > m_map_TexNameToTexture;


};

#endif
