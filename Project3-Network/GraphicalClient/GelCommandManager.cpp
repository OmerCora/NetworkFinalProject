#include "GelCommandManager.h"

void GelCommandManager::AddCommandGroup(cCommandGroup CG)
{
	this->commandGroupQueue.push(CG);
}

void GelCommandManager::Update(float deltaTime)
{
	//serial execution
	// Pick the "top" command group, if there is one 
	if (!this->commandGroupQueue.empty())
	{
		this->commandGroupQueue.front().Update(deltaTime);
		// Is command group done? 
		if (this->commandGroupQueue.front().IsDone())
		{
			this->commandGroupQueue.pop();
		}
	}

	return;
}