//DataStructure.h
#include <map>
#include <boost/shared_ptr.hpp>
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

	//inline MiniDataStructure* getMDS();
	inline boost::shared_ptr<Person> getPerson(int key);
private:
	std::map<int, boost::shared_ptr<Person> > _map;
	pthread_mutex_t* _mutex;
	MiniDataStructure* _mds;		//Not Smart Ptr
	CommandQueue* _commandQueue;		//Not Smart Ptr 
	
	bool isPossible(int key);		//key값을 가진 Person이 매칭이 가능한지
						//확인 시켜준다.

	void send(int* keys);			//isPossible 이후에 전송
	
	void remove(int* keys);			//send 이후에 전송전에 remove호출
						//하여 DS에서 해당 Person 삭제.
};	
