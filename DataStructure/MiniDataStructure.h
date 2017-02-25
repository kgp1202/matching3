//MiniDataStructure.h
#include "MiniPerson.h"
#include "boost/shared_ptr.hpp"
#include <vector>
#include <pthread.h>

class MiniDataStructure{
public:
	bool add(int key);
	bool remove(int* keys);
	boost::shared_ptr<std::vector<int> > check();
private:
	std::vector<MiniPersoon> _vector;
	pthread_mutex_t _mutex;

};
