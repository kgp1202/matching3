//DataStructure.h
#include <map>
#include "boost/shared_ptr.hpp"
#include "boost/shared_array.hpp"
#include <vector>
#include "MiniDataStructure.h"

class Person;
class CommandQueue;

class DataStructre {
public:
	explicit DataStructure(CommandQueue*);
	~DataStructure();	

	void add(int socket, char* msg);	//AddDSCommand
	inline void checkMDS();			//CheckMDSCommand

	typedef std::map<int, boost::shared_ptr<Person> >::iterator mapIterator;
	
	inline boost::shared_ptr<Person> getPerson(int key);	//getPerson 오류 존재!!!
						//key 값으로는 정확한 Person 이 아닌
						//여러개의 후보가 존재!! (MultiMap이기 때문에)
private:
	std::map<int, boost::shared_ptr<Person> > _map;
	pthread_mutex_t* _mutex;
	MiniDataStructure* _mds;		//Not Smart Ptr
	CommandQueue* _commandQueue;		//Not Smart Ptr 
	

	bool isPossible(mapIterator);		//key값을 가진 Person이 매칭이 가능한지
						//확인 시켜준다. send와 remove를 진행.

	void sendAndRemove(boost::shared_array<mapIterator>);	//isPossible 이후에 전송
						//해당 socket에 전송, 삭제
	
	void remove(boost::shared_array<mapIterator>);	//send 이후에 전송전에 remove호출
							//하여 DS에서 해당 Person 삭제.
};	



