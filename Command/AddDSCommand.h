//AddDSCommand.h
#include "Command.h"

class DataStructure;

//Add to DataStructure Command.
class AddDSCommand : public Command {
public:
	explicit AddDSCommand();
	void execute(DataStructure*);
private:
	int _acceptedSocket;
};