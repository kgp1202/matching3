//Person.h
#include "Constant.h"
#include "BasicModule/MMemoryPool.h"
#include "boost/shared_ptr.hpp"

//Person 객체는 DataStructure's Mutex에 의해서
//Thread safe를 보장받는다.(생성시 MMemoryPool을 이용하기 때문)

class Person : public MMemoryPool<Person> {
public:
	Person(int socket, int distance);
	Person(int socket, int distance, int waitAdvantage);
	Person(boost::shared_ptr<Person>);
	inline void setValue(int socket, int distance, int waitAdvantage){
		_socket = socket;
		_distance = distance;
		_waitAdvantage = waitAdvantage;
	}
	
	inline int addWaitAdvantage(){		//waitAdvantage값을 1 증가시키고
		_waitAdvantage++;		//계산된 Priority를 넘겨준다.
		return getPriority();
	}
	
	inline int getSocket(){ return _socket; };
	
	inline int getDistance(){ return _distance; };
	
	inline int getPriority() const{
		if(_socket == LSP_SOCKET)	return -2000000000;
		else if(_socket == RSP_SOCKET)	return 2000000000;
		else				return _distance + _waitAdvantage * WAIT_ADVANTAGE; 
	}
private:
	int _socket;		//socket Number
	int _distance;		//거리
	int _waitAdvantage;	//대기 시간에 따른 어드밴티지
};

