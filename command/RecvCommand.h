//RecvCommand.h
#include "Command.h"

class RecvCommand : public Command {
public:
	explicit RecvCommand(int socket);
	void execute();
private:
	int _socket;
};
