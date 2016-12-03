#include "cCommandGroup.h"

void cCommandGroup::AddSerialCommand(iCommand* newCommand)
{
	this->vecSerialCommands.push_back(newCommand);
	return;
}

void cCommandGroup::AddParallelCommand(iCommand* newCommand)
{
	this->vecParallelCommands.push_back(newCommand);
	return;
}

//void AddParallel( CCommand theCommand );

void cCommandGroup::Update(float deltaTime)
{
	// ___          _      _    ___                              _    
	/// __| ___ _ _(_)__ _| |  / __|___ _ __  _ __  __ _ _ _  __| |___
	//\__ \/ -_) '_| / _` | | | (__/ _ \ '  \| '  \/ _` | ' \/ _` (_-<
	//|___/\___|_| |_\__,_|_|  \___\___/_|_|_|_|_|_\__,_|_||_\__,_/__/

	// Update serial commmands first (why not?)
	if (!this->vecSerialCommands.empty())	// Are there serial commands
	{
		iCommand* currentCommand = *this->vecSerialCommands.begin();
		currentCommand->Execute(deltaTime);
		// Done?
		if (currentCommand->IsDone())
		{	// Yes, so delete it from the "list"
			this->vecSerialCommands.erase(this->vecSerialCommands.begin());
			// Also delete command 
			delete currentCommand;
		}
	}//if ( ! this->vecSerialCommands.empty() )	

	 // ___               _ _     _    ___                              _    
	 //| _ \__ _ _ _ __ _| | |___| |  / __|___ _ __  _ __  __ _ _ _  __| |___
	 //|  _/ _` | '_/ _` | | / -_) | | (__/ _ \ '  \| '  \/ _` | ' \/ _` (_-<
	 //|_| \__,_|_| \__,_|_|_\___|_|  \___\___/_|_|_|_|_|_\__,_|_||_\__,_/__/
	 // Go through ALL serial commands at the same time
	bool anotherCommandActive = false;
	for (std::vector< iCommand* >::iterator itCMD = this->vecParallelCommands.begin();
		itCMD != this->vecParallelCommands.end(); itCMD++)
	{
		// Is command active?
		iCommand* currentCommand = *itCMD;
		if (!currentCommand->IsDone())
		{	// There's at least one active parallel command
			anotherCommandActive = true;
			currentCommand->Execute(deltaTime);
		}
	}
	//If all the parallel commands are done, then delete them all
	if (!anotherCommandActive)
	{
		for (std::vector< iCommand* >::iterator itCMD = this->vecParallelCommands.begin();
			itCMD != this->vecParallelCommands.end(); itCMD++)
		{
			iCommand* currentCommand = *itCMD;
			delete currentCommand;
		}
		this->vecParallelCommands.clear();
	}//if ( ! bSomeParCommandActive )



	 // ___          _      _    ___                              _  ___                      
	 /// __| ___ _ _(_)__ _| |  / __|___ _ __  _ __  __ _ _ _  __| |/ __|_ _ ___ _  _ _ __ ___
	 //\__ \/ -_) '_| / _` | | | (__/ _ \ '  \| '  \/ _` | ' \/ _` | (_ | '_/ _ \ || | '_ (_-<
	 //|___/\___|_| |_\__,_|_|  \___\___/_|_|_|_|_|_\__,_|_||_\__,_|\___|_| \___/\_,_| .__/__/
	 //                                                                              |_|      
	 // We will assume that we also run any command groups that are present as well
	if (!this->vecCommandGroups.empty())	// Are there serial commands
	{
		this->vecCommandGroups.begin()->Update(deltaTime);
		// Done?
		if (this->vecCommandGroups.begin()->IsDone())
		{	// Yes, so delete it from the "list"
			this->vecCommandGroups.erase(this->vecCommandGroups.begin());
		}
	}//if ( ! this->vecSerialCommands.empty() )	


	return;
}

// When all the commands in this group are done, the group is done, also
bool cCommandGroup::IsDone(void)
{
	//What to do here? 
	// If there are any commands or command groups, then we AREN'T done
	bool yattaZO = true;	// assume we are

	if (!this->vecCommandGroups.empty()) { yattaZO = false; }
	if (!this->vecSerialCommands.empty()) { yattaZO = false; }
	if (!this->vecParallelCommands.empty()) { yattaZO = false; }

	return yattaZO;	// For now
}

void cCommandGroup::AddCommandGroup(const cCommandGroup &theCG)
{
	this->vecCommandGroups.push_back(theCG);
	return;
}

