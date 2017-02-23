//CommandQueue.h
#include <queue>
#include <boost/shared_ptr.hpp>
#include <pthread.h>

class Command;

class CommandQueue {
public:
	boost::shared_ptr<Command> pop();
	bool push(boost::shared_ptr<Command>);
	std::size_t size();
private:
	std::queue<boost::shared_ptr<Command> > _queue;
	pthread_mutex_t _mutex;
};
