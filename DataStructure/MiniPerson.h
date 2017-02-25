//MiniPerson.h
#include "BasicModule/MMemoryPool.h"

//이 객체는 DataStructure's mutex에 의해 Thread safe하다.
class MiniPerson : public MMemoryPool<MiniPerson> {
public:
	explicit MiniPerson(int priority, int startTime);	
	int getPriority();
	int getSocket();
private:
	int _socket;
	int _priority;
	int _startTime;
};
