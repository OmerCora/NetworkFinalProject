#include "GelTextureManager.h"

#include <sstream>
#include "GelTextureManager.h"
#include "GelsenizeCore.h"

void GelTextureManager::SetBasePath(std::string basepath)
{
	this->m_basePath = basepath;
	return;
}

void GelTextureManager::BindTexture(std::string alias, GLuint unit)
{
	std::map< std::string, TextureInfo >::iterator itTexture
		= this->m_map_TexNameToTexture.find(alias);
	// Found it?
	if (itTexture == this->m_map_TexNameToTexture.end())
	{
		//or bind a default texture
		return;
	}

	TextureInfo info = this->m_map_TexNameToTexture[alias];

	GLuint textureUnit = unit;
	GLuint textureNumber = info.texture->getTextureNumber();
	glActiveTexture(textureUnit + GL_TEXTURE0);		// GL_TEXTURE0 = 33984
	glBindTexture(GL_TEXTURE_2D, textureNumber);
	////	glBindTextureUnit( 1, texture01Number );			// OpenGL 4.5+ only
	//	// Set texture unit in the shader, too
	//glUniform1i(info.uniformID, textureUnit);
	glUniform1i(GelsenizeCore::sharedInstance().TexSamp2DID(textureUnit), textureUnit);
}


void GelTextureManager::Update(float deltaTime)
{
}

void GelTextureManager::Setup(void)
{
}

bool GelTextureManager::Create2DTextureFromBMPFile(std::string alias, std::string textureFileName, GLuint uniformNum, GLuint unit, bool bGenerateMIPMap )
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
	
	TextureInfo info;
	info.texture = pTempTexture;
	info.uniformID = uniformNum;
	info.textureUnit = unit;

	this->m_map_TexNameToTexture[alias] = info;

	return true;
}

bool GelTextureManager::CreateNewCubeTextureFromBMPFiles(std::string cubeMapName, std::string posX_fileName, std::string negX_fileName, std::string posY_fileName, std::string negY_fileName, std::string posZ_fileName, std::string negZ_fileName, bool bIsSeamless, GLenum & errorEnum, std::string & errorString, std::string & errorDetails)
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


void GelTextureManager::m_appendErrorString( std::string nextErrorText )
{
	std::stringstream ss;
	ss << this->m_lastError << nextErrorText;
	this->m_lastError = ss.str();
	return;
}

GLuint GelTextureManager::GetTextureIDFromName( std::string alias )
{
	std::map< std::string, TextureInfo >::iterator itTexture
		= this->m_map_TexNameToTexture.find(alias);
	// Found it?
	if ( itTexture == this->m_map_TexNameToTexture.end() )
	{
		return 0;
	}
	// Reutrn texture number (from OpenGL genTexture)
	return itTexture->second.texture->getTextureNumber();
}


void GelTextureManager::m_appendErrorStringLine( std::string nextErrorTextLine )
{
	std::stringstream ss;
	ss << this->m_lastError << std::endl;
	ss << nextErrorTextLine << std::endl;
	this->m_lastError = ss.str();
	return;
}