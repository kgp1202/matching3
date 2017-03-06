//MiniDataStructure.cpp
#include "DataStructure/MiniDataStructure.h"
#include "BasicModule/MLock.h"
#include "BasicModule/MLog.h"
#include <stdio.h>

MiniDataStructure::MiniDataStructure()
: _vector()
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

	MiniPerson temp = _vector.at(socket);
	temp.setValue(iter);
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
			MLog::writeLog("check() in MDS.cpp\nMDS's _vector is empty\n");
			continue;
		}
		else
		{
			//시간 계산
			if((*iter).timeCheck()){
				returnVector[count] = (*iter).getIter();
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
