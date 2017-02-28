//MiniDataStructure.h
#include "Constant.h"
#include "MiniPerson.h"
#include "boost/shared_ptr.hpp"
#include <vector>
#include <pthread.h>

class MiniDataStructure{
public:
	explicit MiniDataStructure();

	inline void add(int socket, MiniPerson::DSIterator iter);

	inline void remove(int socket);
	
	boost::shared_ptr<std::vector<MiniPerson> > check();
private:
	vector<MiniPerson> _vector;
	pthread_mutex_t* _mutex;
};
