//MiniDataStructure.cpp
#include "DataStructure/MiniDataStructure.h"
#include "BasicModule/MLock.h"
#include "BasicModule/MLog.h"
#include <stdio.h>

MiniDataStructure::MiniDataStructure()
: _vector(), _mutex(new pthread_mutex_t())
{
	pthread_mutex_init(_mutex, NULL);
}	

void MiniDataStructure::add(int socket, MiniPerson::DSIterator iter){
	if(socket <= 0 || (*iter).get() == NULL)
		MLog::criticalLog("add in MiniDataStructure.cpp\n");
	
	MLock m(_mutex);
	
	if(_vector.size() <= socket){
		int addedSize = socket + 10;
		_vector.resize(addedSize, MiniPerson());
	}

	_vector.at(socket).setValue(iter);
}

std::vector<MiniPerson::DSIterator> MiniDataStructure::check()
{
	MLock m(_mutex);

	std::vector<MiniPerson::DSIterator> returnVector;
	std::vector<MiniPerson>::iterator iter;

	int count = 0;
	for(iter = _vector.begin(); iter != _vector.end(); iter++)
	{
		if((*iter).isEmpty())
		{
			continue;
		}
		else
		{
			//시간 계산
			if((*iter).timeCheck()){
				returnVector.push_back(iter->getIter());
				count++;
			}
		}
	}
	
	return returnVector;
}

MiniPerson::DSIterator MiniDataStructure::getPerson(int socket)
{
	if(socket <= 0)
		MLog::criticalLog("getPerson in MDS.cpp\n");

	MLock m(_mutex);

	MiniPerson::DSIterator d = _vector[socket].getIter();

	return d;
}

//FOR DEBUG
void MiniDataStructure::print(){
	printf("\n-------------MiniDataStructure print------------------\n");
	std::vector<MiniPerson>::iterator iter;

	MLock m(_mutex);
	for(iter = _vector.begin(); iter != _vector.end(); iter++){
		(*iter).print();
	}
}
