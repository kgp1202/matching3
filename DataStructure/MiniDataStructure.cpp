//MiniDataStructure.cpp
#include "DataStructure/MiniDataStructure.cpp"
#include "BasicModule/MLock.h"
#include "BasicModule/MLog.h"

MiniDataStructure::MiniDataStructure()
: _vector()
{
	pthread_mutex_init(_mutex, NULL);
}

void MiniDataStructure::add(int socket, MiniPerson::DSIterator iter)
{
	if(socket <= 0 || iter == NULL)
		criticalLog("add in MiniDataStructure.cpp\n");

	MLock(_mutex);

	_vector[socket].setValue(iter);
}

void MiniDataStructure::remove(int socket)
{
	if(socket <= 0)
		criticalLog("remove in MiniDataStructure.cpp\n");

	MLock(_mutex);

	_vector[socket].clearValue();
}

boost::shared_ptr<std::vector<MiniPerson> > check()
{
	boost::shared_ptr<std::vector<MiniPerson> > returnVectorPtr;
	std::vector<MiniPerson>::iterator iter;

	int count = 0;
	for(iter = _vector.begin(); iter != _vector.end(); iter++)
	{
		if((*iter).isEmpty())
		{
			continue;
		}else
		{
			//시간 계산
			if((*iter).timeCheck()){
				
			}

		}
	}
	


}
