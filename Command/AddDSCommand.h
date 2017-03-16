//AddDSCommand.h
#include "Command.h"

class DataStructure;

//Add to DataStructure Command.
class AddDSCommand : public Command {
public:
	explicit AddDSCommand(int socket);
	~AddDSCommand();
	void execute(DataStructure*);

	//FOR DEBUG
	explicit AddDSCommand(int socket, char* buf);
private:
	int _acceptedSocket;
	char* _buf;
};
