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

//FOR DEBUG=======================
#include <stdio.h>	//FOR DEBUG
//================================

#include <sstream>
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

DataStructure::DataStructure(CommandQueue* commandQueue)
: _commandQueue(commandQueue), _mutex(new pthread_mutex_t())
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

	printf("%s\n", msg.get());

	boost::property_tree::ptree tree;
	std::istringstream is(msg.get());
	boost::property_tree::read_json(is, tree);


	int distance = atoi(tree.get<std::string>("distance").c_str());

	boost::shared_ptr<Person> personPtr(new Person(socket, distance));

	add(socket, personPtr);

	printf("%s add complete\n", msg.get());	
}

void DataStructure::add(int socket, boost::shared_ptr<Person> personPtr){
	MLock* mlock = new MLock(_mutex);	//lock
	
	std::pair<DataStructure::DSIterator, bool> ret;	
	ret = _set.insert(personPtr);

	if(ret.second == false){
		MLog::criticalLog("add() in datastructure.cpp\nalready exist\n");
	}
	
	DataStructure::DSIterator insertedIter = ret.first;

	if(!isPossible(insertedIter))
	{
		delete mlock;			//unlock

		_mds->add(socket, insertedIter);
	}else{
		delete mlock;
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

void DataStructure::changeDS(int socket, int distance)
{
	DataStructure::DSIterator dsIter = _mds->getPerson(socket);
	Person* changedPerson;
	{
		MLock m(_mutex);
		changedPerson = new Person(socket, distance);
		_set.erase(dsIter);
	}

	boost::shared_ptr<Person> personPtr(changedPerson);
	add(changedPerson->getSocket(), personPtr);
}

void DataStructure::removeClient(int socket)
{
	printf("remove : %d\n",socket);
	DataStructure::DSIterator deletedIter = _mds->getPerson(socket);

	//Remove
	_set.erase(deletedIter);
	
	//_mds remove
	_mds->remove(socket);

	printf("remove : %dcomplete\n", socket);
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
		printf("Send %d\n", targetSocket);	


		//Remove
		_set.erase(targetIterator);
		
		//_mds remove
		_mds->remove(targetSocket);
	}
}


bool PersonCompare::operator() (const boost::shared_ptr<Person>& lhs
					,const boost::shared_ptr<Person>& rhs){
	int lhsP = lhs->getPriority();
	int rhsP = rhs->getPriority();

	if(lhsP == rhsP)
		return true;
	else
		return lhsP < rhsP;
}

//FOR DEBUG
void DataStructure::print(){
	printf("\n=================DataStructure print======================\n");		

	{		
		MLock m(_mutex);		

		DSIterator iter;
		for(iter = _set.begin(); iter != _set.end(); iter++){
			printf("%d socket = %d distance = %d waitAdvantage = %d\n", 
				&(*iter), (*iter)->getSocket(), (*iter)->getDistance()
				, (*iter)->getPriority());
		}
	}

	_mds->print();
}
