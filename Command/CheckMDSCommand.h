//CheckMDSCommand.h
#include "Command.h"

class DataStructure;

class CheckMDSCommand : public Command {
public:
	~CheckMDSCommand();
	void execute(DataStructure*);
};
