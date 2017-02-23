//SendCommand.h
#include "Command.h"

class SendCommand : public Command {
public:
	explicit SendCommand(int socket, char* msg);
	void execute();
private:
	int _socket;
	char* _msg;
}; 
