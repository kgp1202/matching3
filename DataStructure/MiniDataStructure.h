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
			MLog::writeLog("remove in MiniDataStructure.cpp %d\n", socket);

		_vector[socket].clearValue();
	}
	
	std::vector<MiniPerson::DSIterator> check();

	MiniPerson::DSIterator getPerson(int socket);
//	MiniPerson* getMiniPerson(int socket);

	bool isEmpty(int socket);

	void setNullIter(MiniPerson::DSIterator);

	//FOR DEBUG
	void print();
	
private:
	MiniPerson::DSIterator nullIter;

	std::vector<MiniPerson> _vector;
	pthread_mutex_t* _mutex;
};
