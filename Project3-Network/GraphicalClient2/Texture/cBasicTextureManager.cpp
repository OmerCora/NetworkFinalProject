#include "cBasicTextureManager.h"

#include <sstream>

void cBasicTextureManager::SetBasePath(std::string basepath)
{
	this->m_basePath = basepath;
	return;
}


bool cBasicTextureManager::Create2DTextureFromBMPFile( std::string textureFileName, bool bGenerateMIPMap )
{
	std::string fileToLoadFullPath = this->m_basePath + "/" + textureFileName;


	CTextureFromBMP* pTempTexture = new CTextureFromBMP();
	if ( ! pTempTexture->CreateNewTextureFromBMPFile2( textureFileName, fileToLoadFullPath, /*textureUnit,*/ bGenerateMIPMap ) )
	{
		this->m_appendErrorString( "Can't load " );
		this->m_appendErrorString( fileToLoadFullPath );
		this->m_appendErrorString( "\n" );
		return false;
	}

	// Texture is loaded OK
	//this->m_nextTextureUnitOffset++;
	
	this->m_map_TexNameToTexture[ textureFileName ] = pTempTexture;

	return true;
}

bool cBasicTextureManager::CreateCubeTextureFromBMPFile(std::string textureFileName)
{
	std::string fileToLoadFullPath = this->m_basePath + "/" + textureFileName;


	CTextureFromBMP* pTempTexture = new CTextureFromBMP();
	if (!pTempTexture->CreateNewCubeTextureFromBMPFiles(textureFileName, fileToLoadFullPath))
	{
		this->m_appendErrorString("Can't load ");
		this->m_appendErrorString(fileToLoadFullPath);
		this->m_appendErrorString("\n");
		return false;
	}

	// Texture is loaded OK
	//this->m_nextTextureUnitOffset++;

	this->m_map_TexNameToTexture[textureFileName] = pTempTexture;

	return true;
}


void cBasicTextureManager::m_appendErrorString( std::string nextErrorText )
{
	std::stringstream ss;
	ss << this->m_lastError << nextErrorText;
	this->m_lastError = ss.str();
	return;
}

GLuint cBasicTextureManager::getTextureIDFromName( std::string textureFileName )
{
	std::map< std::string, CTextureFromBMP* >::iterator itTexture
		= this->m_map_TexNameToTexture.find( textureFileName );
	// Found it?
	if ( itTexture == this->m_map_TexNameToTexture.end() )
	{
		return 0;
	}
	// Reutrn texture number (from OpenGL genTexture)
	return itTexture->second->getTextureNumber();
}


void cBasicTextureManager::m_appendErrorStringLine( std::string nextErrorTextLine )
{
	std::stringstream ss;
	ss << this->m_lastError << std::endl;
	ss << nextErrorTextLine << std::endl;
	this->m_lastError = ss.str();
	return;
}


/********** for cube box implementation ************/
bool cBasicTextureManager::CreateNewCubeTextureFromBMPFiles(std::string cubeMapName,
	std::string posX_fileName, std::string negX_fileName,
	std::string posY_fileName, std::string negY_fileName,
	std::string posZ_fileName, std::string negZ_fileName,
	/*GLenum textureUnit,*/ bool bIsSeamless,
	GLenum &errorEnum, std::string &errorString, std::string &errorDetails)
{
	// Append the full path to this...

	posX_fileName = this->m_basePath + '/' + posX_fileName;
	negX_fileName = this->m_basePath + '/' + negX_fileName;
	posY_fileName = this->m_basePath + '/' + posY_fileName;
	negY_fileName = this->m_basePath + '/' + negY_fileName;
	posZ_fileName = this->m_basePath + '/' + posZ_fileName;
	negZ_fileName = this->m_basePath + '/' + negZ_fileName;


	CTextureFromBMP* pTempTexture = new CTextureFromBMP();
	if (!pTempTexture->CreateNewCubeTextureFromBMPFiles(cubeMapName,
		posX_fileName, negX_fileName,
		posY_fileName, negY_fileName,
		posZ_fileName, negZ_fileName,
		true, errorEnum, errorString, errorDetails))
	{
		this->m_appendErrorString("Can't load ");
		this->m_appendErrorString(cubeMapName);
		this->m_appendErrorString("\n");
		return false;
	}

	return true;
}
