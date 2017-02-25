//DataStructure.h
#include <map>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "MiniDataStructure.h"

class Person;
class CommandQueue;

class DataStructre {
public:
	explicit DataStructure(CommandQueue* commandQueue);
	bool add(int key, boost::shared_ptr<Person>);		//AddDSCommand
	inline std::vector<int> checkMDS();			//CheckMDSCommand
	MiniDataStructure* getMDS();
	boost::shared_ptr<Person> getPerson(int key);
private:
	std::map<int, boost::shared_ptr<Person> > _map;
	pthread_mutex_t _mutex;
	MiniDataStructure* _mds;			//Not Smart Ptr
	CommandQueue* _commandQueue;			//Not Smart Ptr 
							//You should DELETE!!
	
	bool wake(const std::vector<int>& updateKey);		
							//CheckMDSCommand 이후에 _waitAdvantage를
							//update시켜준다.
	
	bool isPossible(int key);			//key값을 가진 Person이 매칭이 가능한지
							//확인 시켜주고 이에 맞는 매칭 정보를
							//전송시켜주는 함수

	bool remove(int* keys);				//isPossible 이후에 전송전에 remove호출
							//하여 DS에서 해당 Person 삭제.
};	
