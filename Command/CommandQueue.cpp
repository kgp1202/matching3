//CommandQueue.cpp
#include "Command/CommandQueue.h"
#include "BasicModule/MLock.h"
#include "boost/shared_ptr.hpp"
#include <queue>
#include <pthread.h>

CommandQueue::CommandQueue()
: _queue()
{	
	pthread_mutex_init(&_mutex, NULL);
}

CommandQueue::~CommandQueue()
{
	pthread_mutex_destroy(_mutex);
}

boost::shared_ptr<Command> CommandQueue::pop()
{
	MLock(_mutex);
	
	return _queue.pop();
}

void CommandQueue::push(boost::shared_ptr<Command> commandPtr)
{
	MLock(_mutex);	

	_queue.push(commandPtr);
}

std::size_t CommandQueue::size()
{
	MLock(_mutex);

	return _queue.size();
}	
