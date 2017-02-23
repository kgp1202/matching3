//AddDSCommand.h
#include "Command.h"

//Add to DataStructure Command.
class AddDSCommand : public Command {
public:
	void execute();
private:
	int _acceptedSocket;
};
