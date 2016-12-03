#ifndef _CCOMMANDGROUP_HG_
#define _CCOMMANDGROUP_HG_


#include "iCommand.h"
#include <vector>

class cCommandGroup
{
public:
	void AddSerialCommand(iCommand* pTheCommand);

	void AddParallelCommand(iCommand* pTheCommand);
	// When all the commands in this group are done, the group is done, also

	void Update(float deltaTime);	// Called over and over
	bool IsDone(void);

	void AddCommandGroup(const cCommandGroup &theCG);
private:
	std::vector< iCommand* > vecSerialCommands;
	std::vector< iCommand* > vecParallelCommands;
	// Assumption is that command groups are processed in a serial mode
	// (so never in a parallel mode, though you could do that if you want)
	std::vector<cCommandGroup > vecCommandGroups;
};

#endif // !_CCOMMANDGROUP_HG_
