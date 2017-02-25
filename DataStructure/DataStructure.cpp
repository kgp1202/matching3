//DataStructure.cpp
#include "DataStructure/DataStructure.h"
#include "Command/CommandQueue.h"
#include "BasicModule/MLock.h"
#include <pthread.h>

DataStructure::DataStructure(CommandQueue* commandQueue)
: _commandQueue(commandQueue), _mds(), _map()
{
	pthread_mutex_init(_mutex, NULL);
}

DataStructure::~DataStructure()
{
	pthread_mutex_detory(_mutex);
	delete _mds;
	delete _commandQueue;
}

void DataStructure::add(int socket, char* msg)
{
	if(socket <= 0 || msg == NULL)
		criticalLog("add() in DataStructure.cpp\n socket is 0 or msg is NULL\n");

	MLock* mlock = new MLock(_mutex);	//lock
	
	//msg 해석, Person 객체 생성		
	//미완성@@@@@@@@@@@@@@@ 

	
	boost::shared_ptr<Person> personPtr(new Person(socket, distance, hopeNum));
	int priority = distance + hopeNum;		//우선순위를 int형으로 변환
	
	_map.insert(pair<int, boost::shared_ptr<Person> >(priority, personPtr));

	if(!isPossible(priority))
	{
		delete mlock;
		
		_mds->add(priority);
	}
}

void DataStructure::checkMDS()
{
	boost::shared_ptr<std::vector<MiniPerson> > wakingList = _mds->check();
	
	if(!wakingList.empty())
	{
		MLock(_mutex);
		
		std::vector<MiniPerson>::iterator iter;
		std::map<int, boost::shared_ptr<Person> > mapIter;
		for(iter = wakingList->begin(); iter != wakingList->end(); iter++){
			do{
				mapIter = _map.find(iter->getPriority());
				if(mapIter == _map.end()){
					//이미 전송된 경우 가능함
					//criticalLog("checkMDS() in DataStructure.cpp\n");
					MLog::writeLog("checkMDS() in DataStructure.cpp\n"
						+ "already send\n");
				}
			} while(mapIter->getSocket() != iter->getSocket());
			
			int newPriority = mapIter->addWaitAdvantage();
			_map.insert(newPriority, *mapIter);
			_map.erase(mapIter);

			//미완성 @@@@@@@@@@@@@@@@@@@@@@@
			if(!isPossible(newPriority)){
				
			}
		}
	}
}

boost::shared_ptr<Person> DataStructure::getPerson(int key)
{
	MLock(_mutex);
	
	return _map.find(key);
}

