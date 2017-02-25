//RecvCommand.h
#include "Command.h"

class DataStructure;

//추가적인 정보의 입력이 있을때 이를 처리하는 Command.
class RecvCommand : public Command {
public:
	explicit RecvCommand(int socket);
	void execute(DataStructure*);
private:
	int _socket;
};
