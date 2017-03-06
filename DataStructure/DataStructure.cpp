//DataStructure.cpp
#include "Constant.h"
#include "DataStructure/DataStructure.h"
#include "DataStructure/Person.h"
#include "Command/CommandQueue.h"
#include "BasicModule/MLock.h"
#include "BasicModule/MLog.h"
#include "boost/shared_ptr.hpp"
#include <pthread.h>
#include <string.h>
#include <stdio.h>

DataStructure::DataStructure(CommandQueue* commandQueue)
: _commandQueue(commandQueue)
{
	pthread_mutex_init(_mutex, NULL);

	_mds = new MiniDataStructure();
	
	//Map -1과 MAX값에 Padding
	boost::shared_ptr<Person> lsp (new Person(LSP_SOCKET, 0));	//Left Side Padding
	boost::shared_ptr<Person> rsp (new Person(RSP_SOCKET, 0));	//Right Side Padding

	_set.insert(lsp);
	_set.insert(rsp);
}

DataStructure::~DataStructure()
{
	pthread_mutex_destroy(_mutex);
	delete _mds;
	delete _commandQueue;
}

void DataStructure::add(int socket, boost::shared_array<char> msg)
{
	if(socket <= 0 || msg.get() == NULL)
		MLog::criticalLog("add() in DataStructure.cpp\n socket is 0 or msg is NULL\n");

	MLock* mlock = new MLock(_mutex);	//lock
	
	//msg 해석, Person 객체 생성
	//미완성@@@@@@@@@@@@@@@
	int distance = 0;
	memcpy(&distance, (int*)(msg.get()), sizeof(int));
	
	boost::shared_ptr<Person> personPtr(new Person(socket, distance));
	std::pair<DataStructure::DSIterator, bool> ret;	
	ret = _set.insert(personPtr);

	if(ret.second == false){
		MLog::criticalLog("add() in datastructure.cpp\nalready exist\n");
	}
	
	DataStructure::DSIterator insertedIter = ret.first;

	if(!isPossible(insertedIter))
	{
		delete mlock;

		_mds->add(socket, insertedIter);
	}
}

void DataStructure::checkMDS()
{
	std::vector<DataStructure::DSIterator> wakingList = _mds->check();

	if(!wakingList.empty())
	{
		MLock m(_mutex);
		
		std::vector<DataStructure::DSIterator>::iterator wlIter;//waking list Iterator
		DataStructure::DSIterator newIter;
		for(wlIter = wakingList.begin(); wlIter != wakingList.end(); wlIter++){
			boost::shared_ptr<Person> newPerson(new Person((*(*wlIter))));
			newPerson->addWaitAdvantage();

			_set.erase(*wlIter);
			std::pair<DataStructure::DSIterator, bool> ret;	
			ret = _set.insert(newPerson);

			if(ret.second == false){
				MLog::criticalLog("add() in datastructure.cpp\nalready exist\n");
			}
			newIter = ret.first;

			isPossible(newIter);
		}
	}
}

void DataStructure::changeDS(Person* changedPerson)
{
	MLock m(_mutex);

	DataStructure::DSIterator dsIter = _mds->getPerson(changedPerson->getSocket());

	boost::shared_ptr<Person> tp = (*dsIter);
	tp.reset(changedPerson);

	_set.erase(*dsIter);
	_set.insert(*dsIter);
}

bool DataStructure::isPossible(DataStructure::DSIterator inputIter)
{
	if((*inputIter).get() == NULL)
		MLog::criticalLog("isPossible in DataStructure.cpp\n inputIter is NULL\n");
	
	DataStructure::DSIterator dsIterFront = inputIter;
	DataStructure::DSIterator dsIterBack = inputIter;	
	int inputPriority = (*inputIter)->getPriority();

	int count = 0;
	boost::shared_array<DataStructure::DSIterator> 
		sendingIters(new DataStructure::DSIterator[4]);

	for(int searchCount = 0; searchCount < 3; searchCount++){
		//_set내부에 begin()과 end() 부분에
		//Padding이 존재.
		if(dsIterFront != _set.end())
		{
			dsIterFront++;
			
			//Matching 기준에 부합하는 값인 경우에
			if((*dsIterFront)->getPriority() - inputPriority < MATCHING_LIMIT){
				sendingIters[count] = dsIterFront;
				count++;
			}else {
				dsIterFront = _set.end();
				searchCount--;
			}
		}
		else if(dsIterBack != _set.begin())
		{
			dsIterBack--;
			
			//Matching 기준에 부합하는 값인 경우에
			if(inputPriority - (*dsIterBack)->getPriority() < MATCHING_LIMIT){
				sendingIters[count] = dsIterBack;
				count++;
			}else {
				printf("a2\n");
				dsIterFront = _set.end();
				searchCount--;
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
		sendingIters[count] = inputIter;
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
	if(dsIterators.get() != NULL){
		MLog::writeLog("sendAndRemove in DataStructure.cpp\n dsIterator is not 4\n");	
	}

	int targetSocket;
	DataStructure::DSIterator targetIterator;
	for(int i = 0; i < 4; i++){
		targetIterator = dsIterators[i];
		targetSocket = (*targetIterator)->getSocket();
	
		//Send@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		


		//Remove
		_set.erase(targetIterator);
		
		//_mds remove
		_mds->remove(targetSocket);
	}
}


bool PersonCompare::operator() (const boost::shared_ptr<Person>& lhs
					,const boost::shared_ptr<Person>& rhs){
	return lhs->getPriority() < rhs->getPriority();
}
