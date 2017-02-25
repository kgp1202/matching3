//MiniDataStructure.h
#include "Constant.h"
#include "MiniPerson.h"
#include "boost/shared_ptr.hpp"
#include <vector>
#include <pthread.h>

class MiniDataStructure{
public:
	explicit MiniDataStructure();
	void add(int priority);
	void remove(int* keys);
	boost::shared_ptr<std::vector<MiniPerson> > check();
private:
	boost::shared_ptr<MiniPerson> _array[MINI_DATASTRUCTURE_SIZE];
	pthread_mutex_t _mutex;
};
