#ifndef _GELSHADERMANAGER_HG_
#define _GELSHADERMANAGER_HG_

#include <string>
#include <map>
#include <vector>

class gelShaderManager
{
public:
	//Singleton
	static gelShaderManager& sharedInstance()
	{
		static gelShaderManager instance;
		return instance;
	}

	class cShader {
	public:
		cShader();
		~cShader();
		enum eShaderType
		{
			VERTEX_SHADER,
			FRAGMENT_SHADER,
			UNKNOWN
		};
		eShaderType shaderType;
		std::string getShaderTypeString(void);

		unsigned int ID;	// or "name" from OpenGL
							//		std::string Source;
		std::vector<std::string> vecSource;
		bool bSourceIsMultiLine;
		std::string fileName;
	};

	class cShaderProgram {
	public:
		cShaderProgram() : ID(0) {};
		~cShaderProgram() {};
		unsigned int ID;	// ID from OpenGL (calls it a "name")
		std::string friendlyName;	// We give it this name
	};

	bool useShaderProgram(unsigned int ID);
	bool useShaderProgram(std::string friendlyName);
	bool createProgramFromFile(std::string friendlyName,
		cShader &vertexShad,
		cShader &fragShader);
	unsigned int getIDWithName(std::string friendlyName);

	// Clears last error
	std::string getLastError(void);
private:
	gelShaderManager() {};
	gelShaderManager(gelShaderManager const&); //dont implement - to build
	void operator=(gelShaderManager const&); //dont implement - to build

	bool m_loadSourceFromFile(cShader &shader);
	bool m_compileShaderFromSource(cShader &shader, std::string &error);
	// returns false if no error
	bool m_wasThereACompileError(unsigned int shaderID, std::string &errorText);
	bool m_wasThereALinkError(unsigned int programID, std::string &errorText);
	
	std::string m_lastError;

	std::map< unsigned int /*ID*/, cShaderProgram > m_ID_to_Shader;
	std::map< std::string /*name*/, unsigned int /*ID*/ > m_name_to_ID;
};


#endif // !_GELSHADERMANAGER_HG_
