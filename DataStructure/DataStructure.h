//DataStructure.h
#include "boost/shared_ptr.hpp"
#include "boost/shared_array.hpp"
#include <set>
#include <vector>
#include "DataStructure/MiniDataStructure.h"
#include "BasicModule/MLog.h"

//FOR DEBUG============================
#include <stdio.h>	//FOR DEBUG
//=====================================


class Person;
class CommandQueue;

struct PersonCompare{
	bool operator() (const boost::shared_ptr<Person>& lhs
				, const boost::shared_ptr<Person> & rhs);
};

class DataStructure {
public:
	explicit DataStructure(CommandQueue*);
	~DataStructure();	

	void add(int socket, boost::shared_array<char> msg);	//AddDSCommand
	void add(int socket, boost::shared_ptr<Person> personPtr);
	void checkMDS();					//CheckMDSCommand
//	void changeDS(Person* ptr);				//RecvCommand
	void changeDS(int socket, int distance);
	void removeClient(int socket);				//RecvCommand로
						//read 0을 리턴된 경우에 호출.

	typedef std::set<boost::shared_ptr<Person> >::iterator DSIterator;	

	//FOR DEBUG
	void print();
	
private:
	std::set<boost::shared_ptr<Person>, PersonCompare > _set;
	pthread_mutex_t* _mutex;
	MiniDataStructure* _mds;		//Not Smart Ptr
	CommandQueue* _commandQueue;		//Not Smart Ptr 

	//Mutex 처리가 안되어 있음 -----------------------------------------------------
	bool isPossible(DSIterator);		//key값을 가진 Person이 매칭이 가능한지
						//확인 시켜준다. send와 remove를 진행.

	void sendAndRemove(boost::shared_array<DSIterator>);	//isPossible 이후에 전송
								//해당 socket에 전송, 삭제
							

//	void remove(boost::shared_array<DSIterator>);	//send 이후에 전송전에 remove호출
							//하여 DS에서 해당 Person 삭제.
};	



