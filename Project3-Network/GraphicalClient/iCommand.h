#ifndef _ICOMMAND_HG_
#define _ICOMMAND_HG_

#include <string>
#include "iGameObject.h"
#include "rapidjson/document.h"

class iCommand
{
public:
	virtual void SetGameObject(iGameObject* pGameObject) = 0;

	// To make this more abstract, you could pass a more general 
	//  item to the Init, like a string, a vector of name-value pairs,
	//  some JSON or XML text, etc. 
	virtual void Init(rapidjson::Value& initJSON) = 0;
	virtual bool Execute(float deltaTime) = 0;		// Called over and over again
	virtual bool IsDone(void) = 0;

	//bool ParseCommandString( std::string theCommand );
protected:
};

#endif 
