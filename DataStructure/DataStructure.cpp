//DataStructure.cpp
#include "Constant.h"
#include "DataStructure/DataStructure.h"
#include "Command/CommandQueue.h"
#include "BasicModule/MLock.h"
#include <pthread.h>

DataStructure::DataStructure(CommandQueue* commandQueue)
: _commandQueue(commandQueue), _mds(), _map()
{
	pthread_mutex_init(_mutex, NULL);
	
	//Map -1과 MAX값에 Padding
	boost::shared_ptr<Person> nullPtr;
	_map.insert(std::pair<int, boost::shared_ptr<Person> >(-1, nullPtr));
	_map.insert(std::pair<int, boost::shared_ptr<Person> >(2000000000, nullPtr));
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
	
	DataStructure::mapIterator insertedIter
		 = _map.insert(pair<int, boost::shared_ptr<Person> >(priority, personPtr));

	if(!isPossible(insertedIter))
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
			DataStructure::mapIterator newIter = _map.insert(newPriority, *mapIter);
			_map.erase(mapIter);

			isPossible(newIter);
		}
	}
}

boost::shared_ptr<Person> DataStructure::getPerson(int key)
{
	if(key <= 0)
		MLog::criticalLog("getPerson in DataStructure.cpp\n parameter is 0\n");

	MLock(_mutex);
	
	return _map.find(key);
}

bool DataStructure::isPossible(DataStructure::mapIterator inputIter)
{
	if(inputIter == NULL)
		MLog::criticalLog("isPossible in DataStructure.cpp\n inputIter is NULL\n");
	
	DataStructure::mapIterator mapIterFront = inputIter;
	DataStructure::mapIterator mapIterBack = inputIter;

	int count = 0;
	boost::shared_array<DataStructure::mapIterator> 
		sendingIters(new DataStructure::mapIterator[4]);
	for(int searchCount = 0; searchCount < 3; searchCount++){
		//_map내부에 begin()과 end() 부분에
		//Padding이 존재.
		if(mapIterFront != _map.end())
		{
			mapIterFront++;
			
			//Matching 기준에 부합하는 값인 경우에
			if((*mapIterFront).first - priority < MATCHING_LIMIT){
				sendingIters[count] = mapIterFront;
				count++;
			}else {
				mapIterFront = _map.end();
				searchingCount--;
			}
		}
		else if(mapIterBack != _map.begin())
		{
			mapIterBack--;
			
			//Matching 기준에 부합하는 값인 경우에
			if(priority - (*mapIterBack).first < MATCHING_LIMIT){
				sendingIters[count] = mapIterBack;
				count++;
			}else {
				mapIterFront = _map.end();
				searchingCount--;
			}
		}
		else 
		{
			break;
		}
	}

	//Search 결과 Possible일 때,
	if(count == 3)
	{
		//Send & Remove
		sendAndRemove(sendingIters);

		return true;
	}
	//Search 결과 Impossible일 때,
	else
	{
		return false;
	}
}


