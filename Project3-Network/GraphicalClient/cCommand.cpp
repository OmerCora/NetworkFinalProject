#include "cCommand.h"

//JSON
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>

cCommand::cCommand()
{
}

cCommand::~cCommand()
{
}


void cCommand::SetGameObject(iGameObject* pGameObject)
{
	this->gameObject = pGameObject;
	return;
}

void cCommand::Init(rapidjson::Value& initJSON)
{
	// TO init stuff here: calculate init positions, velocity, phase of moon, etc.

	return;
}

// Returns true if all is well
bool cCommand::Execute(float deltaTime)		// Called over and over again
{
	return true;
}

bool cCommand::IsDone(void)
{

	// Does nothing command, so returns true right away
	return true;
}
