//CheckMDSCommand.h
#include "Command.h"

class DataStructure;

class CheckMDSCommand : public Command {
private:
	~CheckMDSCommand();
	void execute(DataStructure*);
};
