//Person.h
#include "Constant.h"
#include "BasicModule/MMemoryPool.h"

//Person 객체는 DataStructure's Mutex에 의해서
//Thread safe를 보장받는다.(생성시 MMemoryPool을 이용하기 때문)

class Person : public MMemoryPool<Person> {
public:
	Person(int socket, int distance);
	Person(int socket, int distance, int waitAdvantage);
	setValue(int socket, int distance, int waitAdvantage);
	
	int addWaitAdvantage();	//waitAdvantage값을 1 증가시키고
				//계산된 Priority를 넘겨준다.

	bool operator<(const Person& rhs) const;

	int getSocket();
	int getPriority();
private:
	int _socket;		//socket Number
	int _distance;		//거리
	int _waitAdvantage;	//대기 시간에 따른 어드밴티지
};

