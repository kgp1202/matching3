//Person.h

//Person 객체는 DataStructure's Mutex에 의해서
//Thread safe를 보장받는다.(생성시 MMemoryPool을 이용하기 때문)

class Person {
public:
	Person(int distance, int hopeNum, int waitAdvantage);
	setValue(int distance, int hopeNum, int waitAdvantage);			
private:
//	int _socket;		key value 로 DataSturcture가 지니고 있다.
	int _distance;		//거리
	int _hopeNum;		//희망 인원
	int _waitAdvantage;	//대기 시간에 따른 어드밴티지
};


