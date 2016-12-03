#ifndef _CCOMMAND_HG_
#define _CCOMMAND_HG_

#include "iCommand.h"


class cCommand: public iCommand
{
public:
	cCommand();
	~cCommand();
	virtual void SetGameObject(iGameObject* pGameObject);

	// To make this more abstract, you could pass a more general 
	//  item to the Init, like a string, a vector of name-value pairs,
	//  some JSON or XML text, etc. 
	virtual void Init(rapidjson::Value& initJSON);
	virtual bool Execute(float deltaTime);		// Called over and over again
	virtual bool IsDone(void);

	//bool ParseCommandString( std::string theCommand );
private:
	iGameObject* gameObject;
};

#endif // !_CCOMMAND_HG_
