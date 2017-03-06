//MiniDataStructure.h
#include "Constant.h"
#include "MiniPerson.h"
#include "boost/shared_ptr.hpp"
#include "BasicModule/MLock.h"
#include <vector>
#include <pthread.h>
#include <set>
#include <stdio.h>

class MiniDataStructure{
public:
	explicit MiniDataStructure();

	void add(int socket, MiniPerson::DSIterator iter);

	inline void remove(int socket){
		MLock m(_mutex);
		
		if(socket <= 0 || socket >= _vector.size())
			MLog::criticalLog("remove in MiniDataStructure.cpp\n");

		_vector[socket].clearValue();
	}
	
	std::vector<MiniPerson::DSIterator> check();

	MiniPerson::DSIterator getPerson(int socket);
private:
	std::vector<MiniPerson> _vector;
	pthread_mutex_t* _mutex;
};
