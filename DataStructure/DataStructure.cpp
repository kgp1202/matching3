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
	_set.insert(boost::shared_ptr<Person>(-1, nullPtr));
	_set.insert(boost::shared_ptr<Person>(2000000000, nullPtr));
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

	
	boost::shared_ptr<Person> personPtr(new Person(socket, distance));
	DataStructure::DSIterator insertedIter = _set.insert(personPtr);

	if(!isPossible(insertedIter))
	{
		delete mlock;
		
		_mds->add(socket, insertedIter);
	}
}

void DataStructure::checkMDS()
{
	boost::shared_ptr<std::vector<MiniPerson> > wakingList = _mds->check();
	
	if(!wakingList.empty())
	{
		MLock(_mutex);
		
		std::vector<MiniPerson>::iterator wlIter;	//waking list Iterator
		DataStructure::DSIterator dsIter;
		for(wlIter = wakingList->begin(); wlIter != wakingList->end(); wlIter++){
			dsIter = (*wlIter).getIter();

			boost::shared_ptr<Person> newPerson(*dsIter);
			_set.erase(dsIter);
			_set.insert(newPerson);

			isPossible(newIter);
		}
	}
}

boost::shared_ptr<Person> DataStructure::getPerson(int key)
{
}

bool DataStructure::isPossible(DataStructure::DSIterator inputIter)
{
	if(inputIter == NULL)
		MLog::criticalLog("isPossible in DataStructure.cpp\n inputIter is NULL\n");
	
	DataStructure::DSIterator dsIterFront = inputIter;
	DataStructure::DSIterator dsIterBack = inputIter;
	int inputPriority = (*inputIter).getPriority();

	int count = 0;
	boost::shared_array<DataStructure::DSIterator> 
		sendingIters(new DataStructure::DSIterator[4]);
	for(int searchCount = 0; searchCount < 3; searchCount++){
		//_set내부에 begin()과 end() 부분에
		//Padding이 존재.
		if(dsIterFront != _map.end())
		{
			dsIterFront++;
			
			//Matching 기준에 부합하는 값인 경우에
			if((*dsIterFront).getPriority() - inputPriority < MATCHING_LIMIT){
				sendingIters[count] = dsIterFront;
				count++;
			}else {
				dsIterFront = _set.end();
				searchingCount--;
			}
		}
		else if(dsIterBack != _set.begin())
		{
			dsIterBack--;
			
			//Matching 기준에 부합하는 값인 경우에
			if(inputPriority - (*dsIterBack).getPriority() < MATCHING_LIMIT){
				sendingIters[count] = dsIterBack;
				count++;
			}else {
				dsIterFront = _set.end();
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

//현재 sendAndRemove는 Mutex를 두개 소모한다.
//처음에 DS's mutex를 이용하여 isPossible를 구한 후에 sendAndRemove로 넘어오면
//MDS remove 를 호출하면서 MDS's mutex도 소모한다.
//개선은 가능하나 모양이 더러워진다!
void DataStructure::sendAndRemove(boost::shared_array<DataStructure::DSIterator> dsIterators)
{
	int size = dsIterators->size();
	if(size != 4){
		MLog::writeLog("sendAndRemove in DataStructure.cpp\n dsIterator is not 4\n");	
	}

	int targetSocket;
	DataStructure::DSIterator targetIterator;
	for(int i = 0; i < size; i++){
		targetIterator = dsIterators[i];
		targetSocket = (*targetIterator)->getSocket();
	
		//Send
		


		//Remove
		_set.erase(targetIterator);
		
		//_mds remove
		_mds->remove(targetSocket);
	}
}
