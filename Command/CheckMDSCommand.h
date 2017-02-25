//CheckMDSCommand.h
#include "Command.h"

class DataStructure;

class CheckMDSCommand : public Command {
private:
	void execute(DataStructure*);
};
